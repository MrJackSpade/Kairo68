// license:BSD-3-Clause
// copyright-holders:Aaron Giles

#ifndef MAME_SOUND_YMFM_MAME_H
#define MAME_SOUND_YMFM_MAME_H

#pragma once

#include "ymfm/src/ymfm.h"
#include "ymfm/src/ymfm_ssg.h"
#include "kairo68_perf.h"
#include "kairo/ordered_worker.h"

#include <memory>


// set this to control the output sample rate for SSG-based chips
#define SSG_FIDELITY (ymfm::OPN_FIDELITY_MED)



//*********************************************************
//  MAME INTERFACES
//*********************************************************

// ======================> ym_generic_device

// generic base class for a standalone FM device; this class contains the shared
// configuration helpers, timers, and ymfm interface implementation; it also
// specifies pure virtual functions for read/write access, which means it
// can be used as a generic proxy for systems that have multiple FM types that are
// swappable
class ym_generic_device : public device_t, public device_sound_interface, public ymfm::ymfm_interface
{
public:
	// constructor
	ym_generic_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock, device_type type) :
		device_t(mconfig, type, tag, owner, clock),
		device_sound_interface(mconfig, *this),
		m_timer{ nullptr, nullptr },
		m_update_irq(*this),
		m_io_read(*this, 0),
		m_io_write(*this)
	{
	}

	// configuration helpers
	auto irq_handler() { return m_update_irq.bind(); }
	auto io_read_handler(int index = 0) { return m_io_read[index & 1].bind(); }
	auto io_write_handler(int index = 0) { return m_io_write[index & 1].bind(); }

	// read access interface, implemented by the derived chip-specific class
	virtual u8 read(offs_t offset) = 0;
	virtual u8 status_r() = 0;

	// write access interface, implemented by the derived chip-specific class
	virtual void write(offs_t offset, u8 data) = 0;
	virtual void address_w(u8 data) = 0;
	virtual void data_w(u8 data) = 0;

protected:
	// the chip implementation calls this when a write happens to the mode
	// register, which could affect timers and interrupts; our responsibility
	// is to ensure the system is up to date before calling the engine's
	// engine_mode_write() method
	virtual void ymfm_sync_mode_write(uint8_t data) override
	{
		machine().scheduler().synchronize(timer_expired_delegate(FUNC(ym_generic_device::fm_mode_write), this), data);
	}

	// the chip implementation calls this when the chip's status has changed,
	// which may affect the interrupt state; our responsibility is to ensure
	// the system is up to date before calling the engine's
	// engine_check_interrupts() method
	virtual void ymfm_sync_check_interrupts() override
	{
		// if we're currently executing a CPU, schedule the interrupt check;
		// otherwise, do it directly
		auto &scheduler = machine().scheduler();
		if (scheduler.currently_executing())
			scheduler.synchronize(timer_expired_delegate(FUNC(ym_generic_device::fm_check_interrupts), this));
		else
		{
			m_engine->engine_check_interrupts();
			m_checked_now = true; // Kairo68: the synthesis worker's chip checks now too
		}
	}

	// the chip implementation calls this when one of the two internal timers
	// has changed state; our responsibility is to arrange to call the engine's
	// engine_timer_expired() method after the provided number of clocks; if
	// duration_in_clocks is negative, we should cancel any outstanding timers
	virtual void ymfm_set_timer(uint32_t tnum, int32_t duration_in_clocks) override
	{
		if (duration_in_clocks >= 0)
			m_timer[tnum]->adjust(attotime::from_ticks(duration_in_clocks, device_t::clock()), tnum);
		else
			m_timer[tnum]->enable(false);
	}

	// the chip implementation calls this when the state of the IRQ signal has
	// changed due to a status change; our responsibility is to response as
	// needed to the change in IRQ state, signaling any consumers
	virtual void ymfm_update_irq(bool asserted) override
	{
		m_update_irq(asserted ? ASSERT_LINE : CLEAR_LINE);
	}

	// the chip implementation calls this to indicate that the chip should be
	// considered in a busy state until the given number of clocks has passed;
	// our responsibility is to compute and remember the ending time based on
	// the chip's clock for later checking
	virtual void ymfm_set_busy_end(uint32_t clocks) override
	{
		m_busy_end = machine().time() + attotime::from_ticks(clocks, device_t::clock());
	}

	// the chip implementation calls this to see if the chip is still currently
	// is a busy state, as specified by a previous call to ymfm_set_busy_end();
	// our responsibility is to compare the current time against the previously
	// noted busy end time and return true if we haven't yet passed it
	virtual bool ymfm_is_busy() override
	{
		return (machine().time() < m_busy_end);
	}

	// the chip implementation calls this whenever data is read from outside
	// of the chip; our responsibility is to provide the data requested
	virtual uint8_t ymfm_external_read(ymfm::access_class type, uint32_t address) override
	{
		return (type != ymfm::ACCESS_IO) ? 0 : m_io_read[address & 1]();
	}

	// the chip implementation calls this whenever data is written outside
	// of the chip; our responsibility is to pass the written data on to any consumers
	virtual void ymfm_external_write(ymfm::access_class type, uint32_t address, uint8_t data) override
	{
		if (type == ymfm::ACCESS_IO)
			m_io_write[address & 1](data);
	}

	// handle device start
	virtual void device_start() override
	{
		// allocate our timers
		for (int tnum = 0; tnum < 2; tnum++)
			m_timer[tnum] = timer_alloc(FUNC(ym_generic_device::fm_timer_handler), this);

		// remember the busy end time
		save_item(NAME(m_busy_end));
	}

	// Kairo68: what the chip was given, for a copy of it synthesizing on a worker thread
	// (ymfm_device_base::enable_synthesis_worker): the same calls, in the same order.
	enum : u8 { SYNTH_GENERATE, SYNTH_WRITE, SYNTH_ADDRESS, SYNTH_DATA, SYNTH_MODE, SYNTH_CHECK, SYNTH_TIMER, SYNTH_RESET };
	virtual void synthesis_event(u8 op, u8 data, offs_t offset = 0) { }
	bool m_checked_now = false; // a call to the chip checked its interrupts at once

	// timer callbacks
	void fm_mode_write(s32 param) { m_checked_now = false; m_engine->engine_mode_write(param); synthesis_event(SYNTH_MODE, param); }
	void fm_check_interrupts(s32 param) { m_checked_now = false; m_engine->engine_check_interrupts(); synthesis_event(SYNTH_CHECK, 0); }
	void fm_timer_handler(s32 param) { m_checked_now = false; m_engine->engine_timer_expired(param); synthesis_event(SYNTH_TIMER, param); }

	// internal state
	attotime m_busy_end;                 // busy end time
	emu_timer *m_timer[2];               // two timers
	devcb_write_line m_update_irq;       // IRQ update callback
	devcb_read8::array<2> m_io_read;     // up to 2 input port handlers
	devcb_write8::array<2> m_io_write;   // up to 2 output port handlers
};


// ======================> ymfm_device_base

// this template provides most of the basics used by device objects in MAME
// that wrap ymfm chips; it provides basic read/write functions
template<typename ChipClass>
class ymfm_device_base : public ym_generic_device
{
protected:
	static constexpr int OUTPUTS = ChipClass::OUTPUTS;

public:
	// constructor
	ymfm_device_base(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock, device_type type) :
		ym_generic_device(mconfig, tag, owner, clock, type),
		m_stream(nullptr),
		m_chip(*this)
	{
	}

	// read access: update the streams before performing the read
	virtual u8 read(offs_t offset) override { return update_streams().read(offset); }
	virtual u8 status_r() override { return update_streams().read_status(); }

	// write access: update the strams before performing the write
	// Kairo68: and hand each write to the synthesis worker's chip (synthesis_event)
	virtual void write(offs_t offset, u8 data) override { m_checked_now = false; update_streams().write(offset, data); synthesis_event(SYNTH_WRITE, data, offset); }
	virtual void address_w(u8 data) override { m_checked_now = false; update_streams().write_address(data); synthesis_event(SYNTH_ADDRESS, data); }
	virtual void data_w(u8 data) override { m_checked_now = false; update_streams().write_data(data); synthesis_event(SYNTH_DATA, data); }

	// Kairo68: synthesize on a worker thread, in parallel with the emulation (Kairo68#22). The
	// guest reads the chip's status, timers, IRQ and ports, which stay with m_chip on the emulation
	// thread; m_chip no longer generates, and only counts the clocks it would have. A second chip
	// on the worker is given the same calls in the same order, generates every sample at the
	// position the stream asked for it, and the samples are written into the stream before
	// anything reads it. Output is identical to synthesizing here.
	void enable_synthesis_worker()
	{
		static_assert(requires(ChipClass &chip) { chip.advance_generated(1u); }, "the chip cannot count clocks it does not generate");
		if (m_synth_worker)
			return;
		m_synth_intf = std::make_unique<synthesis_interface>();
		m_synth_chip = std::make_unique<ChipClass>(*m_synth_intf);
		// The worker's chip starts as m_chip is now.
		std::vector<uint8_t> blob;
		ymfm::ymfm_saved_state save(blob, true);
		m_chip.save_restore(save);
		ymfm::ymfm_saved_state load(blob, false);
		m_synth_chip->save_restore(load);
		m_synth_worker = std::make_unique<kairo::OrderedWorker<synthesis_batch, synthesis_results, 16>>(
				[this](const synthesis_batch &batch) {
					synthesis_results results;
					for (const synthesis_command &command : batch)
						if (command.op == SYNTH_GENERATE)
							results.push_back(synthesize(command));
						else
							synthesize(command);
					return results;
				}, "kairo-synth");
		m_stream->set_deferred_finish([this] { finish_synthesis(); });
	}

protected:
	// handle device start
	virtual void device_start() override
	{
		// let our parent do its startup
		ym_generic_device::device_start();

		// allocate our stream
		m_stream = device_sound_interface::stream_alloc(0, OUTPUTS, m_chip.sample_rate(device_t::clock()));

		// compute the size of the save buffer by doing an initial save
		ymfm::ymfm_saved_state state(m_save_blob, true);
		m_chip.save_restore(state);

		// now register the blob for save, on the assumption the size won't change
		save_item(NAME(m_save_blob));
	}

	// device reset
	virtual void device_reset() override
	{
		reset_chip();
	}

	// Kairo68: resets the chip, and the synthesis worker's chip at the same point
	void reset_chip()
	{
		m_checked_now = false;
		m_chip.reset();
		synthesis_event(SYNTH_RESET, 0);
	}

	// handle clock changed
	virtual void device_clock_changed() override
	{
		if (m_stream != nullptr)
		{
			finish_synthesis(); // Kairo68: the samples are in the stream before it resamples them
			m_stream->set_sample_rate(m_chip.sample_rate(device_t::clock()));
		}
	}

	// handle pre-saving by filling the blob
	virtual void device_pre_save() override
	{
		// remember the original blob size
		auto orig_size = m_save_blob.size();

		// save the state
		// Kairo68: with a synthesis worker, its chip holds the whole state once it has caught up
		finish_synthesis();
		ymfm::ymfm_saved_state state(m_save_blob, true);
		(m_synth_chip ? *m_synth_chip : m_chip).save_restore(state);

		// ensure that the size didn't change since we first allocated
		if (m_save_blob.size() != orig_size)
			throw emu_fatalerror("State size changed for ymfm chip");
	}

	// handle post-loading by restoring from the blob
	virtual void device_post_load() override
	{
		// populate the state from the blob
		ymfm::ymfm_saved_state state(m_save_blob, false);
		m_chip.save_restore(state);

		// Kairo68: the worker's pending samples belong to the time before the load
		if (m_synth_worker)
		{
			m_synth_batch.clear();
			m_synth_worker->drain([](synthesis_results &&) { });
			ymfm::ymfm_saved_state worker_state(m_save_blob, false);
			m_synth_chip->save_restore(worker_state);
		}
	}

	// sound overrides
	virtual void sound_stream_update(sound_stream &stream) override
	{
		update_internal(stream);
	}

	// update streams
	virtual ChipClass &update_streams()
	{
		m_stream->update();
		return m_chip;
	}

	// internal update helper
	void update_internal(sound_stream &stream, int output_shift = 0)
	{
		KAIRO68_PERF_FM_SCOPE();
		// Kairo68: the synthesis worker generates these samples; finish_synthesis writes them
		if (m_synth_worker)
		{
			synthesis_command command;
			command.op = SYNTH_GENERATE;
			command.shift = output_shift;
			command.start = stream.start_index();
			command.count = stream.samples();
			command.outputs = std::min(stream.output_count(), u32(OUTPUTS));
			if constexpr (requires(ChipClass &chip) { chip.advance_generated(1u); })
				m_chip.advance_generated(command.count);
			submit_synthesis(command);
			return;
		}
		// local buffer to hold samples
		constexpr int MAX_SAMPLES = 256;
		typename ChipClass::output_data output[MAX_SAMPLES];

		// parameters
		int const outcount = std::min(stream.output_count(), u32(std::size(output[0].data)));
		int const numsamples = stream.samples();

		// generate the FM/ADPCM stream
		for (int sampindex = 0; sampindex < numsamples; sampindex += MAX_SAMPLES)
		{
			int cursamples = std::min(numsamples - sampindex, MAX_SAMPLES);
			m_chip.generate(output, cursamples);
			for (int outnum = 0; outnum < outcount; outnum++)
			{
				int eff_outnum = (outnum + output_shift) % OUTPUTS;
				for (int index = 0; index < cursamples; index++)
					stream.put_int(eff_outnum, sampindex + index, output[index].data[outnum], 32768);
			}
		}
	}

	// Kairo68: the synthesis worker (enable_synthesis_worker)
	struct synthesis_command
	{
		u8 op = SYNTH_GENERATE;
		u8 data = 0;
		u32 offset = 0;
		bool checked_now = false; // the call checked m_chip's interrupts at once
		u8 shift = 0;
		u32 count = 0;
		u32 outputs = 0;
		u64 start = 0;
	};
	struct synthesis_result
	{
		u64 start = 0;
		u32 count = 0;
		u32 outputs = 0;
		u8 shift = 0;
		std::vector<s32> samples; // count samples of each output, output by output
	};
	using synthesis_batch = std::vector<synthesis_command>;
	using synthesis_results = std::vector<synthesis_result>;
	static constexpr size_t SYNTH_BATCH = 32;

	// The worker chip's interface: timers, IRQ, busy and ports belong to m_chip. Mode writes and
	// interrupt checks that m_chip's interface deferred arrive as their own events; one it made at
	// once happens here at once too.
	class synthesis_interface : public ymfm::ymfm_interface
	{
	public:
		bool checked_now = false;
		virtual void ymfm_sync_mode_write(uint8_t data) override { }
		virtual void ymfm_sync_check_interrupts() override { if (checked_now) m_engine->engine_check_interrupts(); }
		ymfm::ymfm_engine_callbacks &engine() { return *m_engine; }
	};

	virtual void synthesis_event(u8 op, u8 data, offs_t offset = 0) override
	{
		if (!m_synth_worker)
			return;
		synthesis_command command;
		command.op = op;
		command.data = data;
		command.offset = offset;
		command.checked_now = m_checked_now;
		submit_synthesis(command);
	}

	// Commands go to the worker in batches: one hand-over per command cost as much as the synthesis
	// it saved when a game polls the chip between every write.
	void submit_synthesis(const synthesis_command &command)
	{
		m_synth_batch.push_back(command);
		if (m_synth_batch.size() >= SYNTH_BATCH)
			send_synthesis_batch();
	}

	void send_synthesis_batch()
	{
		if (m_synth_batch.empty())
			return;
		m_synth_worker->submit(std::move(m_synth_batch), [this](synthesis_results &&results) { write_synthesis(results); });
		m_synth_batch = synthesis_batch();
		m_synth_batch.reserve(SYNTH_BATCH);
	}

	// Writes every sample the worker has been asked for into the stream.
	void finish_synthesis()
	{
		if (!m_synth_worker)
			return;
		send_synthesis_batch();
		m_synth_worker->drain([this](synthesis_results &&results) { write_synthesis(results); });
	}

	void write_synthesis(const synthesis_results &results)
	{
		for (const synthesis_result &result : results)
			for (u32 outnum = 0; outnum < result.outputs; outnum++)
			{
				int const eff_outnum = (outnum + result.shift) % OUTPUTS;
				const s32 *samples = &result.samples[size_t(outnum) * result.count];
				for (u32 index = 0; index < result.count; index++)
					m_stream->put_at(eff_outnum, result.start + index, double(samples[index]) / 32768);
			}
	}

	// On the worker thread: the same call m_chip was given, or the samples it would have generated.
	synthesis_result synthesize(const synthesis_command &command)
	{
		synthesis_result result;
		m_synth_intf->checked_now = command.checked_now;
		switch (command.op)
		{
		case SYNTH_GENERATE:
		{
			constexpr u32 MAX_SAMPLES = 256;
			typename ChipClass::output_data output[MAX_SAMPLES];
			result.start = command.start;
			result.count = command.count;
			result.outputs = command.outputs;
			result.shift = command.shift;
			result.samples.resize(size_t(command.outputs) * command.count);
			for (u32 sampindex = 0; sampindex < command.count; sampindex += MAX_SAMPLES)
			{
				u32 const cursamples = std::min(command.count - sampindex, MAX_SAMPLES);
				m_synth_chip->generate(output, cursamples);
				for (u32 outnum = 0; outnum < command.outputs; outnum++)
					for (u32 index = 0; index < cursamples; index++)
						result.samples[size_t(outnum) * command.count + sampindex + index] = output[index].data[outnum];
			}
			break;
		}
		case SYNTH_WRITE: m_synth_chip->write(command.offset, command.data); break;
		case SYNTH_ADDRESS: m_synth_chip->write_address(command.data); break;
		case SYNTH_DATA: m_synth_chip->write_data(command.data); break;
		case SYNTH_MODE: m_synth_intf->engine().engine_mode_write(command.data); break;
		case SYNTH_CHECK: m_synth_intf->engine().engine_check_interrupts(); break;
		case SYNTH_TIMER: m_synth_intf->engine().engine_timer_expired(command.data); break;
		case SYNTH_RESET: m_synth_chip->reset(); break;
		}
		return result;
	}

	// internal state
	sound_stream *m_stream;           // sound stream
	ChipClass m_chip;                 // core chip implementation
	std::vector<uint8_t> m_save_blob; // state saving buffer
	std::unique_ptr<synthesis_interface> m_synth_intf; // Kairo68: the synthesis worker's chip
	std::unique_ptr<ChipClass> m_synth_chip;
	synthesis_batch m_synth_batch;   // commands not yet handed to the worker
	std::unique_ptr<kairo::OrderedWorker<synthesis_batch, synthesis_results, 16>> m_synth_worker;
};


// ======================> ymfm_ssg_device_base

// this template adds SSG support to the base template, using ymfm's internal
// SSG implementation
template<typename ChipClass>
class ymfm_ssg_device_base : public ymfm_device_base<ChipClass>
{
	using parent = ymfm_device_base<ChipClass>;

public:
	// constructor
	ymfm_ssg_device_base(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock, device_type type) :
		ymfm_device_base<ChipClass>(mconfig, tag, owner, clock, type)
	{
	}

protected:
	// sound overrides
	virtual void sound_stream_update(sound_stream &stream) override
	{
		// ymfm outputs FM first, then SSG, while MAME traditionally
		// wants SSG streams first; to do this, we rotate the outputs
		// by the number of SSG output channels
		parent::update_internal(stream, ChipClass::SSG_OUTPUTS);
	}
};

#endif // MAME_SOUND_YMFM_H

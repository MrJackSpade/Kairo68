package com.mrjackspade.kairo68

import com.mrjackspade.kairo.frontend.ControllerBinding
import com.mrjackspade.kairo.frontend.ControllerBindingsCodec
import com.mrjackspade.kairo.frontend.ControllerLayout
import com.mrjackspade.kairo.frontend.GuestKeyboardLayout
import com.mrjackspade.kairo.frontend.GuestKeyboardPage
import com.mrjackspade.kairo.frontend.JisMainBlock
import com.mrjackspade.kairo.frontend.JisMainBlock.RETURN
import com.mrjackspade.kairo.frontend.JisMainBlock.SPACE
import com.mrjackspade.kairo.frontend.JoystickInputRouter
import com.mrjackspade.kairo.frontend.KeyboardKey
import android.view.KeyEvent

/**
 * The X68000 keyboard by its scan codes (the core's keys), and the FM Towns pad in joystick port 1.
 * The JIS main block, 0x01 to 0x35, is [JisMainBlock]; these are the rest.
 */
object X68kKeys {
    const val HOME = 0x36
    const val DEL = 0x37
    const val ROLL_UP = 0x38
    const val ROLL_DOWN = 0x39
    const val UNDO = 0x3A
    const val LEFT = 0x3B
    const val UP = 0x3C
    const val RIGHT = 0x3D
    const val DOWN = 0x3E
    const val CLR = 0x3F
    const val NUM_SLASH = 0x40
    const val NUM_STAR = 0x41
    const val NUM_MINUS = 0x42
    const val NUM_PLUS = 0x46
    const val NUM_EQUAL = 0x4A
    const val NUM_ENTER = 0x4E
    const val NUM_COMMA = 0x50
    const val NUM_DOT = 0x51
    const val SYMBOL = 0x52
    const val REGISTER = 0x53
    const val HELP = 0x54
    const val XF1 = 0x55
    const val XF2 = 0x56
    const val XF3 = 0x57
    const val XF4 = 0x58
    const val XF5 = 0x59
    const val KANA = 0x5A
    const val ROMAJI = 0x5B
    const val CODE = 0x5C
    const val CAPS = 0x5D
    const val INS = 0x5E
    const val HIRAGANA = 0x5F
    const val ZENKAKU = 0x60
    const val BREAK = 0x61
    const val COPY = 0x62
    const val F1 = 0x63
    const val SHIFT = 0x70
    const val CTRL = 0x71
    const val OPT1 = 0x72
    const val OPT2 = 0x73
    const val MAX_CODE = 0x7F

    private val numpad = mapOf(7 to 0x43, 8 to 0x44, 9 to 0x45, 4 to 0x47, 5 to 0x48, 6 to 0x49,
        1 to 0x4B, 2 to 0x4C, 3 to 0x4D, 0 to 0x4F)
    /** F1 to F10. */
    private val function = (0 until 10).map { F1 + it }

    val layout = GuestKeyboardLayout(
        brand = "KAIRO68",
        pages = listOf(
            GuestKeyboardPage("ABC", JisMainBlock.alphabetRows(CTRL, SHIFT, listOf(KeyboardKey("Caps", CAPS),
                KeyboardKey("Kana", KANA), KeyboardKey("XF1", XF1), KeyboardKey("XF2", XF2), KeyboardKey("Space", SPACE, 4f),
                KeyboardKey("XF3", XF3), KeyboardKey("XF4", XF4), KeyboardKey("XF5", XF5)))),
            GuestKeyboardPage("?123", JisMainBlock.symbolRows(), pickerLabel = "Symbols"),
            GuestKeyboardPage("X68000", x68kRows())
        ),
        modifiers = setOf(SHIFT, CTRL, OPT1, OPT2),
        shiftCodes = setOf(SHIFT),
        capsCode = null,
        chordShiftCode = SHIFT
    )

    private fun x68kRows(): List<List<KeyboardKey>> = listOf(
        function.mapIndexed { index, code -> KeyboardKey("F${index + 1}", code) },
        listOf(KeyboardKey("Break", BREAK), KeyboardKey("Copy", COPY), KeyboardKey("Home", HOME),
            KeyboardKey("Ins", INS), KeyboardKey("Del", DEL), KeyboardKey("Undo", UNDO),
            KeyboardKey("R.Up", ROLL_UP), KeyboardKey("R.Down", ROLL_DOWN), KeyboardKey("Help", HELP)),
        listOf(KeyboardKey("OPT.1", OPT1), KeyboardKey("OPT.2", OPT2), KeyboardKey("Hira", HIRAGANA),
            KeyboardKey("Zen", ZENKAKU), KeyboardKey("Roma", ROMAJI), KeyboardKey("Code", CODE),
            KeyboardKey("Symb", SYMBOL), KeyboardKey("Reg", REGISTER)),
        listOf(KeyboardKey("7", numpad.getValue(7)), KeyboardKey("8", numpad.getValue(8)),
            KeyboardKey("9", numpad.getValue(9)), KeyboardKey("*", NUM_STAR), KeyboardKey("/", NUM_SLASH),
            KeyboardKey("-", NUM_MINUS), KeyboardKey("Clr", CLR)),
        listOf(KeyboardKey("4", numpad.getValue(4)), KeyboardKey("5", numpad.getValue(5)),
            KeyboardKey("6", numpad.getValue(6)), KeyboardKey("+", NUM_PLUS), KeyboardKey("=", NUM_EQUAL),
            KeyboardKey(",", NUM_COMMA), KeyboardKey("↑", UP)),
        listOf(KeyboardKey("1", numpad.getValue(1)), KeyboardKey("2", numpad.getValue(2)),
            KeyboardKey("3", numpad.getValue(3)), KeyboardKey("0", numpad.getValue(0)), KeyboardKey(".", NUM_DOT),
            KeyboardKey("Enter", NUM_ENTER), KeyboardKey("←", LEFT), KeyboardKey("↓", DOWN), KeyboardKey("→", RIGHT))
    )

    /** Every key a controller binding can send. */
    val bindableCodes: List<Int> = (1..MAX_CODE).filter { code -> label(code) != null }

    fun label(code: Int): String? = JisMainBlock.label(code) ?: when (code) {
        HOME -> "Home"
        DEL -> "Del"
        ROLL_UP -> "Roll Up"
        ROLL_DOWN -> "Roll Down"
        UNDO -> "Undo"
        LEFT -> "Left"
        UP -> "Up"
        RIGHT -> "Right"
        DOWN -> "Down"
        CLR -> "Clr"
        NUM_SLASH -> "Numpad /"
        NUM_STAR -> "Numpad *"
        NUM_MINUS -> "Numpad -"
        NUM_PLUS -> "Numpad +"
        NUM_EQUAL -> "Numpad ="
        NUM_ENTER -> "Numpad Enter"
        NUM_COMMA -> "Numpad ,"
        NUM_DOT -> "Numpad ."
        in numpad.values -> "Numpad ${numpad.entries.first { it.value == code }.key}"
        SYMBOL -> "Symbol"
        REGISTER -> "Register"
        HELP -> "Help"
        in XF1..XF5 -> "XF${code - XF1 + 1}"
        KANA -> "Kana"
        ROMAJI -> "Romaji"
        CODE -> "Code"
        CAPS -> "Caps"
        INS -> "Ins"
        HIRAGANA -> "Hiragana"
        ZENKAKU -> "Zenkaku"
        BREAK -> "Break"
        COPY -> "Copy"
        in function -> "F${code - F1 + 1}"
        SHIFT -> "Shift"
        CTRL -> "Ctrl"
        OPT1 -> "OPT.1"
        OPT2 -> "OPT.2"
        else -> null
    }

    /**
     * The X68000 key an Android key types: the JIS main block by position, then the other keys as
     * MAME maps a PC keyboard to them (F11 and F12 are XF1 and XF2, End is Undo).
     */
    fun fromAndroid(key: Int): Int? {
        JisMainBlock.fromAndroid(key)?.let { return it }
        if (key in KeyEvent.KEYCODE_F1..KeyEvent.KEYCODE_F10) return function[key - KeyEvent.KEYCODE_F1]
        if (key in KeyEvent.KEYCODE_NUMPAD_0..KeyEvent.KEYCODE_NUMPAD_9) return numpad.getValue(key - KeyEvent.KEYCODE_NUMPAD_0)
        return when (key) {
            KeyEvent.KEYCODE_F11 -> XF1
            KeyEvent.KEYCODE_F12 -> XF2
            KeyEvent.KEYCODE_MUHENKAN -> XF1
            KeyEvent.KEYCODE_HENKAN -> XF2
            KeyEvent.KEYCODE_FORWARD_DEL -> DEL
            KeyEvent.KEYCODE_INSERT -> INS
            KeyEvent.KEYCODE_MOVE_HOME -> HOME
            KeyEvent.KEYCODE_MOVE_END -> UNDO
            KeyEvent.KEYCODE_PAGE_UP -> ROLL_UP
            KeyEvent.KEYCODE_PAGE_DOWN -> ROLL_DOWN
            KeyEvent.KEYCODE_DPAD_UP -> UP
            KeyEvent.KEYCODE_DPAD_DOWN -> DOWN
            KeyEvent.KEYCODE_DPAD_LEFT -> LEFT
            KeyEvent.KEYCODE_DPAD_RIGHT -> RIGHT
            KeyEvent.KEYCODE_NUM_LOCK -> CLR
            KeyEvent.KEYCODE_SYSRQ -> OPT1
            KeyEvent.KEYCODE_BREAK -> OPT2
            KeyEvent.KEYCODE_SHIFT_LEFT, KeyEvent.KEYCODE_SHIFT_RIGHT -> SHIFT
            KeyEvent.KEYCODE_CTRL_LEFT -> CTRL
            KeyEvent.KEYCODE_CTRL_RIGHT -> ZENKAKU
            KeyEvent.KEYCODE_ALT_LEFT -> HIRAGANA
            KeyEvent.KEYCODE_ALT_RIGHT -> XF5
            KeyEvent.KEYCODE_CAPS_LOCK -> CAPS
            KeyEvent.KEYCODE_KATAKANA_HIRAGANA -> KANA
            KeyEvent.KEYCODE_NUMPAD_DIVIDE -> NUM_SLASH
            KeyEvent.KEYCODE_NUMPAD_MULTIPLY -> NUM_STAR
            KeyEvent.KEYCODE_NUMPAD_SUBTRACT -> NUM_MINUS
            KeyEvent.KEYCODE_NUMPAD_ADD -> NUM_PLUS
            KeyEvent.KEYCODE_NUMPAD_EQUALS -> NUM_EQUAL
            KeyEvent.KEYCODE_NUMPAD_ENTER -> NUM_ENTER
            KeyEvent.KEYCODE_NUMPAD_COMMA -> NUM_COMMA
            KeyEvent.KEYCODE_NUMPAD_DOT -> NUM_DOT
            else -> null
        }
    }

    /** The pad's controls in the core's order (x68k_machine.h kJoystickControls). */
    val joystick = JoystickInputRouter.DEFAULT_CONTROLS + listOf("run", "select")
    val joystickLabels = listOf("Up", "Down", "Left", "Right", "A", "B", "RUN", "SELECT")

    val bindings = ControllerBindingsCodec(::defaults, { it in bindableCodes }, joystick,
        setOf("menu", "pause", "restart", "exit", "fastForward"))

    fun defaults(layout: ControllerLayout): List<ControllerBinding> =
        if (layout == ControllerLayout.WITH_STICKS) defaults()
        else defaults().filterNot { it.input.startsWith("virtual:ls") || it.input.startsWith("virtual:rs") }

    /**
     * X68000 games read the pad in joystick port 1: the D-pad is the pad and the cursor keys, A and
     * B are Return and Space, X and Y are the pad's buttons, the left stick moves the mouse with L1
     * and R1 its buttons, L2 and R2 are F1 and F2, and the right stick is the cursor keys.
     */
    fun defaults() = listOf(
        ControllerBinding("virtual:a", listOf(RETURN)),
        ControllerBinding("virtual:b", listOf(SPACE)),
        ControllerBinding("virtual:x", joystick = "button1"),
        ControllerBinding("virtual:y", joystick = "button2"),
        ControllerBinding("virtual:l1", mouse = "leftButton"),
        ControllerBinding("virtual:r1", mouse = "rightButton"),
        ControllerBinding("virtual:l2", listOf(F1)),
        ControllerBinding("virtual:r2", listOf(F1 + 1)),
        ControllerBinding("virtual:start", joystick = "run"),
        ControllerBinding("virtual:select", joystick = "select"),
        ControllerBinding("virtual:menu", action = "menu"),
        ControllerBinding("virtual:up", listOf(UP), joystick = "up"),
        ControllerBinding("virtual:down", listOf(DOWN), joystick = "down"),
        ControllerBinding("virtual:left", listOf(LEFT), joystick = "left"),
        ControllerBinding("virtual:right", listOf(RIGHT), joystick = "right"),
        ControllerBinding("virtual:lsup", mouse = "moveUp"),
        ControllerBinding("virtual:lsdown", mouse = "moveDown"),
        ControllerBinding("virtual:lsleft", mouse = "moveLeft"),
        ControllerBinding("virtual:lsright", mouse = "moveRight"),
        ControllerBinding("virtual:rsup", listOf(UP)),
        ControllerBinding("virtual:rsdown", listOf(DOWN)),
        ControllerBinding("virtual:rsleft", listOf(LEFT)),
        ControllerBinding("virtual:rsright", listOf(RIGHT))
    )
}

// The measurement tool's executable when its core is a shared library (KAIRO68_TOOL_SHARED): the
// tool itself (game_profile.cpp) is in the library with the core it uses, as the app's
// libkairo68.so holds its own code and the core, so calls inside the core bind as they do in the
// app (Kairo68#8). This only starts it.

int kairo68_game_profile_main(int argc, char **argv);

int main(int argc, char **argv) { return kairo68_game_profile_main(argc, argv); }

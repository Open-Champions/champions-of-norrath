// Observed: loc is a 48-byte table in read-only data at 0x0046cc90 holding twelve
// floats, in order: 0, 0, 200, -120, 180, -120, 0, 0, 0, -120, 0, -140. The level-load
// routine and the automap draw routine read it as loc[2 * mode + k] and loc[2 * mode + k + 1]
// with k = 0 for fewer than two players and 6 otherwise (their own notes). The draw routine
// keeps a file-local copy of the same values; this is the global symbol the level-load
// routine imports.
// Inferred: pairs of screen positions (x, y) per automap mode and player-count case; the
// units are not established.
// Unknown: what the zero pairs stand for.
extern "C" const float loc[12] = {0.0f, 0.0f, 200.0f, -120.0f, 180.0f, -120.0f, 0.0f, 0.0f, 0.0f, -120.0f, 0.0f, -140.0f};
static_assert(sizeof(loc) == 48, "observed size");

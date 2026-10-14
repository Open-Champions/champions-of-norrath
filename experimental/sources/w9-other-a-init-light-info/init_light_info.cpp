// Observed initLightInfo__Fv at 0x00280510, 252 bytes, no arguments, no result.
// It fills the first 80 floats of the 384-byte zero-image table lightInfo (0x008d5e40) from an unnamed
// read-only table of 96 floats at 0x00476618: ten rows of eight values are copied row by row, so the last
// 16 floats of lightInfo are not written here. (The original goes through a 384-byte stack copy of the
// whole table first; that copy has no visible effect and is not reproduced.) Only the ten rows are kept
// below, the final two source rows are all zero.
// Inferred: lightInfo holds ten records of eight floats each. Unknown: what the values mean; the first two
// look like angles or offsets (one is negative in two rows) and the rest like channel or size values, but no
// field names are claimed. Callers: select_init (w8-core-loop-d) and possibly others.
extern "C" {
extern float lightInfo[96];
}

namespace {
const float initial_rows[10][8] = {
    {345.0f, 320.0f, 61.0f, 71.0f, 44.0f, 20.0f, 20.0f, 33.0f},
    {-508.0f, 168.0f, 74.0f, 74.0f, 72.0f, 30.0f, 30.0f, 30.0f},
    {336.0f, 9.0f, 84.0f, 99.0f, 72.0f, 21.0f, 20.0f, 23.0f},
    {335.0f, 5.0f, 57.0f, 69.0f, 57.0f, 30.0f, 33.0f, 33.0f},
    {342.0f, 16.0f, 63.0f, 66.0f, 66.0f, 29.0f, 30.0f, 30.0f},
    {309.0f, 14.0f, 92.0f, 95.0f, 87.0f, 31.0f, 32.0f, 31.0f},
    {334.0f, 319.0f, 90.0f, 90.0f, 90.0f, 28.0f, 30.0f, 32.0f},
    {315.0f, -26.0f, 89.0f, 89.0f, 89.0f, 31.0f, 30.0f, 30.0f},
    {317.0f, -36.0f, 85.0f, 85.0f, 85.0f, 35.0f, 35.0f, 35.0f},
    {-27.0f, -37.0f, 91.0f, 90.0f, 90.0f, 30.0f, 31.0f, 23.0f},
};
}

extern "C" void initLightInfo__Fv() {
    for (int row = 0; row < 10; ++row)
        for (int column = 0; column < 8; ++column) lightInfo[row * 8 + column] = initial_rows[row][column];
}

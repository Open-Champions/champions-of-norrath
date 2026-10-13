// Observed: SetProfiles__7VIWorld at 0x010facf8, 2020 bytes. With the world as this it only stores
// constants into the object and returns zero. Offsets are bytes from the object.
//   +88 = 250.0f and +92 = 800.0f.
//   Then twenty-two 40-byte rows starting at +96 (row r starts at 96 + 40 * r), each:
//     +0..+12   four floats: either 1.0f four times, or an 8-bit RGBA colour converted per channel
//               as (float)byte * 0x3b808080 (the single-precision value just under 1/255; the
//               original multiplies at run time, and so does this unit)
//     +16 word, +20 float, +24 word, +28 word (the only nonzero +28 is the float 1.0f in row 19),
//     +32 float and +36 float (row 21 stores neither).
//   The row values are in the table below (observed from the image's stores; rows 0, 4, 7, 8, 12,
//   15 and 16 use the four 1.0f floats).
// Inferred: the 88/92 pair has the same shape as the +32/+36 pair of every row, so it may be the
// pair of a preceding row at +56; the rows are the terrain profiles that
// CalcProfileBlend__7VIWorld indexes (it returns profile ids), with a colour, two settings, a
// kind word and a near/far pair; this is arithmetic on the stores, not a recovered type.
// Unknown: the meaning of every field and of the row numbering.
extern "C" {
struct VIWorld;

struct ProfileRow {
    unsigned char white;       // 1: store 1.0f four times, otherwise convert rgba
    unsigned char rgba[4];
    int word16;
    float float20;
    int word24;
    float float28;             // stored as a float only in row 19; zero elsewhere
    float near_value;
    float far_value;
    unsigned char has_distances;
};

static const ProfileRow rows[22] = {
    {1, {0, 0, 0, 0}, 1, 1.0f, 9, 0.0f, 0.0f, 400.0f, 1},
    {0, {210, 230, 180, 255}, 1, 1.0f, 4, 0.0f, 250.0f, 800.0f, 1},
    {0, {227, 247, 247, 255}, 0, 1.0f, 6, 0.0f, 250.0f, 800.0f, 1},
    {0, {233, 218, 166, 255}, 0, 1.0f, 3, 0.0f, 150.0f, 800.0f, 1},
    {1, {0, 0, 0, 0}, 2, 0.5f, 5, 0.0f, 0.0f, 400.0f, 1},
    {0, {90, 140, 65, 255}, 0, 1.0f, 7, 0.0f, 0.0f, 350.0f, 1},
    {0, {140, 170, 120, 255}, 2, 0.5f, 10, 0.0f, 250.0f, 800.0f, 1},
    {1, {0, 0, 0, 0}, 1, 1.0f, 2, 0.0f, 250.0f, 800.0f, 1},
    {1, {0, 0, 0, 0}, 1, 1.0f, 1, 0.0f, 0.0f, 400.0f, 1},
    {0, {210, 230, 180, 255}, 1, 1.0f, 1, 0.0f, 250.0f, 800.0f, 1},
    {0, {227, 247, 247, 255}, 0, 1.0f, 1, 0.0f, 250.0f, 800.0f, 1},
    {0, {233, 218, 166, 255}, 0, 1.0f, 1, 0.0f, 150.0f, 800.0f, 1},
    {1, {0, 0, 0, 0}, 2, 0.5f, 1, 0.0f, 0.0f, 400.0f, 1},
    {0, {90, 140, 65, 255}, 0, 1.0f, 1, 0.0f, 0.0f, 350.0f, 1},
    {0, {140, 170, 120, 255}, 2, 0.5f, 1, 0.0f, 250.0f, 800.0f, 1},
    {1, {0, 0, 0, 0}, 1, 1.0f, 8, 0.0f, 250.0f, 800.0f, 1},
    {1, {0, 0, 0, 0}, 1, 1.0f, 1, 0.0f, 0.0f, 800.0f, 1},
    {0, {255, 170, 170, 255}, 0, 1.0f, 3, 0.0f, 0.0f, 550.0f, 1},
    {0, {110, 140, 30, 255}, 1, 1.0f, 8, 0.0f, 0.0f, 600.0f, 1},
    {0, {255, 170, 170, 255}, 2, 1.0f, 8, 1.0f, 250.0f, 800.0f, 1},
    {0, {175, 200, 240, 255}, 1, 1.0f, 6, 0.0f, 0.0f, 600.0f, 1},
    {0, {140, 170, 160, 255}, 1, 1.0f, 7, 0.0f, 0.0f, 0.0f, 0},
};

int SetProfiles__7VIWorld(VIWorld* world) {
    unsigned char* bytes = reinterpret_cast<unsigned char*>(world);
    float inv255 = __builtin_bit_cast(float, 0x3b808080u);
    *reinterpret_cast<float*>(bytes + 88) = 250.0f;
    *reinterpret_cast<float*>(bytes + 92) = 800.0f;
    for (int r = 0; r < 22; ++r) {
        const ProfileRow& row = rows[r];
        unsigned char* at = bytes + 96 + 40 * r;
        float* color = reinterpret_cast<float*>(at);
        if (row.white) {
            color[0] = 1.0f; color[1] = 1.0f; color[2] = 1.0f; color[3] = 1.0f;
        } else {
            color[0] = (float)row.rgba[0] * inv255;
            color[1] = (float)row.rgba[1] * inv255;
            color[2] = (float)row.rgba[2] * inv255;
            color[3] = (float)row.rgba[3] * inv255;
        }
        *reinterpret_cast<int*>(at + 16) = row.word16;
        *reinterpret_cast<float*>(at + 20) = row.float20;
        *reinterpret_cast<int*>(at + 24) = row.word24;
        *reinterpret_cast<float*>(at + 28) = row.float28;
        if (row.has_distances) {
            *reinterpret_cast<float*>(at + 32) = row.near_value;
            *reinterpret_cast<float*>(at + 36) = row.far_value;
        }
    }
    return 0;
}
}

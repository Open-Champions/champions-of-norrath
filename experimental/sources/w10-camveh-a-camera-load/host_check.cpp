// Host contract for Load__6CameraPc. Not an EE plan input.
#include "camera_load.cpp"

#include <cstdio>
#include <cstring>

float worldCameraPos[3];
float worldCameraMatrix[16];

static int resets;
static int reset_defaults, reset_fresh;
void Reset__6Cameraii(void* camera, int defaults, int fresh) {
    ++resets; reset_defaults = defaults; reset_fresh = fresh;
    unsigned char* c = static_cast<unsigned char*>(camera);
    const float v[] = {170.0f, 420.0f, 500.0f, 90.0f, 60.0f, 60.0f};
    std::memset(c, 0xee, 512);   // the old content must not leak through
    for (int i = 0; i < 6; ++i) std::memcpy(c + 176 + 4 * i, &v[i], 4);
}

static int failures = 0;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; } }
static float getf(const unsigned char* c, int off) { float v; std::memcpy(&v, c + off, 4); return v; }

int main() {
    unsigned char in[196];
    for (int i = 0; i < 196; ++i) in[i] = static_cast<unsigned char>(i * 7 + 3);
    float f;
    const float pos[3] = {1.5f, -2.5f, 3.5f};
    std::memcpy(in, pos, 12);
    for (int i = 0; i < 16; ++i) { f = 100.0f + i; std::memcpy(in + 12 + 4 * i, &f, 4); }
    short h0 = -1234, h1 = 4321; std::memcpy(in + 76, &h0, 2); std::memcpy(in + 78, &h1, 2);
    int a = 0, b = 2; std::memcpy(in + 80, &a, 4); std::memcpy(in + 84, &b, 4);
    f = 0.25f; std::memcpy(in + 88, &f, 4);
    f = 7.0f; std::memcpy(in + 92, &f, 4);
    f = 295.0f; std::memcpy(in + 96, &f, 4);

    unsigned char camera[512];
    const int n = Load__6CameraPc(camera, in);
    expect(n == 196 && resets == 1 && reset_defaults == 1 && reset_fresh == 0, "reset and byte count");
    expect(std::memcmp(worldCameraPos, pos, 12) == 0, "position");
    // Matrix groups overlap: index 3*row+col, written in order, later rows win.
    expect(worldCameraMatrix[0] == 100 && worldCameraMatrix[3] == 104 && worldCameraMatrix[6] == 108 && worldCameraMatrix[12] == 115, "matrix indices");
    expect(worldCameraMatrix[4] == 105 && worldCameraMatrix[9] == 112, "overlap order");
    short g; std::memcpy(&g, camera + 104, 2); expect(g == -1234, "halfword 104");
    std::memcpy(&g, camera + 106, 2); expect(g == 4321, "halfword 106");
    int w; std::memcpy(&w, camera + 84, 4); expect(w == 0, "word 84");
    std::memcpy(&w, camera + 88, 4); expect(w == 2, "word 88");
    expect(getf(camera, 144) == 0.25f && getf(camera, 212) == 7.0f && getf(camera, 140) == 295.0f, "floats");
    // 1 - (295-170)/(500-170); F(0)=90 (188), F(2)=60 (196)? F(k)=188+4k: F(2)=196=60
    expect(getf(camera, 216) == 1.0f - (295.0f - 170.0f) / (500.0f - 170.0f), "+216");
    expect(getf(camera, 252) == 60.0f + (90.0f - 60.0f) * 0.25f, "+252 blend below one");
    f = 3.0f; std::memcpy(in + 88, &f, 4);
    Load__6CameraPc(camera, in);
    expect(getf(camera, 252) == 60.0f + (90.0f - 60.0f) * 1.0f, "+252 blend clamps at one");
    bool same = true;
    for (int i = 0; i < 96; ++i) if (camera[256 + i] != in[100 + i]) same = false;
    expect(same, "96 trailing bytes");
    return failures ? 1 : 0;
}

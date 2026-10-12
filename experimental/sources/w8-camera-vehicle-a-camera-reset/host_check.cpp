// Host contract for Reset__6Cameraii. Not an EE plan input.
#include "reset.cpp"

#include <cstdio>
#include <initializer_list>
#include <cstring>

static int failures = 0;
static int order[8];
static int steps = 0;
static int yaw_seen, pitch_seen;
static float zoom_seen;
static float build_args[3];
static void* build_point = &failures;
static unsigned char* expected_camera;
static unsigned char sees_yaw[512];

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

int nmPlayersAlive;
int nmPlayers;

extern "C" void SetYaw__6Camerai(void*, int yaw) { order[steps++] = 1; yaw_seen = yaw; }
extern "C" void SetPitch__6Camerai(void*, int pitch) { order[steps++] = 2; pitch_seen = pitch; }
extern "C" void SetZoomDistance__6Cameraf(void*, float d) { order[steps++] = 3; zoom_seen = d; }
extern "C" void BuildMatrix__6CamerafffP6Point3(void* camera, float a, float b, float c, void* point) {
    order[steps++] = 4;
    build_args[0] = a; build_args[1] = b; build_args[2] = c;
    build_point = point;
    std::memcpy(sees_yaw, camera, 512);
}

static void put_half(unsigned char* c, int off, int v) { short s = static_cast<short>(v); std::memcpy(c + off, &s, 2); }
static void put_word(unsigned char* c, int off, int v) { std::memcpy(c + off, &v, 4); }
static void put_float(unsigned char* c, int off, float v) { std::memcpy(c + off, &v, 4); }
static int get_word(const unsigned char* c, int off) { int v; std::memcpy(&v, c + off, 4); return v; }

static void run(int defaults, int fresh, int index, int alive, int players) {
    static unsigned char camera[512], want[512];
    std::memset(camera, 0x3c, sizeof camera);
    put_word(camera, 84, index);
    put_half(camera, 108, -1111); put_half(camera, 110, -2222); put_half(camera, 112, -3333);
    put_float(camera, 176, 1.5f); put_float(camera, 180, 2.5f); put_float(camera, 184, 3.5f);
    std::memcpy(want, camera, sizeof camera);
    nmPlayersAlive = alive;
    nmPlayers = players;
    steps = 0;
    Reset__6Cameraii(camera, defaults, fresh);

    if (defaults) {
        put_half(want, 108, 8000); put_half(want, 110, 10000); put_half(want, 112, 13000);
        const struct { int off; float v; } f[] = {{176, 170.0f}, {180, 420.0f}, {184, 500.0f}, {188, 90.0f}, {192, 60.0f}, {196, 60.0f},
            {220, 150.0f}, {224, 40.0f}, {228, 20.0f}, {232, -20.0f}, {236, -25.0f}, {240, -25.0f}, {164, -1.0f}};
        for (auto& e : f) put_float(want, e.off, e.v);
        put_word(want, 80, 0);
    }
    int i = index;
    if (fresh) { put_word(want, 88, 0); put_word(want, 84, 1); want[353] = 0xff; i = 1; }
    put_half(want, 138, -8192);
    put_word(want, 160, 0); put_word(want, 92, 1);
    want[364] = 0; want[365] = 0;
    short table;
    std::memcpy(&table, want + 108 + 2 * i, 2);
    put_half(want, 116, table);
    put_word(want, 68, -1);
    put_float(want, 144, 1.0f);
    put_word(want, 56, 0); put_word(want, 76, 0);
    for (int off : {152, 156, 200, 204, 208, 212}) put_float(want, off, 0.0f);
    want[366] = static_cast<unsigned char>(alive > 0 ? alive : players);
    std::memset(want + 256, 0, 96);
    put_word(want, 72, 1);
    expect(std::memcmp(camera, want, sizeof camera) == 0, "final camera image");
    expect(steps == 4 && order[0] == 1 && order[1] == 2 && order[2] == 3 && order[3] == 4, "SetYaw, SetPitch, SetZoomDistance, BuildMatrix in order");
    expect(yaw_seen == -8192, "yaw argument");
    expect(pitch_seen == table, "pitch argument is the signed table halfword");
    float zoom;
    std::memcpy(&zoom, want + 176 + 4 * i, 4);
    expect(zoom_seen == zoom, "zoom argument is the float at +176+4*index");
    expect(build_args[0] == 0.0f && build_args[1] == 0.0f && build_args[2] == 0.0f && build_point == nullptr, "BuildMatrix zeros and null");
    expect(get_word(sees_yaw, 72) == 0x3c3c3c3c, "state word is set after BuildMatrix");
}

int main() {
    for (int defaults = 0; defaults < 2; ++defaults)
        for (int fresh = 0; fresh < 2; ++fresh)
            for (int index = 0; index < 3; ++index) {
                run(defaults, fresh, index, 3, 9);
                run(defaults, fresh, index, 0, 0x1ab);
                run(defaults, fresh, index, -2, 0x77);
            }
    return failures ? 1 : 0;
}

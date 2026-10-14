#include <cmath>
#include <cstdio>
#include <cstring>

extern "C" signed char netNetworkMode = 0;

#include "msg_collision.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

alignas(8) static unsigned char self_object[256];
alignas(8) static unsigned char other_object[1280];
static int added;
static void* added_object;
extern "C" void objectAddToRunList__FP10GameObject(void* o) {
    ++added;
    added_object = o;
}
static unsigned& uword(unsigned char* base, unsigned off) { return *reinterpret_cast<unsigned*>(base + off); }
static float& fword(unsigned char* base, unsigned off) { return *reinterpret_cast<float*>(base + off); }

static void fresh(unsigned other_flags, unsigned self_options, unsigned char other_byte, float ox, float oy, float px, float py) {
    std::memset(self_object, 0x11, sizeof self_object);
    std::memset(other_object, 0x22, sizeof other_object);
    uword(other_object, 12) = other_flags;
    uword(self_object, 104) = self_options;
    other_object[1137] = other_byte;
    fword(other_object, 1196) = ox;
    fword(other_object, 1200) = oy;
    fword(self_object, 108) = px;
    fword(self_object, 112) = py;
    added = 0;
    netNetworkMode = 0;
}
static bool near(float a, double b) { return std::fabs(a - b) <= 1e-3 + 1e-5 * std::fabs(b); }

int main() {
    // moving toward the container: the dot product is well under 52, so the pair is pushed
    fresh(0x40, 0, 0, 3.0f, 4.0f, 10.0f, 20.0f);
    msg_collision__9ContainerP10GameObject(self_object, other_object);
    expect(added == 1 && added_object == self_object, "run list");
    expect(near(fword(self_object, 108), 10.0 + 3.0 * 0.52) && near(fword(self_object, 112), 20.0 + 4.0 * 0.52),
           "pushed by 0.52 times the other pair (dot = 11)");

    // dot product at or above 52: unchanged but still queued
    fresh(0x40, 0, 0, 3.0f, 4.0f, 30.0f, 40.0f);  // dot = 30*0.6 + 40*0.8 = 50 -> update
    msg_collision__9ContainerP10GameObject(self_object, other_object);
    expect(near(fword(self_object, 108), 30.0 + 3.0 * 0.52), "dot 50 is below 52");
    fresh(0x40, 0, 0, 3.0f, 4.0f, 40.0f, 50.0f);  // dot = 24 + 40 = 64
    msg_collision__9ContainerP10GameObject(self_object, other_object);
    expect(added == 1 && fword(self_object, 108) == 40.0f && fword(self_object, 112) == 50.0f, "dot 64: no update, still queued");

    // negative direction
    fresh(0x41, 0xfffffff7u, 0, -3.0f, -4.0f, 1.0f, 1.0f);
    msg_collision__9ContainerP10GameObject(self_object, other_object);
    expect(near(fword(self_object, 108), 1.0 - 3.0 * 0.52) && near(fword(self_object, 112), 1.0 - 4.0 * 0.52), "negative pair");

    // early outs
    fresh(0x40, 0, 0, 3.0f, 4.0f, 10.0f, 20.0f);
    netNetworkMode = 1;
    msg_collision__9ContainerP10GameObject(self_object, other_object);
    expect(added == 0 && fword(self_object, 108) == 10.0f, "network mode set: nothing");
    fresh(0xffffffbfu, 0, 0, 3.0f, 4.0f, 10.0f, 20.0f);
    msg_collision__9ContainerP10GameObject(self_object, other_object);
    expect(added == 0 && fword(self_object, 108) == 10.0f, "bit 6 clear: nothing");
    fresh(0x40, 8, 0, 3.0f, 4.0f, 10.0f, 20.0f);
    msg_collision__9ContainerP10GameObject(self_object, other_object);
    expect(added == 0 && fword(self_object, 108) == 10.0f, "bit 3 set: nothing");
    fresh(0x40, 0, 1, 3.0f, 4.0f, 10.0f, 20.0f);
    msg_collision__9ContainerP10GameObject(self_object, other_object);
    expect(added == 0 && fword(self_object, 108) == 10.0f, "byte 1137 set: nothing");

    // zero pair: infinite scale, NaN dot, no update, still queued
    fresh(0x40, 0, 0, 0.0f, 0.0f, 10.0f, 20.0f);
    msg_collision__9ContainerP10GameObject(self_object, other_object);
    expect(added == 1 && fword(self_object, 108) == 10.0f && fword(self_object, 112) == 20.0f, "zero pair");
    return failures ? 1 : 0;
}

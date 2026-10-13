// Host check only. Not an EE build input.
#include <cmath>
#include <cstdio>
#include <cstring>

signed char netNetworkMode_storage = 0;
extern "C" signed char netNetworkMode = 0;
extern "C" const unsigned norrath_rtti_entities_a_view_481710 = 0x3f051eb8;
static int run_calls;
static void* run_arg;
extern "C" void objectAddToRunList__FP10GameObject(void* object) { ++run_calls; run_arg = object; }
#include "msg_collision.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
alignas(16) static unsigned char self_storage[512];
alignas(16) static unsigned char other_storage[1400];
static void set_f(unsigned char* base, unsigned off, float v) { std::memcpy(base + off, &v, 4); }
static float get_f(const unsigned char* base, unsigned off) { float v; std::memcpy(&v, base + off, 4); return v; }
static void set_w(unsigned char* base, unsigned off, unsigned v) { std::memcpy(base + off, &v, 4); }

static void setup(float ox, float oy, float px, float py) {
    std::memset(self_storage, 0x11, sizeof self_storage);
    std::memset(other_storage, 0x22, sizeof other_storage);
    set_w(other_storage, 12, 0x40);
    set_w(self_storage, 292, 0);
    other_storage[1137] = 0;
    set_f(other_storage, 1196, ox); set_f(other_storage, 1200, oy);
    set_f(self_storage, 296, px); set_f(self_storage, 300, py);
    netNetworkMode = 0;
    run_calls = 0; run_arg = nullptr;
}

int main() {
    const float k = 0.52f;
    // each guard on its own: no run-list call, nothing written
    for (int guard = 0; guard < 5; ++guard) {
        setup(3.0f, 4.0f, 1.0f, 2.0f);
        void* other = other_storage;
        if (guard == 0) netNetworkMode = -1;
        if (guard == 1) other = nullptr;
        if (guard == 2) set_w(other_storage, 12, 0xffffffbfu);
        if (guard == 3) set_w(self_storage, 292, 8);
        if (guard == 4) other_storage[1137] = 1;
        unsigned char before[512];
        std::memcpy(before, self_storage, sizeof before);
        msg_collision__13AutoContainerP10GameObject(self_storage, other);
        expect(run_calls == 0 && std::memcmp(before, self_storage, sizeof before) == 0, "guard stops everything");
    }
    // push: below the threshold
    setup(3.0f, 4.0f, 1.0f, 2.0f);
    msg_collision__13AutoContainerP10GameObject(self_storage, other_storage);
    expect(run_calls == 1 && run_arg == self_storage, "run list gets this object");
    expect(std::fabs(get_f(self_storage, 296) - (1.0f + 3.0f * k)) < 1e-6f && std::fabs(get_f(self_storage, 300) - (2.0f + 4.0f * k)) < 1e-6f, "pair gains other * 0.52");
    // no push at or above the threshold: unit (0.6, 0.8) . (100, 0) = 60
    setup(3.0f, 4.0f, 100.0f, 0.0f);
    msg_collision__13AutoContainerP10GameObject(self_storage, other_storage);
    expect(run_calls == 1 && get_f(self_storage, 296) == 100.0f && get_f(self_storage, 300) == 0.0f, "no push at 60 but still queued");
    // boundary: dot exactly 52 -> not below, so no push
    setup(1.0f, 0.0f, 52.0f, 9.0f);
    msg_collision__13AutoContainerP10GameObject(self_storage, other_storage);
    expect(get_f(self_storage, 296) == 52.0f, "52.0 is not below the threshold");
    setup(1.0f, 0.0f, 51.0f, 9.0f);
    msg_collision__13AutoContainerP10GameObject(self_storage, other_storage);
    expect(get_f(self_storage, 296) == 51.0f + k, "51 is below the threshold");
    // other fields untouched
    expect(self_storage[0] == 0x11 && self_storage[291] == 0x11 && self_storage[304] == 0x11 && other_storage[1200 + 4] == 0x22, "other bytes untouched");
    if (failures == 0) std::printf("msg_collision__13AutoContainer cases passed\n");
    return failures == 0 ? 0 : 1;
}

// Host contract for msg_load__5ChestPc; not an EE input. 32-bit words holding pointers are offsets
// from a host base (W10_ACTOR_D_HOST), so the real field offsets (+36, +28) are used.
#define W10_ACTOR_D_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_d_host_base;
extern "C" { void* chestResourceList[16]; }
#include "msg_load.cpp"
static std::string calls;
static unsigned char markers[16][8];
static unsigned char* objectUnderTest;
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* s, void* h, int f) {
    calls += "add(" + std::to_string(static_cast<unsigned char*>(s) - objectUnderTest) + "," + std::to_string(static_cast<unsigned char*>(h) - w10_actor_d_host_base) + "," + std::to_string(f) + ");";
}
void animSetToEnd__FP14AnimationState(void* s) { calls += "end(" + std::to_string(static_cast<unsigned char*>(s) - objectUnderTest) + ");"; }
void setResource__10GameObjectPQ210GameObject11resourceMap(void* self, void* map) {
    (void)self;
    calls += "res(" + std::to_string((static_cast<unsigned char*>(map) - markers[0]) / 8) + ");";
}
int main() {
    alignas(16) static unsigned char arena[4096], object[200];
    alignas(16) static char buffer[32];
    objectUnderTest = object; w10_actor_d_host_base = arena;
    for (int i = 0; i < 16; ++i) chestResourceList[i] = markers[i];
    int bad = 0;
    struct Case { unsigned w; short half; signed char kind; const char* want; unsigned flagsAfter; } cases[] = {
        {0, 5, 2, "res(2);", 0xffffffff},
        {1, 5, 3, "add(92,1328,516);end(92);res(3);", 0xffffffff},
        {1, 0, 1, "add(92,1328,516);end(92);res(1);", 0xffefffff},
        {1, -4, 0, "add(92,1328,516);end(92);res(0);", 0xffefffff},
        {7, 1, 7, "add(92,1328,516);end(92);res(7);", 0xffffffff},
        {0, -4, 4, "res(4);", 0xffffffff},
    };
    for (const Case& c : cases) {
        std::memset(object, 0, sizeof object);
        unsigned block = 300; std::memcpy(object + 36, &block, 4);
        unsigned header = 1328; std::memcpy(arena + block + 28, &header, 4);
        unsigned flags = 0xffffffff; std::memcpy(object + 12, &flags, 4);
        std::memcpy(buffer, "\x11\x22\x33\x44", 4); std::memcpy(buffer + 4, &c.half, 2);
        buffer[6] = 9; buffer[7] = c.kind; std::memcpy(buffer + 8, &c.w, 4);
        calls.clear();
        int n = msg_load__5ChestPc(object, buffer);
        unsigned w84, w132, f12; short h; std::memcpy(&w84, object + 84, 4); std::memcpy(&w132, object + 132, 4);
        std::memcpy(&h, object + 76, 2); std::memcpy(&f12, object + 12, 4);
        if (n != 12 || w84 != 0x44332211u || h != c.half || object[88] != 9 || object[89] != static_cast<unsigned char>(c.kind) || w132 != c.w || f12 != c.flagsAfter) {
            std::printf("fail: fields (kind %d)\n", c.kind); bad = 1;
        }
        if (calls != c.want) { std::printf("fail: calls %s (kind %d)\n", calls.c_str(), c.kind); bad = 1; }
    }
    return bad;
}

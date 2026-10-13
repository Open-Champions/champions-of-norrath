#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "run.cpp"

int frameNm = 0;

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

struct Event { std::string kind; void* a; void* b; float f0, f1; int i0, i1, i2, i3; };
static std::vector<Event> events;
static void* list_contents[8];
static unsigned list_count;
static Point3 last_low, last_high;
static int last_capacity;
static float difficulty;
alignas(8) static unsigned char others[4][64];

extern "C" void msg_run__8AutoProp(void* object) {
    events.push_back({"base", object, nullptr, 0, 0, 0, 0, 0, 0});
}
extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** out, int capacity) {
    events.push_back({"find", nullptr, nullptr, 0, 0, 0, 0, 0, 0});
    last_low = low;
    last_high = high;
    last_capacity = capacity;
    for (unsigned i = 0; i < list_count; ++i) out[i] = list_contents[i];
    out[list_count] = nullptr;
}
extern "C" float gameDifficultyScale__Fv() { return difficulty; }
extern "C" void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(
    void* target, void* source, float a, float b, int c, int d, int e, int f, void* description, bool flag) {
    events.push_back({"attack", target, source, a, b, c, d, e, f});
    if (description != nullptr || flag) failures += 100;
}
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int mode) {
    events.push_back({"anim", state, header, rate, 0, mode, 0, 0, 0});
}

int main() {
    alignas(8) unsigned char object[512];
    const unsigned flag_values[4] = {0x00, 0x08, 0x20, 0x28};
    for (unsigned mask192 : {0u, 1u, 2u, 0xfffffffeu, 0xffffffffu}) {
        for (int frame : {0, 29, 30, 31, 60, -30, -31, 90}) {
            for (unsigned half158 : {0u, 0x7fu, 0x80u, 0xff80u}) {
                std::memset(object, 0x11, sizeof object);
                std::memcpy(object + 192, &mask192, 4);
                std::memcpy(object + 158, &half158, 2);
                const float px = 10.0f, py = 20.0f, pz = 30.0f, reach = 4.0f;
                std::memcpy(object + 16, &px, 4);
                std::memcpy(object + 20, &py, 4);
                std::memcpy(object + 24, &pz, 4);
                std::memcpy(object + 28, &reach, 4);
                const unsigned header_word = 0x9000;
                std::memcpy(object + 408, &header_word, 4);
                for (int i = 0; i < 4; ++i) {
                    std::memset(others[i], 0, sizeof others[i]);
                    std::memcpy(others[i] + 12, &flag_values[i], 4);
                    list_contents[i] = others[i];
                }
                list_count = 4;
                difficulty = 1.5f;
                frameNm = frame;
                events.clear();
                msg_run__7AntTrap(object);

                std::vector<std::string> want;
                want.push_back("base");
                if (mask192 & 1) {
                    want.push_back("find");
                    want.push_back("attack");  // flags 0x00
                    want.push_back("attack");  // flags 0x20
                }
                const bool second = (frame % 30 == 0) && (half158 & 0x80);
                if (second) {
                    want.push_back("find");
                    want.push_back("anim");  // only flags 0x20
                }
                std::vector<std::string> got;
                for (const Event& e : events) got.push_back(e.kind);
                expect(got == want, "event order");
                unsigned attack_index = 0;
                for (const Event& e : events) {
                    if (e.kind == "find") {
                        expect(last_capacity == 128, "capacity 128");
                        expect(last_low.x == 6.0f && last_low.y == 16.0f && last_low.z == 26.0f, "box low corner");
                        expect(last_high.x == 14.0f && last_high.y == 24.0f && last_high.z == 34.0f, "box high corner");
                    } else if (e.kind == "attack") {
                        expect(e.b == object && e.f0 == 3.0f && e.f1 == 3.0f, "attacker and doubled difficulty");
                        expect(e.i0 == 10000 && e.i1 == 0 && e.i2 == 0 && e.i3 == 8, "integer arguments");
                        expect(e.a == (attack_index == 0 ? others[0] : others[2]), "targets are the objects without flag 8");
                        ++attack_index;
                    } else if (e.kind == "anim") {
                        expect(e.a == object + 152 && e.b == reinterpret_cast<void*>(0x9000) && e.i0 == 6, "transition arguments");
                        expect(e.f0 > 0.0999f && e.f0 < 0.1001f, "rate is the 0.1 word");
                    }
                }
            }
        }
    }
    return failures ? 1 : 0;
}

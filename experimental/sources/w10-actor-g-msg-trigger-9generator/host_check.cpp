// Host contract for msg_trigger__9Generatori; not an EE input. Includes the source.
// The name word is a host-truncated pointer here; it is only compared.
#include "msg_trigger.cpp"
#include <cstdio>
#include <cstring>
#include <string>
static int failures;
static std::string order;
static void* createResult;
static Point3 seenPos; static short seenType; static char** seenArgs; static unsigned seenNameWord;
static int seenTrigValue;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; } }
extern "C" void* objectCreateByName__FPcG6Point3sPPc(char* name, Point3 p, short t, char** a) {
    order += 'C'; seenPos = p; seenType = t; seenArgs = a;
    seenNameWord = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(name));
    return createResult;
}
extern "C" int msg_trigger__8Creaturei(void*, int v) { order += 'T'; seenTrigValue = v; return 4242; }
int main() {
    alignas(16) unsigned char o[1024]; alignas(16) unsigned char made[256];
    auto reset = [&](unsigned flags) {
        std::memset(o, 0, sizeof o); std::memset(made, 0, sizeof made); order.clear();
        float pos[3] = {1.5f, -2.0f, 3.25f}; std::memcpy(o + 16, pos, 12);
        short type = -9; std::memcpy(o + 56, &type, 2);
        std::memcpy(o + 980, &flags, 4);
        createResult = made;
    };
    reset(0);
    expect(msg_trigger__9Generatori(o, 7) == 4242 && order == "T" && seenTrigValue == 7, "flag clear: only the base call");
    reset(2);
    msg_trigger__9Generatori(o, 1); expect(order == "T", "only bit 0 gates the creation");
    reset(1);
    expect(msg_trigger__9Generatori(o, 9) == 4242 && order == "CT" && seenTrigValue == 9, "flag set: create, then base, base result returned");
    expect(seenPos.x == 1.5f && seenPos.y == -2.0f && seenPos.z == 3.25f && seenType == -9 && seenArgs == nullptr, "position copy, type, null arguments");
    unsigned long long bits; std::memcpy(&bits, made + 160, 8);
    expect(bits == 0x01000000ull, "bit 24 set in the created object's 64-bit field");
    reset(0x81);
    std::memset(made + 160, 0xff, 8); createResult = made;
    msg_trigger__9Generatori(o, 1); std::memcpy(&bits, made + 160, 8);
    expect(bits == ~0ull, "other bits untouched");
    reset(1); createResult = nullptr;
    expect(msg_trigger__9Generatori(o, 1) == 4242 && order == "CT", "null creation still calls the base");
    if (!failures) std::puts("generator msg_trigger ok");
    return failures ? 1 : 0;
}

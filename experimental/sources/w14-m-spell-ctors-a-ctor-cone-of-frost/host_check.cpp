// Host contract for __16SpellConeOfFrostiP8CreatureP5Skill; not an EE input. The EE image stores 32-bit
// addresses; the object fields are compared with low() and the pointers passed to the stubs are host pointers.
#include "spell_cone_of_frost_ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_SpellConeOfFrost[8];
unsigned syncRandState;
static std::string calls;
static unsigned char* objectBase;
static unsigned char lightSeen[48];
static Point3 soundPosition;
static void* soundHeader;
static float soundVolume;
static void* lumpResult = reinterpret_cast<void*>(0x7007);
void lightAdd__FP5Light(void* light) {
    calls += "light;";
    std::memcpy(lightSeen, light, 48);
    if (static_cast<unsigned char*>(light) != objectBase + 24) calls += "light-address-wrong;";
}
void* lumpFindResource__FPcT0(char* lump, char* resource) {
    calls += "lump:";
    calls += lump; calls += ":"; calls += resource; calls += ";";
    return lumpResult;
}
int soundPlaySequence__FP15AnimationHeaderG6Point3f(void* header, Point3 position, float volume) {
    calls += "sound;";
    soundHeader = header; soundPosition = position; soundVolume = volume;
    return 0x4242;
}

int main() {
    int bad = 0;
    alignas(16) unsigned char object[128];
    alignas(16) unsigned char creature[32];
    alignas(16) unsigned char skillBuffer[16];
    const unsigned seed = 0x13579bdfu;
    const unsigned next = seed * 0x19660du + 0x3c6ef35fu;
    const Point3 where{1.5f, -2.25f, 3.0f};
    std::memcpy(creature + 16, &where, 12);
    objectBase = object;
    for (int kind : {0x31, -2}) {
        std::memset(object, 0xee, sizeof object);
        calls.clear();
        syncRandState = seed;
        __16SpellConeOfFrostiP8CreatureP5Skill(object, kind, creature, skillBuffer);

        unsigned w4, w8, w12, w16, w20, w84;
        std::memcpy(&w4, object + 4, 4); std::memcpy(&w8, object + 8, 4); std::memcpy(&w12, object + 12, 4);
        std::memcpy(&w16, object + 16, 4); std::memcpy(&w20, object + 20, 4); std::memcpy(&w84, object + 84, 4);
        if (syncRandState != next || w16 != (next >> 16)) { std::puts("fail: random"); bad = 1; }
        if (object[0] != static_cast<unsigned char>(kind) || w4 != 0u) { std::puts("fail: base"); bad = 1; }
        if (w8 != low(skillBuffer) || w12 != low(creature) || w20 != low(vt_SpellConeOfFrost)) { std::puts("fail: fields"); bad = 1; }
        Point3 flash; std::memcpy(&flash, object + 72, 12);
        if (flash.x != where.x + 0.0f || flash.y != where.y || flash.z != where.z + 60.0f) { std::puts("fail: flash point"); bad = 1; }
        // Light record at +24 as written by the constructor, read back in the stub.
        unsigned short h[8]; std::memcpy(h, lightSeen + 10, 16);
        const unsigned short want[8] = {77, 76, 187, 0, 77, 76, 187, 0};
        for (int i = 0; i < 8; ++i) if (h[i] != want[i]) { std::printf("fail: light half %d\n", i); bad = 1; }
        float f28, f32, f36, f40, f44;
        std::memcpy(&f28, lightSeen + 28, 4); std::memcpy(&f32, lightSeen + 32, 4); std::memcpy(&f36, lightSeen + 36, 4);
        std::memcpy(&f40, lightSeen + 40, 4); std::memcpy(&f44, lightSeen + 44, 4);
        unsigned constBits; const unsigned constWant = 0x382c598au;
        std::memcpy(&constBits, &f32, 4);
        if (f28 != 156.0f || f36 != 156.0f || f44 != 0.0f || constBits != constWant || constBits != *reinterpret_cast<unsigned*>(&f40)) {
            std::puts("fail: light floats"); bad = 1;
        }
        if (lightSeen[8] != 0 || lightSeen[9] != 0) { std::puts("fail: light bytes"); bad = 1; }
        if (calls != "light;lump:fx.lmp:spel_ew_cfrost.anm;sound;") { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; }
        if (soundHeader != lumpResult || soundVolume != 1.0f) { std::puts("fail: sound arguments"); bad = 1; }
        if (soundPosition.x != where.x || soundPosition.y != where.y || soundPosition.z != where.z) { std::puts("fail: sound position"); bad = 1; }
        if (w84 != 0x4242u) { std::puts("fail: handle"); bad = 1; }
    }
    if (bad == 0) std::puts("ok");
    return bad;
}

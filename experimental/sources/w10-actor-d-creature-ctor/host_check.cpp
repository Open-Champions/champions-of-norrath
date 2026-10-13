// Host contract for the Creature constructor; not an EE input. The tag lookups are scripted and
// every call is logged together with a checksum of the object, so a store moved across a call fails.
#include "creature_ctor.cpp"
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
unsigned char vt_Creature[8];
unsigned char gameCreatureThinkSpinner;
signed char cutMode;
static std::string calls;
static std::map<std::string, long> tags;
static unsigned char* objectUnderTest;
static unsigned sum() { unsigned s = 0; for (int i = 0; i < 1100; ++i) s = s * 31 + objectUnderTest[i]; return s; }
void gameObject_construct(void*, const Point3* p, void* map, char** a) {
    calls += "go(" + std::to_string(p->x) + "," + std::to_string(map != nullptr) + "," + std::to_string(a != nullptr) + ");";
}
char* findTagString__10GameObjectPci(void*, const char* n, int i) {
    calls += std::string("fs:") + n + ":" + std::to_string(i) + ";";
    auto it = tags.find(n); return it == tags.end() || it->second == 0 ? nullptr : reinterpret_cast<char*>(static_cast<__UINTPTR_TYPE__>(it->second));
}
int findTagInt__10GameObjectPci(void*, const char* n, int i) {
    calls += std::string("fi:") + n + ":" + std::to_string(i) + ";";
    auto it = tags.find(n); return it == tags.end() ? 0 : static_cast<int>(it->second);
}
void scriptStart__FPc(char*) { calls += "script;"; }
void objectRemoveFromGrid__FP10GameObject(void*) { calls += "grid;" ; calls += "b570=" + std::to_string(objectUnderTest[570]) + ";"; }
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::printf("fail: %s\n", what); ++failures; } }
template <typename T> static T get(unsigned off) { T v; std::memcpy(&v, objectUnderTest + off, sizeof v); return v; }
int main() {
    alignas(16) static unsigned char object[1100];
    objectUnderTest = object;
    Point3 pt = {1.5f, 2.5f, 3.5f}; int map = 0; char* args = nullptr;
    auto run = [&](signed char cut, unsigned char spinner) {
        for (int i = 0; i < 1100; ++i) object[i] = static_cast<unsigned char>(i * 13 + 5);
        float z = 77.0f; std::memcpy(object + 24, &z, 4);
        cutMode = cut; gameCreatureThinkSpinner = spinner; calls.clear();
        creature_construct(object, &pt, static_cast<short>(0xfffe), &map, 3.25f, &args);
    };
    tags.clear();
    run(0, 200);
    expect(calls == "go(1.500000,1,1);fs:initScript:0;fi:wait:0;fi:hide:0;fs:ShowName:0;fi:Dead:0;fi:HomeRange:0;fi:Scale:0;fi:MiniBoss:0;fs:light1:0;fs:light2:0;fs:ambient:0;", "empty tag sequence");
    expect(object[293] == 8 && get<float>(760) == 0 && get<float>(764) == 0 && get<float>(768) == 0, "fixed fields");
    expect(get<unsigned>(72) == low32(vt_Creature), "table pointer");
    expect(get<float>(776) == 1.5f && get<float>(780) == 2.5f && get<float>(784) == 3.5f, "point copy");
    expect(get<float>(224) == 1.0f && get<float>(228) == 0 && get<float>(192) == 0, "floats");
    expect(get<short>(56) == -2 && get<unsigned short>(798) == 0xfffe, "type");
    expect(get<float>(564) == 3.25f && get<float>(560) == 3.25f, "scale");
    expect(get<int>(552) == -1 && get<int>(556) == -1 && get<int>(580) == -1 && get<int>(584) == -1, "minus ones");
    expect(object[85] == 0xff && object[76] == 0 && object[156] == 0 && object[800] == 0 && object[570] == 0, "bytes");
    expect(get<float>(60) == 77.0f && get<float>(788) == 1000.0f, "copied and default floats");
    expect(object[569] == 200 && gameCreatureThinkSpinner == 201, "spinner");
    expect((get<unsigned>(12) & 0x20) != 0, "flag 0x20");
    run(0, 255); expect(gameCreatureThinkSpinner == 0 && object[569] == 255, "spinner wraps");
    run(1, 0); expect(calls.find("initScript") == std::string::npos, "cutMode skips initScript");
    run(-1, 0); expect(calls.find("initScript") == std::string::npos, "negative cutMode skips initScript");
    tags["initScript"] = 0x1000; tags["wait"] = 5; tags["hide"] = 1; tags["ShowName"] = 0x2000; tags["Dead"] = 1; tags["HomeRange"] = 40; tags["Scale"] = 250; tags["MiniBoss"] = 1; tags["light2"] = 7;
    run(0, 0);
    expect(calls == "go(1.500000,1,1);fs:initScript:0;script;fi:wait:0;fi:wait:0;fi:hide:0;grid;b570=3;fs:ShowName:0;fs:ShowName:0;fi:Dead:0;fi:HomeRange:0;fi:HomeRange:0;fi:Scale:0;fi:Scale:0;fi:MiniBoss:0;fs:light1:0;fs:light2:0;",
           "full tag sequence");
    expect(object[570] == 3, "hide wins over wait");
    expect(get<unsigned>(892) == 0x2000 && (get<unsigned>(12) & 0x100000) != 0, "ShowName");
    expect(get<float>(192) == 1000.0f && get<float>(788) == 40.0f && get<float>(224) == 250 * 0.01f, "Dead, HomeRange, Scale");
    expect((get<unsigned long long>(160) & 1) != 0 && (get<unsigned long long>(160) & 0x40000000) != 0, "MiniBoss and light bits");
    tags.clear(); tags["wait"] = 0x1ff; tags["ambient"] = 3; run(0, 0);
    expect(object[570] == 0xff, "wait stored as a byte");
    expect(calls.find("fs:light1:0;fs:light2:0;fs:ambient:0;") != std::string::npos && (get<unsigned long long>(160) & 0x40000000) != 0, "ambient alone sets the light bit");
    tags.clear(); tags["light1"] = 9; run(0, 0);
    expect(calls.find("light2") == std::string::npos && (get<unsigned long long>(160) & 0x40000000) != 0, "light1 short-circuits");
    return failures ? 1 : 0;
}

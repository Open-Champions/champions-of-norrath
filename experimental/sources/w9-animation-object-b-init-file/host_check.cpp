// Host contract for initFile__FPcPvi. Not an EE plan input.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" int initFile__FPcPvi(char*, void*, int);
int noNewVags;
static std::vector<std::string> calls;
static void* last_data;
static int sound_flag;
static int world_result;
extern "C" {
void animInitAnimation__FP15AnimationHeader(void* d) { calls.push_back("anm"); last_data = d; }
void modelInitModel__FPv(void* d) { calls.push_back("vif"); last_data = d; }
void textureInitTexture__FP8_texture(void* d) { calls.push_back("tex"); last_data = d; }
void fontInitFont__FP4Font(void* d) { calls.push_back("fnt"); last_data = d; }
int worldInitWorld__FP12_worldHeader(void* d) { calls.push_back("world"); last_data = d; return world_result; }
void soundAddSound__FPcP10_vagHeaderb(char* n, void* d, int flag) { calls.push_back(std::string("vag:") + n); last_data = d; sound_flag = flag; }
void textAddText__FPv(void* d) { calls.push_back("uni"); last_data = d; }
}

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    int data;
    struct Case { const char* name; const char* call; int result; } cases[] = {
        {"walk.anm", "anm", 0}, {"orc.vif", "vif", 0}, {"face.tex", "tex", 0}, {"big.fnt", "fnt", 0}, {"text.uni", "uni", 0},
    };
    for (const Case& c : cases) {
        calls.clear(); last_data = nullptr;
        char name[32]; std::strcpy(name, c.name);
        int r = initFile__FPcPvi(name, &data, 500);
        expect(r == c.result && calls.size() == 1 && calls[0] == c.call && last_data == &data, c.name);
    }
    // The last '.' selects the extension.
    calls.clear();
    char dotted[] = "a.b.anm";
    initFile__FPcPvi(dotted, &data, 1);
    expect(calls.size() == 1 && calls[0] == "anm", "extension is after the last dot");
    // Case matters.
    calls.clear();
    char upper[] = "walk.ANM";
    expect(initFile__FPcPvi(upper, &data, 9) == 0 && calls.empty(), "extension compare is case sensitive");
    // World: size minus the initializer's answer, zero when it answers zero.
    calls.clear(); world_result = 100;
    char world[] = "level.world";
    expect(initFile__FPcPvi(world, &data, 700) == 600 && calls.size() == 1 && calls[0] == "world", "world returns size minus answer");
    world_result = 0;
    expect(initFile__FPcPvi(world, &data, 700) == 0, "world with a zero answer returns 0");
    world_result = 800;
    expect(initFile__FPcPvi(world, &data, 700) == -100, "the difference is signed");
    // Sound: name and the raw word are passed, result is size - 64.
    calls.clear(); noNewVags = 0x1234;
    char vag[] = "bang.vag";
    expect(initFile__FPcPvi(vag, &data, 1000) == 936 && calls.size() == 1 && calls[0] == "vag:bang.vag" && sound_flag == 0x1234, "vag passes name and the whole flag word, returns size - 64");
    // etex and unknown: nothing is called.
    calls.clear();
    char etex[] = "x.etex";
    expect(initFile__FPcPvi(etex, &data, 5) == 0 && calls.empty(), "etex calls nothing");
    char unknown[] = "x.bin";
    expect(initFile__FPcPvi(unknown, &data, 5) == 0 && calls.empty(), "unknown extension");
    char nodot[] = "noextension";
    expect(initFile__FPcPvi(nodot, &data, 5) == 0 && calls.empty(), "no dot");
    char empty_ext[] = "x.";
    expect(initFile__FPcPvi(empty_ext, &data, 5) == 0 && calls.empty(), "empty extension");
    if (failures) return 1;
    std::printf("initFile contract passed\n");
    return 0;
}

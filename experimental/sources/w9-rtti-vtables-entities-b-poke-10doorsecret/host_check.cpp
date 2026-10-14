#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "msg_poke.cpp"

unsigned frameNm = 0;

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

static std::vector<std::string> calls;
static bool has_key;
static int script_result;
static unsigned char* current_object;
static unsigned flags_at_run_list;
static unsigned short entry[2] = {0x41, 0};
static char key_text[] = "script_name";
extern "C" char* findTagString__10GameObjectPci(void* object, const char* tag, int index) {
    calls.push_back(std::string("find:") + tag + ":" + std::to_string(index));
    (void)object;
    return has_key ? key_text : nullptr;
}
extern "C" int scriptStart__FPc(char* script) {
    calls.push_back(std::string("script:") + script);
    return script_result;
}
extern "C" void playerPlaySound__Fii(int index, int sound) {
    calls.push_back("sound:" + std::to_string(index) + ":" + std::to_string(sound));
}
extern "C" unsigned short* textGetString__FPc(const char* key) {
    calls.push_back(std::string("text:") + key);
    return entry;
}
extern "C" void hudSetInfoMessage__FPUsiic(unsigned short* text, int duration, int player, char extra) {
    calls.push_back("hud:" + std::to_string(text == entry) + ":" + std::to_string(duration) + ":" + std::to_string(player) + ":" + std::to_string(extra));
}
extern "C" void objectAddToRunList__FP10GameObject(void* object) {
    calls.push_back(object == current_object ? "runlist" : "runlist-wrong-object");
    std::memcpy(&flags_at_run_list, current_object + 12, 4);
}
extern "C" char* lumpFindResource__FPcT0(const char* lump, const char* name) {
    calls.push_back(std::string("lump:") + lump + ":" + name);
    return key_text;
}
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float rate) {
    calls.push_back(std::string("seq:") + std::to_string(header == key_text) + ":" + std::to_string(object == current_object) + ":" + std::to_string(rate));
    return 0;
}

static unsigned word(const unsigned char* base, unsigned off) {
    unsigned v;
    std::memcpy(&v, base + off, 4);
    return v;
}

int main() {
    alignas(8) unsigned char object[128];
    struct Case { bool key; int script; };
    for (Case c : {Case{false, 0}, Case{true, 0}, Case{true, 1}, Case{true, -1}}) {
        std::memset(object, 0x22, sizeof object);
        unsigned all = 0xffffffffu;
        std::memcpy(object + 12, &all, 4);
        current_object = object;
        has_key = c.key;
        script_result = c.script;
        frameNm = 4321;
        calls.clear();
        msg_poke__10DoorSecreti(object, 3);
        if (c.key && c.script == 0) {
            const std::vector<std::string> want = {"find:key:0", "find:key:0", "script:script_name", "sound:3:7", "text:DoorLocked", "hud:1:180:-1:0"};
            expect(calls == want, "locked: message and sound, nothing else");
            expect(word(object, 12) == all && object[88] == 0x22 && word(object, 84) == 0x22222222u, "locked: object untouched");
        } else {
            std::vector<std::string> want;
            want.push_back("find:key:0");
            if (c.key) {
                want.push_back("find:key:0");
                want.push_back("script:script_name");
            }
            want.push_back("runlist");
            want.push_back("lump:fx.lmp:door_stn_open.anm");
            want.push_back("seq:1:1:1.000000");
            expect(calls == want, "open: call order");
            expect(object[88] == 1 && word(object, 12) == 0xffefffffu && word(object, 84) == 4321, "open: fields");
            expect(flags_at_run_list == 0xffefffffu, "flag cleared before the run-list call");
            expect(object[83] == 0x22 && object[89] == 0x22, "neighbours untouched");
        }
    }
    return failures ? 1 : 0;
}

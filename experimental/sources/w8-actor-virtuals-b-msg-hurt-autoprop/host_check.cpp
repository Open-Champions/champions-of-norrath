#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#define NORRATH_W8AVB_HOST
extern "C" unsigned long long norrath_w8avb_host_base = 0;
static void advance_stub(void* self);
extern "C" void* norrath_w8avb_host_function(unsigned word) {
    return word == 7 ? reinterpret_cast<void*>(&advance_stub) : nullptr;
}
#include "msg_hurt.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

alignas(8) static unsigned char memory[1024];
static std::vector<std::string> log_calls;
static int sequence_token[4];
static void advance_stub(void* self) {
    log_calls.push_back("advance");
    if (self != memory + 24) log_calls.push_back("bad-self");
}
extern "C" int strcasecmp(const char* left, const char* right) {
    log_calls.push_back(std::string("cmp:") + left + ":" + right);
    for (;; ++left, ++right) {
        int a = *left, b = *right;
        if (a >= 'A' && a <= 'Z') a += 32;
        if (b >= 'A' && b <= 'Z') b += 32;
        if (a != b) return a < b ? -1 : 1;
        if (a == 0) return 0;
    }
}
extern "C" void* lumpFindResource__FPcT0(const char* lump, const char* name) {
    log_calls.push_back(std::string("lump:") + lump + ":" + name);
    return &sequence_token[log_calls.size() % 4];
}
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume) {
    (void)sequence;
    log_calls.push_back(std::string("sound:") + (object == memory ? "self" : "other") + ":" + (volume == 1.0f ? "1" : "?"));
}

static void setup(unsigned flags, unsigned state, const char* name) {
    std::memset(memory, 0, sizeof memory);
    norrath_w8avb_host_base = reinterpret_cast<unsigned long long>(memory);
    unsigned table = 512;
    std::memcpy(memory + 72, &table, 4);
    short adjust = 24;
    std::memcpy(memory + 512 + 136, &adjust, 2);
    unsigned function = 7;
    std::memcpy(memory + 512 + 140, &function, 4);
    std::memcpy(memory + 284, &flags, 4);
    std::memcpy(memory + 288, &state, 4);
    std::strcpy(reinterpret_cast<char*>(memory + 76), name);
    log_calls.clear();
}

int main() {
    setup(4, 1, "GothTrap");
    msg_hurt__8AutoPropfP10GameObjecti(memory, 3.0f, nullptr, 0x800);
    expect(log_calls == std::vector<std::string>({"advance", "cmp:GothTrap:gothtrap", "lump:gothtrap.lmp:cs_wallcrumble.anm",
                                                   "sound:self:1"}),
           "matching name: advance, compare, one sound");

    setup(8, 1, "Lever");
    msg_hurt__8AutoPropfP10GameObjecti(memory, 0.0f, nullptr, 0);
    expect(log_calls == std::vector<std::string>({"advance", "cmp:Lever:gothtrap", "lump:fx.lmp:spel_he_repulse.anm", "sound:self:1",
                                                   "lump:fx.lmp:hit_chnmail_bludg_hvy.anm", "sound:self:1"}),
           "other name: two sounds in order");

    setup(0xc, 2, "gothtrapx");
    msg_hurt__8AutoPropfP10GameObjecti(memory, 0.0f, nullptr, 0);
    expect(log_calls.size() == 6u && log_calls[2] == "lump:fx.lmp:spel_he_repulse.anm", "a longer name does not match");

    setup(4, 0, "gothtrap");
    msg_hurt__8AutoPropfP10GameObjecti(memory, 0.0f, nullptr, 0);
    expect(log_calls.empty(), "zero state: nothing");
    setup(0xfffffff3u, 5, "gothtrap");
    msg_hurt__8AutoPropfP10GameObjecti(memory, 0.0f, nullptr, 0);
    expect(log_calls.empty(), "bits 2 and 3 clear: nothing");
    setup(1, 5, "gothtrap");
    msg_hurt__8AutoPropfP10GameObjecti(memory, 0.0f, nullptr, 0);
    expect(log_calls.empty(), "bit 0 alone: nothing");
    return failures ? 1 : 0;
}

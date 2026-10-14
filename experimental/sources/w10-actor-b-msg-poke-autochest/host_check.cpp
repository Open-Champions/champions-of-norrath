// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST
#include <cstdio>
#include <cstring>
#include <string>

extern "C" int frameNm;
int frameNm;
static std::string calls;
static unsigned char* current;
struct Seen {
    std::string lump, name;
    float volume = 0;
    void* object = nullptr;
    void* state = nullptr;
    void* header = nullptr;
    int anim_flags = 0, gold_player = 0, gold_amount = 0;
    unsigned gold_point[3] = {};
    unsigned opened_at_anim = 0, copy_at_runlist = 0, flags_at_runlist = 0;
} seen;
static int sound_token;
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) { calls += "lump;"; seen.lump = lump; seen.name = name; return &sound_token; }
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sound, void* object, float volume) {
    calls += sound == &sound_token ? "sound;" : "sound?;"; seen.object = object; seen.volume = volume;
}
extern "C" void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flags) {
    calls += "anim;"; seen.state = state; seen.header = header; seen.anim_flags = flags;
    std::memcpy(&seen.opened_at_anim, current + 216, 4);
}
extern "C" void objectAddToRunList__FP10GameObject(void* object) {
    calls += object == current ? "run;" : "run?;";
    std::memcpy(&seen.copy_at_runlist, current + 224, 4);
    std::memcpy(&seen.flags_at_runlist, current + 12, 4);
}
#include "msg_poke.cpp"
extern "C" void hudGivePlayerGold__FiG6Point3i(int player, const GoldPoint* point, int amount) {
    calls += "gold;"; seen.gold_player = player; seen.gold_amount = amount;
    seen.gold_point[0] = point->x; seen.gold_point[1] = point->y; seen.gold_point[2] = point->z;
}

static unsigned word(const unsigned char* b, unsigned off) { unsigned v; std::memcpy(&v, b + off, 4); return v; }
static void put(unsigned char* b, unsigned off, unsigned v) { std::memcpy(b + off, &v, 4); }
int main() {
    int failures = 0;
    alignas(8) unsigned char object[512];
    alignas(8) unsigned char header[16];
    const int kinds[] = {0, 1, 2, 3, -1, 0x7f, -128};
    const int amounts[] = {-5, 0, 1, 120};
    const int divided[] = {0, 7, -7, 300, -300, 2, 0x7fffffff, -0x7fffffff - 1};
    const int frames[] = {0, 100, 123456};
    for (int kind : kinds) for (int opened : {0, 1}) for (int has_header : {0, 1}) for (int amount : amounts) for (int d : divided) for (int frame : frames) {
        std::memset(object, 0x6e, sizeof object);
        std::memset(header, 0, sizeof header);
        put(header, 4, static_cast<unsigned>(d));
        rtti_c::host_note(header);
        object[240] = static_cast<unsigned char>(kind);
        put(object, 216, static_cast<unsigned>(opened));
        put(object, 244, has_header ? static_cast<unsigned>(reinterpret_cast<unsigned long long>(header)) : 0u);
        const unsigned short amount16 = static_cast<unsigned short>(amount);
        std::memcpy(object + 220, &amount16, 2);
        put(object, 12, 0xffffffffu);
        put(object, 16, 0x11111111u); put(object, 20, 0x22222222u); put(object, 24, 0x33333333u);
        frameNm = frame;
        current = object; calls.clear(); seen = Seen();
        msg_poke__9AutoChesti(object, 4);
        const int want232 = has_header ? frame + d / 3 : frame;
        if (static_cast<int>(word(object, 232)) != want232) { std::fprintf(stderr, "fail: +232\n"); ++failures; }
        if (!opened) {
            std::string want;
            const char* name = kind == 1 ? "door_mtlchst_open.anm" : kind == 0 ? "door_wdchst_open.anm" : kind == 2 ? "door_stnchst_open.anm" : nullptr;
            if (name) want += "lump;sound;";
            want += "anim;run;";
            if (calls != want) { std::fprintf(stderr, "fail: calls %s want %s\n", calls.c_str(), want.c_str()); ++failures; continue; }
            if (name && (seen.lump != "fx.lmp" || seen.name != name || seen.volume != 1.0f || seen.object != object)) { std::fprintf(stderr, "fail: sound\n"); ++failures; }
            if (seen.state != object + 152 || seen.anim_flags != 516 || (has_header ? seen.header != header : seen.header != nullptr)) { std::fprintf(stderr, "fail: animAdd arguments\n"); ++failures; }
            if (seen.opened_at_anim != 0 || word(object, 216) != 1) { std::fprintf(stderr, "fail: opened latch\n"); ++failures; }
            if (static_cast<int>(word(object, 224)) != want232) { std::fprintf(stderr, "fail: +224\n"); ++failures; }
            const unsigned want12 = amount <= 0 ? 0xffefffffu : 0xffffffffu;
            if (word(object, 12) != want12) { std::fprintf(stderr, "fail: flag word\n"); ++failures; }
            if (word(object, 200) != 0x6e6e6e6eu) { std::fprintf(stderr, "fail: +200 written\n"); ++failures; }
        } else {
            if (calls != "lump;sound;gold;") { std::fprintf(stderr, "fail: calls %s\n", calls.c_str()); ++failures; continue; }
            if (seen.lump != "fx.lmp" || seen.name != "game_pu_coin.anm" || seen.volume != 1.0f || seen.object != object) { std::fprintf(stderr, "fail: coin sound\n"); ++failures; }
            if (seen.gold_player != 4 || seen.gold_amount != amount || seen.gold_point[0] != 0x11111111u || seen.gold_point[1] != 0x22222222u || seen.gold_point[2] != 0x33333333u) {
                std::fprintf(stderr, "fail: gold arguments\n"); ++failures;
            }
            unsigned long long quad;
            std::memcpy(&quad, object + 200, 8);
            unsigned short left;
            std::memcpy(&left, object + 220, 2);
            if (quad != 1ull || left != 0) { std::fprintf(stderr, "fail: +200/+220\n"); ++failures; }
            if (word(object, 12) != 0xffffffffu || word(object, 216) != 1u || word(object, 224) != 0x6e6e6e6eu) { std::fprintf(stderr, "fail: untouched words\n"); ++failures; }
        }
    }
    return failures ? 1 : 0;
}

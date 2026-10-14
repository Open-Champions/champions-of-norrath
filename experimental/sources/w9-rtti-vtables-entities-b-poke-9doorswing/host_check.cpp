#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#define NORRATH_W8AVB_HOST
extern "C" unsigned long long norrath_w8avb_host_base = 0;
extern "C" void* norrath_w8avb_host_function(unsigned) { return nullptr; }

#include "msg_poke.cpp"

int world = 0;
unsigned char player[4 * 23984];

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

alignas(16) static unsigned char memory[0x4000];
static const unsigned WORLD = 0x1000, DOORS = 0x1400, TABLE = 0x1800, PLAYERS = 0x2000, SELF = 0;
static std::vector<std::string> log_;
static const char* key_tag;
static const char* sound_tag;
static int script_result;
static float coll_float;
static char marker;
static unsigned short entry_text[2] = {1, 0};

static void put(unsigned off, unsigned v) { std::memcpy(memory + off, &v, 4); }
static unsigned get(unsigned off) { unsigned v; std::memcpy(&v, memory + off, 4); return v; }
static void put_float(unsigned off, float f) { std::memcpy(memory + off, &f, 4); }

extern "C" char* findTagString__10GameObjectPci(void*, const char* tag, int index) {
    log_.push_back(std::string("tag:") + tag + ":" + std::to_string(index));
    return const_cast<char*>(std::strcmp(tag, "key") == 0 ? key_tag : sound_tag);
}
extern "C" int scriptStart__FPc(char* script) {
    log_.push_back(std::string("script:") + script);
    return script_result;
}
extern "C" int strcmp(const char* a, const char* b) {
    int i = 0;
    while (a[i] && a[i] == b[i]) ++i;
    return static_cast<unsigned char>(a[i]) - static_cast<unsigned char>(b[i]);
}
extern "C" char* lumpFindResource__FPcT0(const char* lump, const char* name) {
    log_.push_back(std::string("lump:") + lump + ":" + name);
    return &marker;
}
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float rate) {
    log_.push_back(header == &marker && object == memory + SELF && rate == 1.0f ? "seq" : "seq-wrong");
    return 0;
}
extern "C" void playerPlaySound__Fii(int index, int sound) { log_.push_back("psound:" + std::to_string(index) + ":" + std::to_string(sound)); }
extern "C" unsigned short* textGetString__FPc(const char* key) {
    log_.push_back(std::string("text:") + key);
    return entry_text;
}
extern "C" void hudSetInfoMessage__FPUsiic(unsigned short* text, int duration, int player_index, char extra) {
    log_.push_back("hud:" + std::to_string(text == entry_text) + ":" + std::to_string(duration) + ":" + std::to_string(player_index) + ":" + std::to_string(extra));
}
extern "C" void objectAddToRunList__FP10GameObject(void*) { log_.push_back("run:" + std::to_string(get(12))); }
extern "C" void worldSetDoorCollision__FP12_worldHeaderii(void* w, int index, int enable) {
    log_.push_back("collision:" + std::to_string(index) + ":" + std::to_string(enable) + (w == reinterpret_cast<void*>(WORLD) ? "" : ":badworld"));
    std::memcpy(&coll_float, memory + 84, 4);
}

struct Door { int record, x, y, z; short c, s; };

static void setup(int door_index, unsigned flags, Door door, unsigned record_flags, float px, float py) {
    std::memset(memory, 0, sizeof memory);
    std::memset(player, 0, sizeof player);
    norrath_w8avb_host_base = reinterpret_cast<unsigned long long>(memory);
    world = WORLD;
    put(WORLD + 84, DOORS);
    put(WORLD + 36, TABLE);
    short idx = static_cast<short>(door_index);
    std::memcpy(memory + 76, &idx, 2);
    put(12, flags);
    if (door_index >= 0) {
        unsigned char* e = memory + DOORS + 28 * door_index;
        std::memcpy(e, &door.record, 4);
        std::memcpy(e + 8, &door.x, 4);
        std::memcpy(e + 12, &door.y, 4);
        std::memcpy(e + 16, &door.z, 4);
        std::memcpy(e + 20, &door.c, 2);
        std::memcpy(e + 22, &door.s, 2);
        put(TABLE + 60 * door.record + 36, record_flags);
    }
    for (int p = 0; p < 4; ++p) {
        unsigned obj = PLAYERS + 0x100 * p;
        std::memcpy(player + 23984 * p + 21872, &obj, 4);
        put_float(obj + 16, px + static_cast<float>(p));
        put_float(obj + 20, py);
    }
    log_.clear();
}

static bool has(const std::string& entry) {
    for (const std::string& e : log_) if (e == entry) return true;
    return false;
}

int main() {
    const Door door = {1, 40, 80, 8, 16384, 16384};
    // Locked: key present and the script says no.
    key_tag = "thekey";
    script_result = 0;
    sound_tag = nullptr;
    setup(2, 0xffffffffu, door, 0, 0, 0);
    put(DOORS, 0);
    log_.clear();
    msg_poke__9DoorSwingi(memory, 3);
    {
        const std::vector<std::string> want = {"tag:key:0", "tag:key:0", "script:thekey", "tag:sound:0", "lump:fx.lmp:door_wd_locked.anm", "seq",
                                               "psound:3:7", "text:DoorLocked", "hud:1:180:-1:0"};
        expect(log_ == want, "locked, no sound tag");
        expect(get(12) == 0xffffffffu && get(84) == 0 && memory[88] == 0, "locked: object untouched");
    }
    sound_tag = "jail";
    log_.clear();
    msg_poke__9DoorSwingi(memory, 1);
    expect(has("lump:fx.lmp:door_jail_locked.anm") && has("psound:1:7") && !has("run:4293918719"), "locked, jail sound");
    sound_tag = "stone";
    log_.clear();
    msg_poke__9DoorSwingi(memory, 1);
    expect(!has("seq") && has("psound:1:7") && has("hud:1:180:-1:0"), "locked, other sound tag: no sequence");

    // -10 skips the key test entirely.
    setup(-1, 0xffffffffu, door, 0, 0, 0);
    log_.clear();
    msg_poke__9DoorSwingi(memory, -10);
    {
        const std::vector<std::string> want = {"run:4293918719"};
        expect(log_ == want, "-10 with a negative door: flag cleared, run list, nothing else");
        expect(get(12) == 0xffefffffu, "bit 20 cleared, bit 0 not set");
    }

    // Script returns nonzero: opens. Compute a pivot and the facing byte.
    script_result = 7;
    sound_tag = nullptr;
    for (unsigned record_flags : {0u, 0x40u}) {
        for (int who : {-10, 0, 2}) {
            setup(2, 0xffefffffu, door, record_flags, 100.0f, 100.0f);
            put(DOORS, 0);  // record 0 for this door
            put(TABLE + 36, record_flags);
            msg_poke__9DoorSwingi(memory, who);
            const float sine = 16384.0f * single_from_bits(0x38000100u), cosine = sine;
            const float x = 10.0f, y = 20.0f;
            float px, py;
            if (record_flags & 0x40) { py = x * sine - y * cosine; px = x * cosine + y * sine; }
            else { py = x * sine + y * cosine; px = x * cosine - y * sine; }
            const int slot = who == -10 ? 0 : who;
            const float dx = 100.0f + static_cast<float>(slot) - px, dy = 100.0f - py;
            unsigned char want88 = dx * cosine + dy * sine < 0.0f ? 1 : 0xff;
            if (record_flags & 0x40) want88 = static_cast<unsigned char>(-static_cast<int>(want88));
            expect(memory[88] == want88, "facing byte");
            expect(get(12) == (0xffefffffu | 1u), "flag bit 20 stays clear and bit 0 is set");
            float f84;
            std::memcpy(&f84, memory + 84, 4);
            expect(f84 == 600.0f && coll_float == 600.0f, "float 600.0 stored before the collision call");
            std::vector<std::string> want;
            if (who != -10) want = {"tag:key:0", "tag:key:0", "script:thekey"};
            want.insert(want.end(), {"run:" + std::to_string(0xffefffffu), "tag:sound:0", "lump:fx.lmp:door_wd_open.anm", "seq", "collision:2:0"});
            expect(log_ == want, "open sequence of calls");
        }
    }
    // Sound tags when opening.
    for (const char* tag : {"jail", "stone"}) {
        sound_tag = tag;
        setup(0, 0, door, 0, 0, 0);
        put(DOORS, 0);
        msg_poke__9DoorSwingi(memory, 0);
        expect(has("lump:fx.lmp:door_jail_open.anm") == (std::strcmp(tag, "jail") == 0), "open with sound tag");
        expect(!has("lump:fx.lmp:door_wd_open.anm"), "no wood sound with a tag");
    }
    return failures ? 1 : 0;
}

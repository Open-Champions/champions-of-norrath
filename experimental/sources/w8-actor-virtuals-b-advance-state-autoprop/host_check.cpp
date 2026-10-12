#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#define NORRATH_W8AVB_HOST
extern "C" unsigned long long norrath_w8avb_host_base = 0;
static void passable_stub(void* self, int flag);
extern "C" void* norrath_w8avb_host_function(unsigned word) {
    return word == 31 ? reinterpret_cast<void*>(&passable_stub) : nullptr;
}
extern "C" void* currentRunningObject = nullptr;
#include "advance_state.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

alignas(16) static unsigned char arena[16384];
static const unsigned kTable = 4096;
static const unsigned kProps = 8192;  // PhProp stand-ins, 640 bytes each

static std::vector<std::string> log_calls;
static std::vector<unsigned long long> chunk_queue;
static unsigned chunk_next;
static std::vector<int> chunk_mask_args;
static std::vector<unsigned> chunk_model_args;
static std::vector<std::string> tags;  // scripts for tag "action<state>"
static bool alloc_fail;
static unsigned alloc_next;
static void* script_owner_seen[8];
static int script_count;


static void passable_stub(void* self, int flag) {
    log_calls.push_back("passable:" + std::to_string(flag) + (self == arena + 8 ? ":adj" : ":?"));
}
extern "C" unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int mask) {
    chunk_model_args.push_back(static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(model)));
    chunk_mask_args.push_back(mask);
    log_calls.push_back("chunks");
    return chunk_next < chunk_queue.size() ? chunk_queue[chunk_next++] : 0;
}
extern "C" int strcasecmp(const char* left, const char* right) {
    log_calls.push_back(std::string("cmp:") + right);
    for (;; ++left, ++right) {
        int a = *left, b = *right;
        if (a >= 'A' && a <= 'Z') a += 32;
        if (b >= 'A' && b <= 'Z') b += 32;
        if (a != b) return a < b ? -1 : 1;
        if (a == 0) return 0;
    }
}
extern "C" unsigned char* norrath_block_alloc(int size) {
    log_calls.push_back("alloc:" + std::to_string(size));
    if (alloc_fail) return nullptr;
    unsigned char* p = arena + kProps + 640 * alloc_next++;
    float pos[3] = {1.0f * alloc_next, 2.0f, 3.0f};
    std::memcpy(p + 416, pos, 12);
    return p;
}
extern "C" void phprop_construct(void* self, const void* position, const char* a, const char* b, const char* c,
                                 unsigned long long mask, int flags, void* description, float scale)
    asm("__6PhPropG6Point3PcN22liPQ24item11descriptionf");
extern "C" void phprop_construct(void* self, const void* position, const char* a, const char* b, const char* c,
                                 unsigned long long mask, int flags, void* description, float scale) {
    float p[3];
    std::memcpy(p, position, 12);
    char buf[200];
    std::snprintf(buf, sizeof buf, "phprop:%d:pos%g,%g,%g:%s:%s:%s:mask%llx:flags%x:%s:%g",
                  static_cast<int>((static_cast<unsigned char*>(self) - (arena + kProps)) / 640), p[0], p[1], p[2], a, b, c, mask, flags,
                  description == nullptr ? "nodesc" : "desc", scale);
    log_calls.push_back(buf);
}
extern "C" void addEffect__FiG6Point3(int kind, const void* position) {
    float p[3];
    std::memcpy(p, position, 12);
    log_calls.push_back("effect:" + std::to_string(kind) + ":" + std::to_string(static_cast<int>(p[0])));
}
extern "C" void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int mode) {
    log_calls.push_back("anim:" + std::string(state == arena + 152 ? "state" : "?") + ":" +
                        std::to_string(reinterpret_cast<std::uintptr_t>(header)) + ":" + std::to_string(mode) +
                        ":byte156=" + std::to_string(arena[156]));
}
extern "C" int format_text(char* buffer, const char* format, ...) {
    va_list args;
    va_start(args, format);
    int value = va_arg(args, int);
    int n = std::snprintf(buffer, 16, format, value);
    va_end(args);
    log_calls.push_back(std::string("format:") + buffer);
    return n;
}
extern "C" char* findTagString__10GameObjectPci(void*, const char* tag, int index) {
    log_calls.push_back(std::string("find:") + tag + ":" + std::to_string(index));
    if (std::string(tag) != "action1" && std::string(tag) != "action2" && std::string(tag) != "action0") return nullptr;
    if (index >= static_cast<int>(tags.size())) return nullptr;
    return const_cast<char*>(tags[index].c_str());
}
extern "C" int scriptStart__FPc(char* script) {
    log_calls.push_back(std::string("script:") + script + (currentRunningObject == arena ? ":owner" : ":noowner"));
    return 0;
}
extern "C" void objectAddToRunList__FP10GameObject(void* object) {
    log_calls.push_back(object == arena ? "runlist" : "runlist:?");
}
extern "C" void* lumpFindResource__FPcT0(const char* lump, const char* name) {
    log_calls.push_back(std::string("lump:") + lump + ":" + name);
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(0x5150));
}
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume) {
    log_calls.push_back(std::string("sound:") + std::to_string(reinterpret_cast<std::uintptr_t>(sequence)) + ":" +
                        (object == arena ? "self" : "?") + ":" + std::to_string(static_cast<int>(volume)));
}

static void put32(unsigned off, unsigned value) { std::memcpy(arena + off, &value, 4); }
static unsigned get32(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
static unsigned long long get64(unsigned off) { unsigned long long v; std::memcpy(&v, arena + off, 8); return v; }

static void setup(int state, int count, unsigned options, const char* name) {
    std::memset(arena, 0, 8192);
    norrath_w8avb_host_base = reinterpret_cast<unsigned long long>(arena);
    put32(72, kTable);
    short adjust = 8;
    std::memcpy(arena + kTable + 152, &adjust, 2);
    put32(kTable + 156, 31);
    put32(144, 0x7770u);
    put32(376, static_cast<unsigned>(state));
    put32(288, static_cast<unsigned>(count));
    put32(284, options);
    float pos[3] = {10.0f, 20.0f, 30.0f};
    std::memcpy(arena + 16, pos, 12);
    std::strcpy(reinterpret_cast<char*>(arena + 76), name);
    std::strcpy(reinterpret_cast<char*>(arena + 109), "lumpname");
    std::strcpy(reinterpret_cast<char*>(arena + 216), "second");
    std::strcpy(reinterpret_cast<char*>(arena + 248), "third");
    for (int i = 0; i < 4; ++i) {
        put32(296 + 8 * i, 0x9000u + 16u * i);  // animation header words
        put32(300 + 8 * i, (i & 1) ? 3u : 1u);   // flags: odd states have bit 1 and bit 0
    }
    put32(300, 0);  // state 0: flags 0
    log_calls.clear();
    chunk_queue.clear();
    chunk_next = 0;
    chunk_mask_args.clear();
    chunk_model_args.clear();
    tags.clear();
    alloc_fail = false;
    alloc_next = 0;
    currentRunningObject = nullptr;
    script_count = 0;
    (void)script_owner_seen;
}
static bool has(const std::string& entry) {
    for (auto& e : log_calls) if (e == entry) return true;
    return false;
}
static std::vector<std::string> filter(const char* prefix) {
    std::vector<std::string> out;
    for (auto& e : log_calls) if (e.rfind(prefix, 0) == 0) out.push_back(e);
    return out;
}

int main() {
    // 1. first call: state 0 -> 1, mask 1|2, two bits created, anim and scripts run, brazier sound
    setup(0, 3, 0x8, "dt_brazier");
    chunk_queue = {0x55ull, 0x0bull, 0x01ull};  // initial store, before = bits 0,1,3, after = bit 0
    tags = {"t0", "t1"};
    advanceState__8AutoProp(arena);
    expect(chunk_mask_args == std::vector<int>({1, 1, 3}), "chunk calls use the low mask word: 1, 1, then 3");
    expect(chunk_model_args[0] == 0x7770u, "model word passed");
    expect(get32(376) == 1 && get32(380) == 0, "state became 1");
    expect(get64(384) == 3ull, "mask is 3");
    expect(get64(200) == 0x01ull, "result stored from the last chunk call");
    expect(has("passable:1:adj"), "slot 152 gets (record flags & 1) = 1 for state 1");
    const std::vector<std::string> props = filter("phprop:");
    expect(props.size() == 2u, "two PhProps (bits 1 and 3)");
    if (props.size() == 2u) {
        expect(props[0] == "phprop:0:pos10,20,30:lumpname:second:third:mask2:flags1020e:nodesc:1", "first PhProp");
        expect(props[1] == "phprop:1:pos10,20,30:lumpname:second:third:mask8:flags1020e:nodesc:1", "second PhProp");
    }
    expect(filter("effect:") == std::vector<std::string>({"effect:2:1", "effect:2:2"}), "effects at each new PhProp position");
    expect(filter("alloc:") == std::vector<std::string>({"alloc:584", "alloc:584"}), "584-byte allocations");
    expect(has("anim:state:" + std::to_string(0x9000 + 16) + ":3:byte156=0"), "anim for state 1: flags bit 1 set -> mode 3");
    expect(filter("format:") == std::vector<std::string>({"format:action1"}), "tag name");
    expect(filter("script:") == std::vector<std::string>({"script:t0:owner", "script:t1:owner"}), "scripts with owner set");
    expect(currentRunningObject == nullptr, "owner cleared");
    expect(has("runlist") && has("lump:lumpname:spel_brazier_out.anm") && has("sound:20816:self:1"), "run list and brazier sound");
    // order of the main stages
    {
        size_t passable = 0, props_at = 0, anim = 0, fmt = 0, run = 0, snd = 0;
        for (size_t i = 0; i < log_calls.size(); ++i) {
            if (log_calls[i].rfind("passable", 0) == 0) passable = i;
            if (log_calls[i].rfind("phprop:0", 0) == 0) props_at = i;
            if (log_calls[i].rfind("anim", 0) == 0) anim = i;
            if (log_calls[i].rfind("format", 0) == 0) fmt = i;
            if (log_calls[i] == "runlist") run = i;
            if (log_calls[i].rfind("sound", 0) == 0) snd = i;
        }
        expect(passable < props_at && props_at < anim && anim < fmt && fmt < run && run < snd, "stage order");
    }

    // 2. changed == after (both zero): no comparison with the first name, no PhProp
    setup(1, 3, 0, "other");
    chunk_queue = {0, 0};
    advanceState__8AutoProp(arena);
    expect(!has("cmp:hateicon") && filter("phprop:").empty(), "equal masks: nothing built");
    expect(get32(376) == 2, "state 1 -> 2");
    expect(has("cmp:dt_brazier") && filter("sound:").empty(), "brazier name compared; no match");

    // 3. hateicon: bit 36 skipped, bit 63 ignored, bit 2 built; flags 0x22e without bit 3, options 0x100 adds 0x100
    setup(1, 3, 0x100, "HateIcon");
    chunk_queue = {(1ull << 36) | (1ull << 63) | 4ull, 0ull};
    advanceState__8AutoProp(arena);
    {
        auto p = filter("phprop:");
        expect(p.size() == 1u && p[0] == "phprop:0:pos10,20,30:lumpname:second:third:mask4:flags32e:nodesc:1", "only bit 2, flags 0x32e");
        expect(filter("effect:").empty(), "no effect without bit 3");
    }
    // same but not hateicon: bit 36 is built
    setup(1, 3, 0, "plain");
    chunk_queue = {(1ull << 36) | 4ull, 0ull};
    advanceState__8AutoProp(arena);
    {
        auto p = filter("phprop:");
        expect(p.size() == 2u && p[1] == "phprop:1:pos10,20,30:lumpname:second:third:mask1000000000:flags22e:nodesc:1",
               "bit 36 built for other names");
    }

    // 4. wrap and no-wrap
    setup(2, 3, 0x80, "x");
    chunk_queue = {0, 0};
    advanceState__8AutoProp(arena);
    expect(get32(376) == 0 && get32(380) == 0, "state wraps to 0");
    expect(has("passable:0:adj"), "slot flag from state 0 record (flags 0)");
    expect(has("anim:state:" + std::to_string(0x9000) + ":6:byte156=0"), "state 0 flags bit 1 clear -> mode 6");
    setup(2, 3, 0x7f, "x");
    put32(380, 77);
    chunk_queue = {0, 0};
    advanceState__8AutoProp(arena);
    expect(get32(376) == 2 && get32(380) == 77, "no wrap without bit 7: state and +380 unchanged");
    expect(filter("anim:").empty() && filter("format:").empty() && has("runlist"), "unchanged state: no animation or tags");

    // 5. null header: no animAdd and byte 156 kept, tags still run
    setup(0, 3, 0, "x");
    arena[156] = 9;
    put32(304, 0);  // state 1 header null
    chunk_queue = {0, 0, 0};
    tags = {"only"};
    advanceState__8AutoProp(arena);
    expect(filter("anim:").empty() && arena[156] == 9, "null header: no animAdd, byte 156 kept");
    expect(filter("script:") == std::vector<std::string>({"script:only:owner"}), "scripts still run");
    // no tags for the state: nothing runs and owner stays clear
    setup(0, 3, 0, "x");
    chunk_queue = {0, 0, 0};
    advanceState__8AutoProp(arena);
    expect(filter("script:").empty() && filter("find:") == std::vector<std::string>({"find:action1:0"}), "no tag: single lookup");

    // 6. allocation failure: no constructor, no effect
    setup(1, 3, 0x8, "x");
    alloc_fail = true;
    chunk_queue = {0x6ull, 0};
    advanceState__8AutoProp(arena);
    expect(filter("alloc:").size() == 2u && filter("phprop:").empty() && filter("effect:").empty(), "failed allocations skip construction");

    // 7. high state: 1 << 31 sign-extends into the mask
    setup(30, 40, 0, "x");
    chunk_queue = {0, 0};
    advanceState__8AutoProp(arena);
    expect(get64(384) == 0xffffffff80000000ull, "1<<31 is sign-extended");
    return failures ? 1 : 0;
}

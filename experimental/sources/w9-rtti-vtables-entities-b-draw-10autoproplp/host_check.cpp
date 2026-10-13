#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#define NORRATH_W8AVB_HOST
extern "C" unsigned long long norrath_w8avb_host_base = 0;
static void* slot_address;
extern "C" void* norrath_w8avb_host_function(unsigned word) { return word == 7 ? slot_address : nullptr; }

#include "draw.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

alignas(16) static unsigned char memory[0x4000];
static const unsigned ENTRY = 0x1000, TABLE = 0x2000;
static std::vector<std::string> log_;
static Point3 slot_point;
static Point3* second_point;
static void* slot_second;
static void* slot_this;

static unsigned get(unsigned off) { unsigned v; std::memcpy(&v, memory + off, 4); return v; }
static void put(unsigned off, unsigned v) { std::memcpy(memory + off, &v, 4); }
static unsigned entry_of(int i) { return ENTRY + 0x200 * i; }

extern "C" void msg_draw__10AutoObject(void* object) { log_.push_back(object == memory ? "base" : "base-wrong"); }
extern "C" void lightActivate__FP5Light(void* light) {
    log_.push_back("on:" + std::to_string((static_cast<unsigned char*>(light) - memory - 256 - ENTRY) / 0x200));
}
extern "C" void lightDeactivate__FP5Light(void* light) {
    log_.push_back("off:" + std::to_string((static_cast<unsigned char*>(light) - memory - 256 - ENTRY) / 0x200));
}
static int shadow_count;
extern "C" int modelGetNmShadowPoints__FP12_modelHeader(void* model) {
    log_.push_back(model == reinterpret_cast<void*>(0x5000) ? "count" : "count-wrong");
    return shadow_count;
}
extern "C" int modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* state, Point3 position, short angle, int index, Point3* out, float* scratch, void* matrix, float scale) {
    log_.push_back("shadow:" + std::to_string(index));
    (void)scratch;
    if (model != reinterpret_cast<void*>(0x5000) || state != memory + 152 || matrix != nullptr || scale != 1.0f ||
        angle != static_cast<short>(20000 - 16384))
        failures += 100;
    unsigned bits;
    std::memcpy(&bits, &position.y, 4);
    if (bits != 0x22222222u) failures += 100;
    *out = Point3{50.0f + static_cast<float>(index), 0.0f, 0.0f};
    return 0;
}
extern "C" void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, Point3 position, short angle, void* state, int index, Point3* out, Point3* other, float scale) {
    log_.push_back("attach:" + std::to_string(index));
    if (model != reinterpret_cast<void*>(0x5000) || state != memory + 152 || scale != 1.0f ||
        angle != static_cast<short>(20000 - 16385))
        failures += 100;
    (void)position;
    *out = Point3{100.0f + static_cast<float>(index), 2.0f, 3.0f};
    *other = Point3{9.0f, 9.0f, 9.0f};
    second_point = other;
}
static void slot_function(void* self, Point3 p, void* second, int kind) {
    log_.push_back("slot:" + std::to_string(kind));
    slot_this = self;
    slot_point = p;
    slot_second = second;
}

static void setup(int row_base, int reference) {
    std::memset(memory, 0, sizeof memory);
    norrath_w8avb_host_base = reinterpret_cast<unsigned long long>(memory);
    put(376, row_base);
    put(380, reference);
    put(144, 0x5000);
    const unsigned short angle = 20000;
    std::memcpy(memory + 56, &angle, 2);
    put(16, 0x11111111u); put(20, 0x22222222u); put(24, 0x33333333u);
    log_.clear();
}
static void make_entry(int i, unsigned flags, unsigned w80, unsigned w84, unsigned w88, unsigned w252, int adjust) {
    put(424 + 4 * i, entry_of(i));
    put(440 + 4 * i, flags);
    put(entry_of(i) + 80, w80);
    put(entry_of(i) + 84, w84);
    put(entry_of(i) + 88, w88);
    put(entry_of(i) + 252, w252);
    put(entry_of(i) + 72, TABLE);
    short a = static_cast<short>(adjust);
    std::memcpy(memory + TABLE + 136, &a, 2);
    put(TABLE + 136 + 4, 7);
    put(TABLE + 144 + 4, 9);
    put(entry_of(i) + 92, 0x77777777u);
    put(entry_of(i) + 96, 0x77777777u);
}
static void rows(int index, int row_base, int low1, int high1, int low2, int high2) {
    const int m = 10 * index + row_base;
    put(456 + 8 * m, low1); put(460 + 8 * m, high1);
    put(776 + 8 * m, low2); put(780 + 8 * m, high2);
}

int main() {
    slot_address = reinterpret_cast<void*>(&slot_function);

    // No entries: only the final base call, and no guard word is consulted.
    setup(0, 0);
    msg_draw__10AutoPropLP(memory);
    expect(log_.size() == 1 && log_[0] == "base", "no entries: base only");

    // Null and idle entries are skipped.
    setup(0, 4);
    make_entry(1, 1, 0, 0, 0, 0, 0);
    make_entry(2, 1, 0, 0, 3, 0, 0);
    rows(2, 0, 0, 5, 0, 5);
    shadow_count = 0;
    msg_draw__10AutoPropLP(memory);
    expect(get(entry_of(1) + 92) == 0x77777777u, "idle entry untouched");
    expect(get(entry_of(2) + 92) == 1 && get(entry_of(2) + 252) == 1, "entry with only +88 set is processed");

    // Pair tests as in the run check.
    struct Pair { int low, high, ref; bool inside; };
    const Pair pairs[] = {{5, 3, 100, true}, {3, 5, 4, true}, {3, 5, 3, false}, {3, 5, 5, false}, {3, 5, 9, false}, {3, 3, 3, false}, {-4, -2, -3, true}};
    for (const Pair& p : pairs) {
        setup(2, p.ref);
        make_entry(3, 0, 1, 0, 0, 0, 0);
        rows(3, 2, p.low, p.high, p.low, p.high);
        put(entry_of(3) + 96, 0x55555555u);
        msg_draw__10AutoPropLP(memory);
        expect(get(entry_of(3) + 92) == (p.inside ? 1u : 0u), "first pair: +92");
        expect(get(entry_of(3) + 96) == (p.inside ? 0u : 0x55555555u), "first pair: +96");
        expect(get(entry_of(3) + 252) == (p.inside ? 1u : 0u), "second pair: +252");
    }
    setup(0, 4);
    make_entry(0, 0, 0, 9, 0, 0, 0);
    rows(0, 0, 3, 5, 3, 5);
    msg_draw__10AutoPropLP(memory);
    expect(log_[0] == "on:0", "inside with +84 set: light on");
    setup(0, 4);
    make_entry(0, 0, 0, 9, 0, 1, 0);
    rows(0, 0, 8, 9, 8, 9);
    msg_draw__10AutoPropLP(memory);
    expect(log_[0] == "off:0" && get(entry_of(0) + 252) == 0, "outside with +84 set: light off");

    // Flag bit 0: one slot call per shadow point; the last argument is 1, 2 or 3.
    for (int n : {-2, 0, 1, 2, 3, 5}) {
        setup(0, 0);
        make_entry(1, 1, 1, 0, 0, 0, 24);
        rows(1, 0, 1, 1, 1, 1);
        shadow_count = n;
        msg_draw__10AutoPropLP(memory);
        std::vector<std::string> want = {"count"};
        for (int j = 0; j < n; ++j) {
            want.push_back("shadow:" + std::to_string(j));
            want.push_back("slot:" + std::to_string(j == 0 ? 1 : (j == n - 1 ? 3 : 2)));
        }
        want.push_back("base");
        expect(log_ == want, "shadow loop order and kinds");
        if (n > 0) {
            expect(slot_this == memory + entry_of(1) + 24 && slot_second == nullptr, "adjusted entry, null third argument");
            expect(slot_point.x == 50.0f + static_cast<float>(n - 1), "last point passed");
        }
    }

    // Flag bit 0 clear: attachment point, then slot kind 0 with the second point only when bit 1 is set.
    for (unsigned flags : {0u, 2u, 4u, 6u}) {
        setup(0, 0);
        make_entry(2, flags, 1, 0, 0, 0, -16);
        rows(2, 0, 1, 1, 1, 1);
        msg_draw__10AutoPropLP(memory);
        const std::vector<std::string> want = {"attach:2", "slot:0", "base"};
        expect(log_ == want, "attach, slot, base");
        expect(slot_this == memory + entry_of(2) - 16 && slot_point.x == 102.0f, "slot gets the first point");
        expect(slot_second == ((flags & 2) ? static_cast<void*>(second_point) : nullptr), "second point only with bit 1");
    }
    return failures ? 1 : 0;
}

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#define NORRATH_W8AVB_HOST
extern "C" unsigned long long norrath_w8avb_host_base = 0;
static void* slot_address;
extern "C" void* norrath_w8avb_host_function(unsigned word) { return word == 7 ? slot_address : nullptr; }

#include "run.cpp"

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
static Point3 slot_point, attach_first;
static void* slot_this;
static unsigned slot_triple[3];
static int attach_index;
static short attach_angle;
static float attach_scale;
static void* attach_model;
static void* attach_state;

static unsigned get(unsigned off) { unsigned v; std::memcpy(&v, memory + off, 4); return v; }
static void put(unsigned off, unsigned v) { std::memcpy(memory + off, &v, 4); }
static unsigned entry_of(int i) { return ENTRY + 0x200 * i; }

extern "C" void msg_run__8AutoProp(void* object) { log_.push_back(object == memory ? "base" : "base-wrong"); }
extern "C" void lightActivate__FP5Light(void* light) {
    log_.push_back("on:" + std::to_string((static_cast<unsigned char*>(light) - memory - 256 - ENTRY) / 0x200));
}
extern "C" void lightDeactivate__FP5Light(void* light) {
    log_.push_back("off:" + std::to_string((static_cast<unsigned char*>(light) - memory - 256 - ENTRY) / 0x200));
}
extern "C" void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, Point3 position, short angle, void* state, int index, Point3* out, Point3* other, float scale) {
    log_.push_back("attach");
    attach_model = model;
    attach_state = state;
    attach_index = index;
    attach_angle = angle;
    attach_scale = scale;
    attach_first = position;
    *out = Point3{100.0f + static_cast<float>(index), 2.0f, 3.0f};
    *other = Point3{9.0f, 9.0f, 9.0f};
}
static void slot_function_triple(void* self, Triple t) {
    log_.push_back("slot-copy");
    slot_this = self;
    slot_triple[0] = t.a; slot_triple[1] = t.b; slot_triple[2] = t.c;
}
static void slot_function_point(void* self, Point3 p) {
    log_.push_back("slot-point");
    slot_this = self;
    slot_point = p;
}

static void setup(int word280, int row_base, int reference) {
    std::memset(memory, 0, sizeof memory);
    norrath_w8avb_host_base = reinterpret_cast<unsigned long long>(memory);
    put(280, word280);
    put(376, row_base);
    put(380, reference);
    put(144, 0x5000);
    const unsigned short angle = 20000;
    std::memcpy(memory + 56, &angle, 2);
    put(16, 0x11111111u); put(20, 0x22222222u); put(24, 0x33333333u);
    for (int i = 0; i < 4; ++i) {
        put(424 + 4 * i, 0);
        put(440 + 4 * i, 0);
    }
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
    std::memcpy(memory + TABLE + 144, &a, 2);
    put(TABLE + 144 + 4, 7);
    put(entry_of(i) + 92, 0x77777777u);
    put(entry_of(i) + 96, 0x77777777u);
}
static void rows(int index, int row_base, int low1, int high1, int low2, int high2) {
    const int m = 10 * index + row_base;
    put(456 + 8 * m, low1); put(460 + 8 * m, high1);
    put(776 + 8 * m, low2); put(780 + 8 * m, high2);
}

int main() {
    // The slot function is called through a typed pointer; one stub per signature.
    // Zero guard word: only the base method runs.
    slot_address = reinterpret_cast<void*>(&slot_function_triple);
    setup(0, 0, 0);
    make_entry(0, 1, 1, 1, 1, 0, 0);
    msg_run__10AutoPropLP(memory);
    expect(log_.size() == 1 && log_[0] == "base", "guard word zero: base only");

    // Null and idle entries are skipped, even when others are not.
    setup(1, 0, 4);
    make_entry(1, 1, 0, 0, 0, 0, 0);
    make_entry(2, 1, 0, 0, 3, 0, 0);
    rows(2, 0, 0, 5, 0, 5);
    msg_run__10AutoPropLP(memory);
    expect(log_.size() >= 1 && log_[0] == "base", "base first");
    expect(get(entry_of(1) + 92) == 0x77777777u && get(entry_of(1) + 252) == 0, "idle entry untouched");
    expect(get(entry_of(2) + 92) == 1 && get(entry_of(2) + 252) == 1, "entry with only +88 set is processed");

    // First pair: high < low, or low < reference < high, sets +92 = 1 and +96 = 0.
    struct Pair { int low, high, ref; bool inside; };
    const Pair pairs[] = {{5, 3, 100, true}, {3, 5, 4, true}, {3, 5, 3, false}, {3, 5, 5, false}, {3, 5, 9, false}, {3, 3, 3, false}, {-4, -2, -3, true}, {7, 7, 0, false}};
    for (const Pair& p : pairs) {
        setup(1, 2, p.ref);
        make_entry(3, 0, 1, 0, 0, 0, 0);
        rows(3, 2, p.low, p.high, p.low, p.high);
        put(entry_of(3) + 96, 0x55555555u);
        slot_address = reinterpret_cast<void*>(&slot_function_point);
        msg_run__10AutoPropLP(memory);
        expect(get(entry_of(3) + 92) == (p.inside ? 1u : 0u), "first pair: +92");
        expect(get(entry_of(3) + 96) == (p.inside ? 0u : 0x55555555u), "first pair: +96 cleared only when inside");
        expect(get(entry_of(3) + 252) == (p.inside ? 1u : 0u), "second pair: +252");
    }

    // Light calls follow the second pair and the +84 word; +252 resets to 0 when outside.
    setup(1, 0, 4);
    make_entry(0, 0, 0, 9, 0, 0, 0);
    rows(0, 0, 3, 5, 3, 5);
    slot_address = reinterpret_cast<void*>(&slot_function_point);
    msg_run__10AutoPropLP(memory);
    expect(log_[1] == "on:0" && get(entry_of(0) + 252) == 1, "inside with +84 set: light on");
    setup(1, 0, 4);
    make_entry(0, 0, 0, 9, 0, 1, 0);
    rows(0, 0, 8, 9, 8, 9);
    msg_run__10AutoPropLP(memory);
    expect(log_[1] == "off:0" && get(entry_of(0) + 252) == 0, "outside with +84 set: light off");
    setup(1, 0, 4);
    make_entry(0, 0, 4, 0, 0, 1, 0);
    rows(0, 0, 8, 9, 8, 9);
    msg_run__10AutoPropLP(memory);
    expect(log_[1] == "attach" && get(entry_of(0) + 252) == 0, "outside with +84 clear: no light call");

    // Flag bit 0 set: slot with a copy of the three words at +16.
    slot_address = reinterpret_cast<void*>(&slot_function_triple);
    setup(1, 0, 0);
    make_entry(1, 1, 1, 0, 0, 0, 40);
    rows(1, 0, 1, 1, 1, 1);
    msg_run__10AutoPropLP(memory);
    expect(log_.size() == 2 && log_[1] == "slot-copy", "flag bit 0: slot call only");
    expect(slot_this == memory + entry_of(1) + 40, "adjusted entry address");
    expect(slot_triple[0] == 0x11111111u && slot_triple[1] == 0x22222222u && slot_triple[2] == 0x33333333u, "copy of the object's three words");

    // Flag bit 0 clear: attachment point, then slot with the first point.
    slot_address = reinterpret_cast<void*>(&slot_function_point);
    setup(1, 0, 0);
    make_entry(2, 2, 1, 0, 0, 0, -16);
    rows(2, 0, 1, 1, 1, 1);
    msg_run__10AutoPropLP(memory);
    expect(log_.size() == 3 && log_[1] == "attach" && log_[2] == "slot-point", "attach then slot");
    expect(attach_model == reinterpret_cast<void*>(0x5000) && attach_state == memory + 152 && attach_index == 2, "attachment arguments");
    expect(attach_angle == static_cast<short>(20000 - 16385) && attach_scale == 1.0f, "angle minus 16385 and scale 1.0");
    expect(slot_point.x == 102.0f && slot_this == memory + entry_of(2) - 16, "slot gets the first output point");
    unsigned bits;
    std::memcpy(&bits, &attach_first.x, 4);
    expect(bits == 0x11111111u, "position copy passed");
    return failures ? 1 : 0;
}

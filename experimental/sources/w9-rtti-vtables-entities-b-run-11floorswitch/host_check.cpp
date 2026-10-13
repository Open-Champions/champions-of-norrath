#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include "run.cpp"

int world = 0;

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s (", message);
        ++failures;
    }
}

static std::vector<std::string> log_;
static unsigned char* current_object;
alignas(8) static unsigned char others[3][64];
static void* list_contents[3];
static unsigned list_count;
static char key_text[] = "k1";
static char other_names[3][4];
static bool name_present[3];
static const char* key_tag;
static int only_on, only_off, rand_value;
static float height_arg, move_z;
static int actions_arg;
static Point3 find_low, find_high;
static std::string format_seen;
static int format_arg;

static std::string number(int v) { return std::to_string(v); }

extern "C" void worldFindDoorBoundingBox__FP12_worldHeaderiP6Point3T2(void* w, int index, Point3* low, Point3* high) {
    log_.push_back("bbox:" + number(index) + (w == reinterpret_cast<void*>(4660) ? "" : ":badworld"));
    *low = Point3{1.0f, 2.0f, 3.0f};
    *high = Point3{11.0f, 12.0f, 13.0f};
}
extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** out, int capacity) {
    log_.push_back("find:" + number(capacity));
    find_low = low;
    find_high = high;
    for (unsigned i = 0; i < list_count; ++i) out[i] = list_contents[i];
    out[list_count] = nullptr;
}
extern "C" char* findTagString__10GameObjectPci(void* object, const char* tag, int index) {
    log_.push_back(std::string("tag:") + tag + ":" + number(index));
    if (std::strcmp(tag, "keyObject") == 0) return const_cast<char*>(key_tag);
    for (int i = 0; i < 3; ++i)
        if (object == others[i]) return name_present[i] ? other_names[i] : nullptr;
    return nullptr;
}
extern "C" int findTagInt__10GameObjectPci(void* object, const char* tag, int index) {
    (void)object;
    (void)index;
    log_.push_back(std::string("int:") + tag);
    return std::strcmp(tag, "onOnly") == 0 ? only_on : only_off;
}
extern "C" int strcmp(const char* a, const char* b) {
    int i = 0;
    while (a[i] && a[i] == b[i]) ++i;
    return static_cast<unsigned char>(a[i]) - static_cast<unsigned char>(b[i]);
}
extern "C" void objectAddToRunList__FP10GameObject(void*) { log_.push_back("run"); }
extern "C" void objectAddToMediumRunList__FP10GameObject(void*) { log_.push_back("medium"); }
extern "C" int eRand__10GameObject(void*) { log_.push_back("rand"); return rand_value; }
extern "C" int sprintf(char* destination, const char* format, ...) {
    __builtin_va_list ap;
    __builtin_va_start(ap, format);
    format_arg = __builtin_va_arg(ap, int);
    __builtin_va_end(ap);
    format_seen = format;
    log_.push_back("sprintf");
    std::memcpy(destination, "seq", 4);
    return 3;
}
static char lump_result;
extern "C" char* lumpFindResource__FPcT0(const char* lump, const char* name) {
    log_.push_back(std::string("lump:") + lump + ":" + name);
    return &lump_result;
}
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float rate) {
    log_.push_back(header == &lump_result && object == current_object && rate == 1.0f ? "seq" : "seq-wrong");
    return 0;
}
extern "C" int worldMoveDoor__FP12_worldHeaderiG6Point3s(void* w, int index, Point3 point, short extra) {
    log_.push_back("move");
    move_z = point.z;
    if (w != reinterpret_cast<void*>(4660) || index != -5 || point.x != 0.0f || point.y != 0.0f || extra != 0) failures += 100;
    return 0;
}
extern "C" int worldSetDoorHeight__FP12_worldHeaderif(void* w, int index, float height) {
    log_.push_back("height");
    height_arg = height;
    if (w != reinterpret_cast<void*>(4660) || index != -5) failures += 100;
    return 0;
}
extern "C" void objectDoMyActions__FP10GameObjecti(void*, int on) {
    log_.push_back("actions");
    actions_arg = on;
}

static float read_float(unsigned off) { float v; std::memcpy(&v, current_object + off, 4); return v; }

struct Setup {
    int state;
    float value;
    unsigned flags[3];
    bool names[3];
    bool matching[3];
    const char* key;
    unsigned count;
};

static void run(const Setup& s, unsigned char* object) {
    std::memset(object, 0x22, 512);
    object[80] = static_cast<unsigned char>(s.state);
    std::memcpy(object + 84, &s.value, 4);
    const short door = -5, height = 100;
    std::memcpy(object + 76, &door, 2);
    std::memcpy(object + 78, &height, 2);
    for (int i = 0; i < 3; ++i) {
        std::memset(others[i], 0, sizeof others[i]);
        std::memcpy(others[i] + 12, &s.flags[i], 4);
        list_contents[i] = others[i];
        name_present[i] = s.names[i];
        std::strcpy(other_names[i], s.matching[i] ? "k1" : "zz");
    }
    list_count = s.count;
    key_tag = s.key;
    world = 4660;
    current_object = object;
    log_.clear();
    msg_run__11FloorSwitch(object);
}

static bool has(const char* entry) {
    for (const std::string& e : log_) if (e == entry) return true;
    return false;
}
static int count_of(const char* entry) {
    int n = 0;
    for (const std::string& e : log_) if (e == entry) ++n;
    return n;
}

int main() {
    alignas(8) unsigned char object[512];
    only_on = only_off = 0;
    rand_value = 6;

    // Pressed with no keyObject tag: the first object without flag bit 0 counts.
    run({0, 0.0f, {1, 0, 0}, {false, false, false}, {false, false, false}, nullptr, 3}, object);
    expect(log_[0] == "bbox:-5" && log_[1] == "find:64" && log_[2] == "tag:keyObject:0", "scan setup order");
    expect(find_low.x == 1.0f && find_high.z == 19.0f && find_high.x == 11.0f, "6.0 added to the high z only");
    expect(object[80] == 1 && has("run") && has("lump:fx.lmp:seq") && has("seq") && format_seen == "game_floorplt_dprss%d.anm" && format_arg == 1, "state 0 pressed: sequence 1 (rand is even), state 1");
    expect(has("actions") && actions_arg == 1, "onOnly/offOnly zero: argument is 'state is 1'");
    rand_value = 7;
    run({0, 0.0f, {1, 1, 1}, {false, false, false}, {false, false, false}, nullptr, 3}, object);
    expect(object[80] == 0x00 && !has("run"), "every object skipped: not pressed, nothing to lower");

    // Key tag must equal a name tag.
    only_on = only_off = 0;
    run({0, 0.0f, {0, 0, 0}, {true, true, true}, {false, true, false}, key_text, 3}, object);
    expect(object[80] == 1 && format_arg == 2 && count_of("tag:name:0") == 4, "the second object matches; each candidate's name tag is looked up twice");
    run({0, 0.0f, {0, 0, 0}, {false, true, true}, {true, false, false}, key_text, 3}, object);
    expect(object[80] == 0, "no name or mismatch: not pressed");
    run({0, 0.0f, {0, 0, 0}, {true, true, true}, {false, false, true}, key_text, 2}, object);
    expect(object[80] == 0, "matching object beyond the list end is not scanned");

    // State 0, not pressed: lower a positive value.
    run({0, 1.0f, {1, 1, 1}, {false, false, false}, {false, false, false}, nullptr, 3}, object);
    expect(read_float(84) == 0.75f && move_z == -0.75f && height_arg == 100.0f - 0.75f && !has("medium") && !has("actions"), "lowering from 1.0");
    run({0, 0.25f, {1, 1, 1}, {false, false, false}, {false, false, false}, nullptr, 3}, object);
    expect(read_float(84) == 0.0f && has("medium") && has("move") && has("height"), "reaching zero joins the medium run list");
    run({0, 0.0f, {1, 1, 1}, {false, false, false}, {false, false, false}, nullptr, 3}, object);
    expect(!has("move") && !has("medium"), "zero: nothing");
    run({0, -2.0f, {1, 1, 1}, {false, false, false}, {false, false, false}, nullptr, 3}, object);
    expect(!has("move") && !has("medium") && read_float(84) == -2.0f, "negative: nothing");

    // State 1.
    only_on = 0; only_off = 0;
    run({1, 0.0f, {1, 1, 1}, {false, false, false}, {false, false, false}, nullptr, 3}, object);
    expect(object[80] == 0 && has("actions") && actions_arg == 0, "state 1 not pressed: back to 0, argument is 'state is 1' = 0");
    only_on = 1; only_off = 0;
    run({1, 0.0f, {1, 1, 1}, {false, false, false}, {false, false, false}, nullptr, 3}, object);
    expect(actions_arg == 1, "onOnly nonzero: argument 1");
    only_off = 1;
    run({1, 0.0f, {1, 1, 1}, {false, false, false}, {false, false, false}, nullptr, 3}, object);
    expect(actions_arg == 0, "offOnly nonzero: argument 0");
    run({1, 0.5f, {0, 0, 0}, {false, false, false}, {false, false, false}, nullptr, 3}, object);
    expect(object[80] == 1 && read_float(84) == 0.75f && move_z == -0.75f && !has("medium") && !has("actions"), "state 1 pressed raises");
    run({1, 0.75f, {0, 0, 0}, {false, false, false}, {false, false, false}, nullptr, 3}, object);
    expect(read_float(84) == 1.0f && has("medium"), "reaching 1.0 joins the medium run list");
    run({1, 1.0f, {0, 0, 0}, {false, false, false}, {false, false, false}, nullptr, 3}, object);
    expect(!has("move") && read_float(84) == 1.0f, "1.0: nothing");

    // Other states do nothing but the scan.
    run({2, 0.5f, {0, 0, 0}, {false, false, false}, {false, false, false}, nullptr, 3}, object);
    expect(object[80] == 2 && !has("move") && !has("actions") && !has("run"), "state 2: nothing");
    run({-1, 0.5f, {0, 0, 0}, {false, false, false}, {false, false, false}, nullptr, 3}, object);
    expect(object[80] == 0xff && !has("move") && !has("actions"), "state -1: nothing");
    if (failures) std::fprintf(stderr, ")\n");
    return failures ? 1 : 0;
}

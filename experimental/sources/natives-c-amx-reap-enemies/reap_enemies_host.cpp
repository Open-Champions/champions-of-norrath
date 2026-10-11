#include <cstdio>
#include <cstring>

extern "C" int amxReapEnemies__FP5__amxPi(void*, int*);

static unsigned char object[96];
static unsigned char table[32];
static float box_min[3];
static float box_max[3];
static int box_count;
static int type_calls;
static int reap_calls;
static void* reap_self;
static int reap_kind;
static int match_on;
static int empty;
static const char phprop[] = "ph";
static const char prop[] = "prop";
static const char item[] = "item";
static void* last_left;
static void* last_right;

extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(const float* minimum, const float* maximum, void** objects, int count) {
    std::memcpy(box_min, minimum, sizeof box_min);
    std::memcpy(box_max, maximum, sizeof box_max);
    box_count = count;
    objects[0] = empty ? nullptr : object;
    objects[1] = nullptr;
}

static void on_reap(void* self, int kind) {
    reap_self = self;
    reap_kind = kind;
    reap_calls += 1;
}

static void install_reap() {
    short adjust = -2;
    std::memcpy(table + 8, &adjust, sizeof adjust);
    auto function = on_reap;
    std::memcpy(table + 12, &function, sizeof function);
}

static void* on_type() {
    type_calls += 1;
    if (type_calls != match_on) return nullptr;
    install_reap();
    if (match_on == 1) return const_cast<char*>(phprop);
    if (match_on == 2) return const_cast<char*>(prop);
    return const_cast<char*>(item);
}

static void arm_table() {
    void* pointer = table;
    std::memcpy(object + 72, &pointer, sizeof pointer);
}

static void arm_type() {
    arm_table();
    auto function = on_type;
    std::memcpy(table + 4, &function, sizeof function);
}

extern "C" void* __tfP6PhProp() { return const_cast<char*>(phprop); }
extern "C" void* __tfP4Prop() { return const_cast<char*>(prop); }
extern "C" void* __tfP8ItemProp() { return const_cast<char*>(item); }
extern "C" int __eq__C9type_infoRC9type_info(void* left, void* right) {
    last_left = left;
    last_right = right;
    return left == right && left != nullptr;
}
extern "C" void __throw_bad_typeid() {}

static int flags_of(int flags) {
    std::memcpy(object + 12, &flags, 4);
    return flags;
}

static int box_ok() {
    unsigned low = 0;
    unsigned high = 0;
    std::memcpy(&low, &box_min[0], 4);
    std::memcpy(&high, &box_max[2], 4);
    if (low != 0xc76a6000u || high != 0x476a6000u) return 0;
    if (box_min[1] != box_min[0] || box_max[0] != box_max[1]) return 0;
    return box_count == 512;
}

int main() {
    empty = 1;
    if (amxReapEnemies__FP5__amxPi(nullptr, nullptr) != 0) return 1;
    if (!box_ok() || type_calls != 0 || reap_calls != 0) return 2;

    empty = 0;
    arm_table();
    flags_of(0x68);
    install_reap();
    if (amxReapEnemies__FP5__amxPi(nullptr, nullptr) != 0) return 3;
    if (type_calls != 0 || reap_calls != 0) return 4;

    flags_of(0x28);
    reap_calls = 0;
    install_reap();
    if (amxReapEnemies__FP5__amxPi(nullptr, nullptr) != 0) return 5;
    if (type_calls != 0 || reap_calls != 1 || reap_kind != 3 || reap_self != object - 2) return 6;

    flags_of(0);
    arm_type();
    match_on = 0;
    type_calls = 0;
    reap_calls = 0;
    if (amxReapEnemies__FP5__amxPi(nullptr, nullptr) != 0) return 7;
    if (type_calls != 3 || reap_calls != 0) return 8;

    arm_type();
    match_on = 1;
    type_calls = 0;
    reap_calls = 0;
    if (amxReapEnemies__FP5__amxPi(nullptr, nullptr) != 0) return 9;
    if (type_calls != 1 || reap_calls != 1 || last_right != phprop) return 10;

    flags_of(0x40);
    arm_type();
    match_on = 3;
    type_calls = 0;
    reap_calls = 0;
    if (amxReapEnemies__FP5__amxPi(nullptr, nullptr) != 0) return 11;
    if (type_calls != 3 || reap_calls != 0 || last_right != item) return 12;
    std::printf("reap enemies passed\n");
    return 0;
}

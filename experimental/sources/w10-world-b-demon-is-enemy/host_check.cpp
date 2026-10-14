// Host contract for isEnemy__5DemonP10GameObject. Not an EE plan input. Build: c++ -std=c++17 host_check.cpp
// A host pointer is 8 bytes: the object's table pointer at +72 and the function pointer at
// table + 4 are host pointers (the unit reads nothing else near them).
#include <cstdio>
#include <cstring>
static int base_answer, base_calls, type_calls, eq_calls;
static int soul_type, other_type;
static void* eq_left; static void* eq_right;
static int order[8], order_n;
extern "C" int isEnemy__8CreatureP10GameObject(unsigned char*, unsigned char*) { ++base_calls; order[order_n++] = 1; return base_answer; }
extern "C" void* __tf4Soul() { order[order_n++] = 3; return &soul_type; }
extern "C" int __eq__C9type_infoRC9type_info(void* l, void* r) { ++eq_calls; eq_left = l; eq_right = r; return l == r; }
static void* type_fn_soul() { ++type_calls; order[order_n++] = 2; return &soul_type; }
static void* type_fn_other() { ++type_calls; order[order_n++] = 2; return &other_type; }
#include "demon_is_enemy.cpp"
static void make(unsigned char* obj, unsigned char* table, void* (*fn)(), unsigned flags) {
    std::memset(obj, 0, 1000); std::memset(table, 0, 64);
    std::memcpy(table + 4, &fn, sizeof fn);
    std::memcpy(obj + 72, &table, sizeof table);
    std::memcpy(obj + 12, &flags, 4);
}
int main() {
    unsigned char self[1000] = {0}, obj[1000], table[64];
    // The base answer wins when nonzero, and nothing else is consulted.
    make(obj, table, type_fn_soul, 0);
    base_answer = 5; order_n = base_calls = type_calls = eq_calls = 0;
    if (isEnemy__5DemonP10GameObject(self, obj) != 5 || type_calls != 0 || eq_calls != 0) return 1;
    base_answer = -2;
    if (isEnemy__5DemonP10GameObject(self, obj) != -2) return 2;
    // Null other object: the (zero) base answer is returned.
    base_answer = 0;
    if (isEnemy__5DemonP10GameObject(self, nullptr) != 0 || eq_calls != 0) return 3;
    // Soul, not flagged: 3. Call order is base, type function, __tf4Soul, comparison.
    order_n = 0; eq_calls = 0;
    if (isEnemy__5DemonP10GameObject(self, obj) != 3) return 4;
    if (order_n != 3 || order[0] != 1 || order[1] != 2 || order[2] != 3) return 5;
    if (eq_left != &soul_type || eq_right != &soul_type) return 6;
    // Soul with flag 0x8: 0. Other bits of the flag word do not matter.
    make(obj, table, type_fn_soul, 0x8);
    if (isEnemy__5DemonP10GameObject(self, obj) != 0) return 7;
    make(obj, table, type_fn_soul, 0xfffffff7u);
    if (isEnemy__5DemonP10GameObject(self, obj) != 3) return 8;
    // Another type: 0 whatever the flag.
    make(obj, table, type_fn_other, 0);
    if (isEnemy__5DemonP10GameObject(self, obj) != 0) return 9;
    std::puts("Demon isEnemy check passed");
    return 0;
}

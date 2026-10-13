// Host contract for isEnemy__8SkeletonP10GameObject. Not an EE plan input. Build: c++ -std=c++17 host_check.cpp
// A host pointer is 8 bytes: the other object's table pointer at +72 and the function pointer
// at table + 4 are host pointers (the unit reads nothing else near them).
#include <cstdio>
#include <cstring>
static int base_answer, type_calls, eq_calls;
static int skeleton_type, other_type;
extern "C" int isEnemy__8CreatureP10GameObject(unsigned char*, unsigned char*) { return base_answer; }
extern "C" void* __tf8Skeleton() { return &skeleton_type; }
extern "C" int __eq__C9type_infoRC9type_info(void* l, void* r) { ++eq_calls; return l == r; }
static void* type_fn_skeleton() { ++type_calls; return &skeleton_type; }
static void* type_fn_other() { ++type_calls; return &other_type; }
#include "skeleton_is_enemy.cpp"
static void make(unsigned char* obj, unsigned char* table, void* (*fn)(), unsigned flags, int marker) {
    std::memset(obj, 0, 1000); std::memset(table, 0, 64);
    std::memcpy(table + 4, &fn, sizeof fn);
    std::memcpy(obj + 72, &table, sizeof table);
    std::memcpy(obj + 12, &flags, 4);
    obj[972] = static_cast<unsigned char>(marker);
}
int main() {
    unsigned char self[1000], obj[1000], table[64];
    make(self, table, nullptr, 0, 2);
    // Nonzero base answer: plus one, no type test.
    make(obj, table, type_fn_skeleton, 0, 1);
    base_answer = 4; type_calls = eq_calls = 0;
    if (isEnemy__8SkeletonP10GameObject(self, obj) != 5 || type_calls != 0 || eq_calls != 0) return 1;
    base_answer = -1;
    if (isEnemy__8SkeletonP10GameObject(self, obj) != 1) return 2;  // -1 + 1 == 0 falls through to the tests
    base_answer = 0;
    if (isEnemy__8SkeletonP10GameObject(self, nullptr) != 0) return 3;
    base_answer = 1;
    if (isEnemy__8SkeletonP10GameObject(self, nullptr) != 2) return 4;
    // Opposite markers 2/1 and 1/2 give 1; equal, zero or other markers give 0.
    base_answer = 0;
    const int marks[][3] = {{2,1,1},{1,2,1},{2,2,0},{1,1,0},{0,0,0},{2,0,0},{0,1,0},{3,1,0},{1,3,0},{-1,1,0},{1,-2,0},{2,-1,0},{0x7f,2,0}};
    for (auto& m : marks) {
        make(self, table, nullptr, 0, m[0]);
        make(obj, table, type_fn_skeleton, 0, m[1]);
        if (isEnemy__8SkeletonP10GameObject(self, obj) != m[2]) return 10 + int(&m - &marks[0]);
    }
    // Self halfword at +792 must be zero (either byte of it blocks).
    make(self, table, nullptr, 0, 2); make(obj, table, type_fn_skeleton, 0, 1);
    self[792] = 1; if (isEnemy__8SkeletonP10GameObject(self, obj) != 0) return 30;
    self[792] = 0; self[793] = 1; if (isEnemy__8SkeletonP10GameObject(self, obj) != 0) return 31;
    self[793] = 0; self[794] = 1; if (isEnemy__8SkeletonP10GameObject(self, obj) != 1) return 32;
    // Flag bit 0x8 on the other object blocks; other bits do not.
    make(obj, table, type_fn_skeleton, 0x8, 1);
    if (isEnemy__8SkeletonP10GameObject(self, obj) != 0) return 33;
    make(obj, table, type_fn_skeleton, 0xfffffff7u, 1);
    if (isEnemy__8SkeletonP10GameObject(self, obj) != 1) return 34;
    // A different type blocks.
    make(obj, table, type_fn_other, 0, 1);
    if (isEnemy__8SkeletonP10GameObject(self, obj) != 0) return 35;
    std::puts("Skeleton isEnemy check passed");
    return 0;
}

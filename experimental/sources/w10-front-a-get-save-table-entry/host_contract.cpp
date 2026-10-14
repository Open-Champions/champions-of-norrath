#include "get_save_table_entry.cpp"

#include <cstdio>
#include <cstring>

int curAct;
int actLevelCount[5] = {3, 0, 2, 1, 2};
// Host pointers are 8 bytes, so one host pointer stands for one 8-byte table entry.
static const char* act0[] = {"a_one", "ab", "x"};
static const char* act2[] = {"cave", "cavern"};
unsigned char* allActs[5] = {reinterpret_cast<unsigned char*>(act0), nullptr, reinterpret_cast<unsigned char*>(act2),
                             nullptr, nullptr};


int main() {
    char name1[] = "ab_long", name2[] = "x2", name3[] = "cavernous", name4[] = "zzz";
    curAct = -1;
    if (getSaveTableEntry__FPc(name1) != -1) return 1;
    curAct = 0;
    if (getSaveTableEntry__FPc(name1) != 1) return 2;
    if (getSaveTableEntry__FPc(name2) != 2) return 3;
    if (getSaveTableEntry__FPc(name4) != -1) return 4;
    curAct = 1;
    if (getSaveTableEntry__FPc(name1) != -1) return 5;
    curAct = 2;
    if (getSaveTableEntry__FPc(name3) != 0) return 6;
    std::puts("get save table entry passed");
    return 0;
}

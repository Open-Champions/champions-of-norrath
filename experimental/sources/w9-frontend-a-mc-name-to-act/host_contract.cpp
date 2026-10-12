#include "mc_name_to_act.cpp"

#include <cstdio>

int actCount;
int actLevelCount[6];
unsigned char* allActs[6];

static int compares;
extern "C" __SIZE_TYPE__ strlen(const char* text) {
    __SIZE_TYPE__ n = 0;
    while (text[n] != 0) n += 1;
    return n;
}
extern "C" int strncmp(const char* left, const char* right, __SIZE_TYPE__ count) {
    compares += 1;
    for (__SIZE_TYPE__ i = 0; i < count; ++i) {
        if (left[i] != right[i]) return static_cast<unsigned char>(left[i]) - static_cast<unsigned char>(right[i]);
        if (left[i] == 0) break;
    }
    return 0;
}

// The code strides entries by 8 bytes, which is one pointer on this host.
struct Entry {
    const char* name;
};

int main() {
    static const Entry act0[] = {{"town"}, {"cave"}};
    static const Entry act1[] = {{"swamp"}, {"keep"}, {"crypt"}};
    static const Entry act2[] = {{"forest"}};
    if (sizeof(Entry) != 8) return 100;
    allActs[0] = reinterpret_cast<unsigned char*>(const_cast<Entry*>(act0));
    allActs[1] = reinterpret_cast<unsigned char*>(const_cast<Entry*>(act1));
    allActs[2] = reinterpret_cast<unsigned char*>(const_cast<Entry*>(act2));
    actLevelCount[0] = 2;
    actLevelCount[1] = 3;
    actLevelCount[2] = 1;
    actCount = 3;
    if (MC_NameToAct__FPc(const_cast<char*>("town")) != 0) return 1;
    if (MC_NameToAct__FPc(const_cast<char*>("cave_extra")) != 0) return 2;
    if (MC_NameToAct__FPc(const_cast<char*>("crypt2")) != 1) return 3;
    if (MC_NameToAct__FPc(const_cast<char*>("forest")) != 2) return 4;
    if (MC_NameToAct__FPc(const_cast<char*>("fores")) != -1) return 5;
    if (MC_NameToAct__FPc(const_cast<char*>("")) != -1) return 6;
    actCount = 2;
    if (MC_NameToAct__FPc(const_cast<char*>("forest")) != -1) return 7;
    actLevelCount[1] = 1;
    compares = 0;
    if (MC_NameToAct__FPc(const_cast<char*>("keep")) != -1 || compares != 3) return 8;
    actCount = 0;
    if (MC_NameToAct__FPc(const_cast<char*>("town")) != -1) return 9;
    actCount = -2;
    if (MC_NameToAct__FPc(const_cast<char*>("town")) != 0) return 10;
    std::puts("mc name to act passed");
    return 0;
}

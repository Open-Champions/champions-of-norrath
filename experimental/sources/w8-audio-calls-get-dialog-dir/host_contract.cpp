#include "get_dialog_dir.cpp"
#include <cstdio>
#include <cstring>
extern "C" char engineCurrentLevelName[80] = "act2";
static int query_ok = 1, queries = 0, finds = 0;
static char seen_query[80], seen_find[80], seen_name[32];
static unsigned char table[160];
static unsigned char* find_result = table;
extern "C" int lumpQuery__FPc(char* n) { ++queries; std::strcpy(seen_query, n); return query_ok; }
extern "C" unsigned char* lumpFindResource__FPcT0(char* l, char* n) {
    ++finds; std::strcpy(seen_find, l); std::strcpy(seen_name, n); return find_result;
}
int main() {
    if (soundGetDialogDir__Fv() != table) return 1;
    if (std::strcmp(seen_query, "act2.lmp") != 0 || std::strcmp(seen_find, "act2.lmp") != 0) return 2;
    if (std::strcmp(seen_name, "dialog.bin") != 0 || queries != 1 || finds != 1) return 3;
    query_ok = 0;
    if (soundGetDialogDir__Fv() != nullptr || finds != 1) return 4;
    query_ok = 1; find_result = nullptr;
    if (soundGetDialogDir__Fv() != nullptr) return 5;   // a missing resource is passed through
    std::strcpy(engineCurrentLevelName, "b");
    find_result = table;
    soundGetDialogDir__Fv();
    if (std::strcmp(seen_query, "b.lmp") != 0) return 6;
    std::puts("get_dialog_dir passed");
    return 0;
}

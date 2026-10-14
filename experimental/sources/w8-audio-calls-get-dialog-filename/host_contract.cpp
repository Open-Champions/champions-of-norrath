#include "get_dialog_filename.cpp"
#include <cstdio>
#include <cstring>
extern "C" char engineCurrentLevelName[80] = "act1_level";
int main() {
    char* first = soundGetDialogFilename__Fv();
    if (std::strcmp(first, "act1_level.vat") != 0) return 1;
    std::strcpy(engineCurrentLevelName, "b");
    char* second = soundGetDialogFilename__Fv();
    if (second != first || std::strcmp(second, "b.vat") != 0) return 2;  // same static buffer
    std::puts("get_dialog_filename passed");
    return 0;
}

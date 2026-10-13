#include "get_global_dialog_dir.cpp"
#include <cstdio>
extern "C" unsigned char norrath_sound_global_dialog_map_view[12] = {};
static void* seen;
static unsigned char table[80];
static unsigned char* answer = table;
extern "C" unsigned char* get__11ResourceMap(void* map) { seen = map; return answer; }
int main() {
    if (soundGetGlobalDialogDir__Fv() != table) return 1;
    if (seen != norrath_sound_global_dialog_map_view) return 2;
    answer = nullptr;
    if (soundGetGlobalDialogDir__Fv() != nullptr) return 3;
    std::puts("get_global_dialog_dir passed");
    return 0;
}

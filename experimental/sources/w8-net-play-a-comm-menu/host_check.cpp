#include "comm_menu.cpp"
#include <cstring>
int commEnable, commEcho;
float commRecord, commThresh, commPlayback;
int main() {
    // The 16-byte entry assumes 32-bit target pointers; the host has 8-byte ones.
    if (sizeof(void*) == 4 && (sizeof(CommMenuEntry) != 16 || sizeof(commMenu) != 96)) return 1;
    if (sizeof(commMenu) != 6 * sizeof(CommMenuEntry)) return 1;
    const char* keys[5] = {"menu_commenable", "menu_commecho", "menu_commrecord", "menu_commthresh", "menu_commplay"};
    const int kinds[5] = {3, 3, 4, 4, 4};
    void* targets[5] = {&commEnable, &commEcho, &commRecord, &commThresh, &commPlayback};
    for (int i = 0; i < 5; ++i) {
        if (std::strcmp(commMenu[i].key, keys[i]) != 0) return 2;
        if (commMenu[i].kind != kinds[i] || commMenu[i].target != targets[i] || commMenu[i].reserved != 0) return 3;
    }
    if (commMenu[5].key || commMenu[5].kind || commMenu[5].target || commMenu[5].reserved) return 4;
    return 0;
}

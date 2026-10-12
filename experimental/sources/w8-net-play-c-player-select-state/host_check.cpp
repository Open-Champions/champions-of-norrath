#include "player_select_state.cpp"
#include <cstring>
int main() {
    const char* names[4] = {"menu_oneplayer", "menu_twoplayer", "menu_threeplayer", "menu_fourplayer"};
    if (sizeof(playerMenu) / sizeof(playerMenu[0]) != 5) return 1;
    for (int i = 0; i < 4; ++i) {
        if (strcmp(playerMenu[i].name, names[i])) return 2;
        for (int j = 0; j < 3; ++j) if (playerMenu[i].pad[j]) return 3;
    }
    if (playerMenu[4].name || playerMenu[4].pad[0] || playerMenu[4].pad[1] || playerMenu[4].pad[2]) return 4;
    if (sizeof playerSelectNextName != 64 || sizeof playerSelectPrevName != 64 || sizeof worldCharViewMatrix != 64) return 5;
    for (int i = 0; i < 64; ++i)
        if (playerSelectNextName[i] || playerSelectPrevName[i] || worldCharViewMatrix[i]) return 6;
    if (((unsigned long)worldCharViewMatrix & 15) != 0) return 7;
    if (playerSelectSuccessful != 1) return 8;
    if (playerRecalled || recallEnter || playerSelectCount || playerSelectStartPlayer || playerSelectEnforce) return 9;
    if (playerSelectExitMessage || syncState) return 10;
    if (sizeof playerSelectSuccessful != 1 || sizeof playerSelectExitMessage != 4 || sizeof syncState != 4) return 11;
    return 0;
}

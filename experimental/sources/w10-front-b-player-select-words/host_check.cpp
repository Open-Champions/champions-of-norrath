#include "player_select_words.cpp"
#include <cstdio>
#include <cstring>

int main() {
    const char* want[4] = {"menu_oneplayer", "menu_twoplayer", "menu_threeplayer", "menu_fourplayer"};
    int bad = sizeof(playerMenu) / sizeof(playerMenu[0]) != 5;
    for (int i = 0; i < 4; ++i)
        bad |= std::strcmp(playerMenu[i].label, want[i]) != 0 || playerMenu[i].second || playerMenu[i].third ||
               playerMenu[i].fourth;
    bad |= playerMenu[4].label != 0 || playerMenu[4].second || playerMenu[4].third || playerMenu[4].fourth;
    bad |= sizeof(playerSelectNextName) != 64 || sizeof(playerSelectPrevName) != 64;
    for (int i = 0; i < 64; ++i) bad |= playerSelectNextName[i] || playerSelectPrevName[i];
    bad |= playerRecalled || playerSelectCount || playerSelectStartPlayer || playerSelectEnforce ||
           playerSelectExitMessage;
    bad |= sizeof(playerRecalled) != 1 || sizeof(playerSelectExitMessage) != 4;
    std::printf(bad ? "FAIL\n" : "ok: menu records, name buffers and flags\n");
    return bad;
}

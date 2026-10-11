#include "main_menu.cpp"
#include <cstdio>
#include <cstring>
int main() {
    const char* labels[7] = {
        "menu_returntogame", "menu_recall", "menu_loadgame", "menu_option",
        "menu_changeplayers", "menu_kickplayers", "menu_exittomenu"
    };
    for (int index = 0; index < 7; ++index) {
        if (std::strcmp(mainMenu[index].label, labels[index]) != 0) return 1 + index;
        if (mainMenu[index].word1 != 0 || mainMenu[index].word2 != 0 || mainMenu[index].word3 != 0) return 20 + index;
    }
    if (mainMenu[7].label != 0 || mainMenu[7].word1 != 0 || mainMenu[7].word2 != 0 || mainMenu[7].word3 != 0) return 30;
    std::puts("mainMenu contract passed");
    return 0;
}

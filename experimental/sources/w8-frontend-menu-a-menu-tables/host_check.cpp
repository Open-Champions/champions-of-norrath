#include <cstdio>
#include <cstring>
#include <initializer_list>
#include "menu_tables.cpp"
int main() {
    if (sizeof(void*) == 4 && (sizeof gameTypeMenu != 48 || sizeof difficultyMenu != 64 || sizeof kickMenu != 96))
        return 1;
    if (sizeof gameTypeMenu != 3 * sizeof(norrath_menu_record) || sizeof kickMenu != 6 * sizeof(norrath_menu_record))
        return 2;
    if (std::strcmp(gameTypeMenu[0].label, "menu_normalgame") || std::strcmp(gameTypeMenu[1].label, "menu_netgame") ||
        gameTypeMenu[2].label)
        return 3;
    if (std::strcmp(difficultyMenu[0].label, "menu_easy2") || std::strcmp(difficultyMenu[1].label, "menu_normal2") ||
        std::strcmp(difficultyMenu[2].label, "menu_hard2") || difficultyMenu[3].label)
        return 4;
    if (std::strcmp(kickMenu[0].label, "menu_choosetokick") || kickMenu[0].second != 5) return 5;
    for (int i = 1; i < 6; ++i) if (kickMenu[i].label || kickMenu[i].second || kickMenu[i].third || kickMenu[i].fourth) return 6;
    for (const auto* t : {gameTypeMenu, difficultyMenu}) if (t[0].second || t[0].third || t[0].fourth) return 7;
    std::puts("menu tables check passed");
    return 0;
}

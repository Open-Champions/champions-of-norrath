#include "net_menus.cpp"

#include <cstdio>
#include <cstring>

static int failures;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d: %s\n", __LINE__, #c); ++failures; } } while (0)

static void check_menu(norrath_menu_record* menu, const char* first, const char* second) {
    CHECK(menu[0].label != 0 && std::strcmp(menu[0].label, first) == 0);
    CHECK(menu[1].label != 0 && std::strcmp(menu[1].label, second) == 0);
    CHECK(menu[2].label == 0);
    for (int i = 0; i < 3; ++i) CHECK(menu[i].kind == 0 && menu[i].value == 0 && menu[i].flags == 0);
}

int main() {
    CHECK(sizeof menuNetClientServer == 3 * sizeof(norrath_menu_record));
    check_menu(menuNetClientServer, "menu_net_client", "menu_net_server");
    check_menu(menuNetImportOrNewPlayer, "menu_net_newplayer", "menu_net_importplayer");
    check_menu(menuNetImportOrNewGame, "menu_net_newgame", "menu_net_importgame");
    if (failures == 0) std::printf("net_menus ok\n");
    return failures != 0;
}

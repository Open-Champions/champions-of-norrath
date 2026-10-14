// Host contract for frontEndStartInNetMenu. Not an EE plan input.
#include "front_end_net_menu.cpp"

extern "C" {
int isNetGame = 7;
int menuNm = 7;
unsigned char* menu = nullptr;
unsigned char menuNetClientServer[48];
unsigned char netGameLaunchInfo[328];
}

static int memset_calls = 0;
static void* memset_dest = nullptr;
static int memset_value = -1;
static unsigned memset_count = 0;
static unsigned char* menu_during = nullptr;
static int menu_nm_during = 0;
static int net_during = 0;

extern "C" void* memset(void* destination, int value, unsigned count) {
    ++memset_calls;
    memset_dest = destination;
    memset_value = value;
    memset_count = count;
    menu_during = menu;
    menu_nm_during = menuNm;
    net_during = isNetGame;
    return destination;
}

int main() {
    frontEndStartInNetMenu__Fv();
    if (isNetGame != 1 || menuNm != 2) return 1;
    if (menu != menuNetClientServer) return 2;
    if (memset_calls != 1 || memset_dest != netGameLaunchInfo) return 3;
    if (memset_value != 0 || memset_count != 328u) return 4;
    if (menu_during != menuNetClientServer || menu_nm_during != 2 || net_during != 1) return 5;
    return 0;
}

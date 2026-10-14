#include <cstdint>
#include <csetjmp>
#define main norrath_main
#include "../../experimental/sources/runtime-main/main.cpp"
#undef main

static std::jmp_buf stop_jump;
static int stop_code;
static int mode;
static int game_inits, fronts, menus, frees, loops, clears, logos, colors, tasks, prints;
static unsigned printf_left[2], printf_right[2];
static int printfs;

char levelName[322];
alignas(4) unsigned char pad[16];
char* defaultFont;
char* mediumFont;
char* smallFont;
char* haloTexture1;
char* haloTexture2;
char* haloTexture3;
char* haloTexture4;
char* haloTexture5;
char* haloTexture6;
char* haloTexture7;
char* haloTexture8;
int engineDisplayDisable;
int skipIntro;
int worldWaterBumpSpeed;
unsigned char netNetworkMode;
unsigned char fromSRS;
unsigned char fromSRSButEnteringGame;
unsigned char netInitState;
unsigned char coreLaunchSRS;

char norrath_startup_core_source_name[] = "site";
char norrath_startup_fx_lump[] = "lump";
char norrath_startup_net_argument[] = "arg1";
char norrath_startup_halo2_tex[] = "h2";
char norrath_startup_halo3_tex[] = "h3";
char norrath_startup_halo4_tex[] = "h4";
char norrath_startup_wisphalo_tex[] = "h5";
char norrath_startup_halo5_tex[] = "h6";
char norrath_startup_halo7_tex[] = "h7";
char norrath_startup_post_mc_memory_text[] = "text-1";
char norrath_startup_initial_level_name[9] = {11, 22, 33, 44, 55, 66, 77, 88, 99};
char norrath_startup_warp_menu_task_text[] = "task";
char norrath_startup_mouth_lump[] = "load-a";
char norrath_startup_font_lump[] = "load-b";
char norrath_startup_gamefont_a[] = "font-a";
char norrath_startup_gamefont_medium[] = "font-b";
char norrath_startup_gamefont_small[] = "font-c";
char norrath_startup_gtext_lump[] = "load-c";
char norrath_startup_interface_lump[] = "load-d";
char norrath_startup_engine_lump[] = "halo-a";
char norrath_startup_halo1_tex[] = "halo-b";
char norrath_startup_halo8_tex[] = "h8";
char norrath_startup_pre_main_loop_memory_text[] = "text-2";
char norrath_startup_main_menu_argument[] = "arg2";

static char marker_a[] = "match-first";
static char marker_b[] = "match-second";
static char* args_both[] = {marker_a, marker_a, marker_b};
static char* args_second[] = {marker_a, marker_b};
static char* args_null[] = {marker_a, nullptr};
static char* args_one[] = {marker_a};
static int arg_count;
static char** arg_values;
static int sif_calls;
static int find_calls;
static int load_calls;
static int lock_calls;
static char* resources[11];

static void fail(int code) {
    stop_code = code;
    std::longjmp(stop_jump, 1);
}

extern "C" void __main() {}
extern "C" void engineInit__Ff(float value) {
    unsigned bits = 0;
    __builtin_memcpy(&bits, &value, sizeof bits);
    if (bits != 0x3f7fffffu) fail(10);
}
extern "C" void _engineCheckStack__FPCci(const char* site, int line) {
    if (site != norrath_startup_core_source_name) fail(11);
    if (line != 2648 && line != 2950) fail(12);
}
extern "C" void cutInitNet__Fv() {
    if (levelName[0] != 0 || levelName[81] != 0 || levelName[242] != 0) fail(14);
}
extern "C" void commInitNet__Fv() {}
extern "C" void DLG_InitNet__Fv() {}
extern "C" void hudInitNet__Fv() {}
extern "C" void inventoryInitNet__Fv() {}
extern "C" void textInitNet__Fv() {}
extern "C" void shopInitNet__Fv() {}
extern "C" void picInit__4itemv() {}
extern "C" void MC_Init__Fi(int value) { if (value != 0) fail(15); }
extern "C" void MC_Configure__Fv() {}
extern "C" int sceSifQueryMaxFreeMemSize() {
    int which = sif_calls++;
    return which == 0 ? 4096 : -2048;
}
extern "C" int sceSifQueryTotalFreeMemSize() {
    int which = sif_calls++;
    return which == 1 ? 8192 : 1024;
}
extern "C" void scePrintf(char* text, unsigned left, unsigned right) {
    if (printfs == 0 && text != norrath_startup_post_mc_memory_text) fail(16);
    if (printfs == 1 && text != norrath_startup_pre_main_loop_memory_text) fail(17);
    if (printfs > 1) fail(18);
    printf_left[printfs] = left;
    printf_right[printfs] = right;
    ++printfs;
}
extern "C" void DLG_Init__Fv() {}
extern "C" void gameobjInit__Fv() {}
extern "C" void gameBoot__Fv() {}
extern "C" void gameInit__Fv() {
    ++game_inits;
    if (mode == 7 && game_inits == 3) {
        netInitState = 1;
        coreLaunchSRS = 1;
    }
}
extern "C" void debugCheckForPauseMenuStart__Fv() {}
extern "C" void engineAddTask__FPFv_viPci(void (*callback)(), int second, char* third, int fourth) {
    if (callback != debugCheckForPauseMenuStart__Fv || second != 110 ||
        third != norrath_startup_warp_menu_task_text || fourth != 0) fail(19);
    for (int i = 0; i < 9; ++i) {
        if (static_cast<unsigned char>(levelName[i]) != norrath_startup_initial_level_name[i]) fail(20);
    }
    if (static_cast<unsigned char>(levelName[80]) != 0xffu || levelName[81] != 0 || levelName[242] != 0) fail(21);
    ++tasks;
}
extern "C" void enginePrintTasks__Fv() { ++prints; }
extern "C" void lumpLoad__FPc(char* name) {
    char* expected[] = {
        norrath_startup_mouth_lump, norrath_startup_font_lump, norrath_startup_gtext_lump,
        norrath_startup_interface_lump, norrath_startup_fx_lump,
    };
    if (load_calls >= 5 || name != expected[load_calls]) fail(22);
    ++load_calls;
}
extern "C" char* lumpFindResource__FPcT0(char* lump, char* name) {
    struct Pair { char* lump; char* name; };
    static const Pair expected[] = {
        {norrath_startup_font_lump, norrath_startup_gamefont_a},
        {norrath_startup_font_lump, norrath_startup_gamefont_medium},
        {norrath_startup_font_lump, norrath_startup_gamefont_small},
        {norrath_startup_engine_lump, norrath_startup_halo1_tex},
        {norrath_startup_fx_lump, norrath_startup_halo2_tex},
        {norrath_startup_fx_lump, norrath_startup_halo3_tex},
        {norrath_startup_fx_lump, norrath_startup_halo4_tex},
        {norrath_startup_fx_lump, norrath_startup_wisphalo_tex},
        {norrath_startup_fx_lump, norrath_startup_halo5_tex},
        {norrath_startup_fx_lump, norrath_startup_halo7_tex},
        {norrath_startup_fx_lump, norrath_startup_halo8_tex},
    };
    if (find_calls >= 11 || lump != expected[find_calls].lump || name != expected[find_calls].name) fail(23);
    char* result = resources[find_calls];
    ++find_calls;
    return result;
}
extern "C" void lumpLock__Fi(int value) {
    if ((lock_calls == 0 && value != 100) || (lock_calls == 1 && value != 50) || lock_calls > 1) fail(24);
    ++lock_calls;
}
extern "C" void engineSetScreenColor__Fiiii(int red, int green, int blue, int alpha) {
    if (red != -128 || green != -128 || blue != -128 || alpha != 0) fail(25);
    if (engineDisplayDisable != 4) fail(26);
    ++colors;
}
extern "C" void displaySOELogo__Fv() {}
extern "C" void displaySBLogoScreen__Fv() {}
extern "C" void mcProcess__Fv() {}
extern "C" void simpleDrawLoop__FPFv_i(void (*callback)()) {
    if (logos == 0 && (callback != displaySOELogo__Fv || engineDisplayDisable != 4)) fail(27);
    if (logos == 1 && skipIntro == 0 && callback != displaySBLogoScreen__Fv) fail(28);
    if (logos == 1 && skipIntro != 0 && callback != mcProcess__Fv) fail(29);
    if (logos == 2 && callback != mcProcess__Fv) fail(30);
    if (logos > 2) fail(31);
    ++logos;
}
extern "C" int strcmp(const char* left, const char* right) {
    if (left == norrath_startup_net_argument && right == marker_a) return 0;
    if (left == norrath_startup_net_argument && right == nullptr && mode == 6) return 0;
    if (left == norrath_startup_main_menu_argument && right == marker_b) return 0;
    return 1;
}
extern "C" void initChars__FiPPc(int argc, char** argv) {
    if (argc != arg_count || argv != arg_values) fail(32);
    if (defaultFont != resources[0] || mediumFont != resources[1] || smallFont != resources[2]) fail(33);
    char* halos[] = {haloTexture1, haloTexture2, haloTexture3, haloTexture4,
                     haloTexture5, haloTexture6, haloTexture7, haloTexture8};
    for (int i = 0; i < 8; ++i) if (halos[i] != resources[3 + i]) fail(34);
    if (engineDisplayDisable != 5) fail(35);
    if (tasks != 1 || prints != 1 || printfs != 2) fail(36);
    if (printf_left[0] != 4u || printf_right[0] != 8u) fail(37);
    if (printf_left[1] != 0x3ffffeu || printf_right[1] != 1u) fail(38);
}
extern "C" void lumpClear__Fi(int value) {
    if (value != 5) fail(39);
    ++clears;
}
extern "C" void frontEndInit__Fi(int value) {
    if (value != 0) fail(40);
    ++fronts;
}
extern "C" void frontEndStartInNetMenu__Fv() { ++menus; }
extern "C" void gameLoop__Fv() { ++loops; }
extern "C" void netFree__Fb(int value) {
    if (value != 0) fail(41);
    ++frees;
}
extern "C" void launchSRS__Fv() {
    if (mode == 7 && (game_inits != 3 || clears != 2 || loops != 2 || frees != 1)) fail(42);
    fail(1);
}

static void reset() {
    __builtin_memset(levelName, 0x5a, sizeof levelName);
    __builtin_memset(pad, 0, sizeof pad);
    defaultFont = mediumFont = smallFont = nullptr;
    haloTexture1 = haloTexture2 = haloTexture3 = haloTexture4 = nullptr;
    haloTexture5 = haloTexture6 = haloTexture7 = haloTexture8 = nullptr;
    engineDisplayDisable = skipIntro = worldWaterBumpSpeed = 0;
    netNetworkMode = fromSRS = fromSRSButEnteringGame = netInitState = coreLaunchSRS = 0;
    game_inits = fronts = menus = frees = loops = clears = logos = colors = tasks = prints = printfs = 0;
    sif_calls = find_calls = load_calls = lock_calls = 0;
    for (int i = 0; i < 11; ++i) resources[i] = reinterpret_cast<char*>(static_cast<std::uintptr_t>(0x1000 + i));
}

static int run() {
    stop_code = 0;
    if (setjmp(stop_jump) == 0) norrath_main(arg_count, arg_values);
    return stop_code;
}

int main() {
    reset();
    mode = 1;
    arg_count = 3;
    arg_values = args_both;
    *reinterpret_cast<unsigned*>(pad + 8) = 16398u;
    netInitState = 1;
    coreLaunchSRS = 1;
    if (run() != 1 || fronts != 1 || menus != 0 || frees != 1 || colors != 2 || logos != 3) return 2;
    if (worldWaterBumpSpeed != 1 || netNetworkMode != 1 || fromSRS != 0) return 3;

    reset();
    mode = 2;
    arg_count = 1;
    arg_values = args_one;
    skipIntro = 1;
    fromSRSButEnteringGame = 1;
    fromSRS = 1;
    coreLaunchSRS = 1;
    if (run() != 1 || fronts != 1 || menus != 0 || frees != 0 || colors != 1 || logos != 2) return 4;
    if (worldWaterBumpSpeed != 0 || fromSRSButEnteringGame != 0 || fromSRS != 1) return 5;

    reset();
    mode = 3;
    arg_count = 1;
    arg_values = args_one;
    skipIntro = 1;
    fromSRS = 1;
    coreLaunchSRS = 1;
    if (run() != 1 || fronts != 1 || menus != 1 || fromSRS != 0) return 6;

    reset();
    mode = 4;
    arg_count = 1;
    arg_values = args_one;
    skipIntro = 1;
    netNetworkMode = 1;
    coreLaunchSRS = 1;
    if (run() != 1 || fronts != 0 || menus != 0) return 7;

    reset();
    mode = 5;
    arg_count = 2;
    arg_values = args_second;
    skipIntro = 1;
    coreLaunchSRS = 1;
    if (run() != 1 || fronts != 2 || menus != 0 || netNetworkMode != 0) return 8;

    reset();
    mode = 6;
    arg_count = 2;
    arg_values = args_null;
    skipIntro = 1;
    coreLaunchSRS = 1;
    if (run() != 1 || netNetworkMode != 0 || fronts != 1 || menus != 0) return 9;

    reset();
    mode = 7;
    arg_count = 1;
    arg_values = args_one;
    skipIntro = 1;
    if (run() != 1 || fronts != 2 || menus != 0) return 50;
    return 0;
}

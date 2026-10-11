// Observed main at 0x002d0818, 1140 bytes. It does not return.
// The text arguments are the named objects in startup_text.cpp. Those
// addresses have no ELF symbol. engineAddTask is declared and not defined.
extern "C" {
void __main();
void engineInit__Ff(float value);
void _engineCheckStack__FPCci(const char* site, int line);
void cutInitNet__Fv();
void commInitNet__Fv();
void DLG_InitNet__Fv();
void hudInitNet__Fv();
void inventoryInitNet__Fv();
void textInitNet__Fv();
void shopInitNet__Fv();
void picInit__4itemv();
void MC_Init__Fi(int value);
void MC_Configure__Fv();
int sceSifQueryMaxFreeMemSize();
int sceSifQueryTotalFreeMemSize();
void scePrintf(char* text, unsigned left, unsigned right);
void DLG_Init__Fv();
void gameobjInit__Fv();
void gameBoot__Fv();
void gameInit__Fv();
void debugCheckForPauseMenuStart__Fv();
void engineAddTask__FPFv_viPci(void (*callback)(), int second, char* third, int fourth);
void enginePrintTasks__Fv();
void lumpLoad__FPc(char* name);
char* lumpFindResource__FPcT0(char* lump, char* name);
void lumpLock__Fi(int value);
void engineSetScreenColor__Fiiii(int red, int green, int blue, int alpha);
void displaySOELogo__Fv();
void displaySBLogoScreen__Fv();
void mcProcess__Fv();
void simpleDrawLoop__FPFv_i(void (*callback)());
int strcmp(const char* left, const char* right);
void initChars__FiPPc(int argc, char** argv);
void lumpClear__Fi(int value);
void frontEndInit__Fi(int value);
void frontEndStartInNetMenu__Fv();
void gameLoop__Fv();
void netFree__Fb(int value);
void launchSRS__Fv();

extern char levelName[];
extern unsigned char pad[];
extern char* defaultFont;
extern char* mediumFont;
extern char* smallFont;
extern char* haloTexture1;
extern char* haloTexture2;
extern char* haloTexture3;
extern char* haloTexture4;
extern char* haloTexture5;
extern char* haloTexture6;
extern char* haloTexture7;
extern char* haloTexture8;
extern int engineDisplayDisable;
extern int skipIntro;
extern int worldWaterBumpSpeed;
extern unsigned char netNetworkMode;
extern unsigned char fromSRS;
extern unsigned char fromSRSButEnteringGame;
extern unsigned char netInitState;
extern unsigned char coreLaunchSRS;

extern char norrath_startup_core_source_name[];
extern char norrath_startup_fx_lump[];
extern char norrath_startup_net_argument[];
extern char norrath_startup_halo2_tex[];
extern char norrath_startup_halo3_tex[];
extern char norrath_startup_halo4_tex[];
extern char norrath_startup_wisphalo_tex[];
extern char norrath_startup_halo5_tex[];
extern char norrath_startup_halo7_tex[];
extern char norrath_startup_post_mc_memory_text[];
extern char norrath_startup_initial_level_name[];
extern char norrath_startup_warp_menu_task_text[];
extern char norrath_startup_mouth_lump[];
extern char norrath_startup_font_lump[];
extern char norrath_startup_gamefont_a[];
extern char norrath_startup_gamefont_medium[];
extern char norrath_startup_gamefont_small[];
extern char norrath_startup_gtext_lump[];
extern char norrath_startup_interface_lump[];
extern char norrath_startup_engine_lump[];
extern char norrath_startup_halo1_tex[];
extern char norrath_startup_halo8_tex[];
extern char norrath_startup_pre_main_loop_memory_text[];
extern char norrath_startup_main_menu_argument[];

int main(int argc, char** argv) {
    union {
        unsigned bits;
        float value;
    } literal;
    literal.bits = 0x3f7fffffu;
    __main();
    engineInit__Ff(literal.value);
    _engineCheckStack__FPCci(norrath_startup_core_source_name, 2648);
    levelName[0] = 0;
    levelName[81] = 0;
    levelName[242] = 0;
    cutInitNet__Fv();
    commInitNet__Fv();
    DLG_InitNet__Fv();
    hudInitNet__Fv();
    inventoryInitNet__Fv();
    textInitNet__Fv();
    shopInitNet__Fv();
    picInit__4itemv();
    MC_Init__Fi(0);
    MC_Configure__Fv();
    unsigned max_shifted = static_cast<unsigned>(sceSifQueryMaxFreeMemSize()) >> 10;
    unsigned total_shifted = static_cast<unsigned>(sceSifQueryTotalFreeMemSize()) >> 10;
    scePrintf(norrath_startup_post_mc_memory_text, max_shifted, total_shifted);
    DLG_Init__Fv();
    gameobjInit__Fv();
    gameBoot__Fv();
    gameInit__Fv();
    volatile unsigned char* copied = reinterpret_cast<volatile unsigned char*>(levelName);
    const volatile unsigned char* source =
        reinterpret_cast<const volatile unsigned char*>(norrath_startup_initial_level_name);
    for (int byte = 0; byte < 9; ++byte) copied[byte] = source[byte];
    copied[80] = 0xffu;
    engineAddTask__FPFv_viPci(debugCheckForPauseMenuStart__Fv, 110, norrath_startup_warp_menu_task_text, 0);
    enginePrintTasks__Fv();
    lumpLoad__FPc(norrath_startup_mouth_lump);
    lumpLoad__FPc(norrath_startup_font_lump);
    defaultFont = lumpFindResource__FPcT0(norrath_startup_font_lump, norrath_startup_gamefont_a);
    mediumFont = lumpFindResource__FPcT0(norrath_startup_font_lump, norrath_startup_gamefont_medium);
    smallFont = lumpFindResource__FPcT0(norrath_startup_font_lump, norrath_startup_gamefont_small);
    lumpLoad__FPc(norrath_startup_gtext_lump);
    lumpLoad__FPc(norrath_startup_interface_lump);
    lumpLock__Fi(100);
    lumpLoad__FPc(norrath_startup_fx_lump);
    lumpLock__Fi(50);
    haloTexture1 = lumpFindResource__FPcT0(norrath_startup_engine_lump, norrath_startup_halo1_tex);
    haloTexture2 = lumpFindResource__FPcT0(norrath_startup_fx_lump, norrath_startup_halo2_tex);
    haloTexture3 = lumpFindResource__FPcT0(norrath_startup_fx_lump, norrath_startup_halo3_tex);
    haloTexture4 = lumpFindResource__FPcT0(norrath_startup_fx_lump, norrath_startup_halo4_tex);
    haloTexture5 = lumpFindResource__FPcT0(norrath_startup_fx_lump, norrath_startup_wisphalo_tex);
    haloTexture6 = lumpFindResource__FPcT0(norrath_startup_fx_lump, norrath_startup_halo5_tex);
    haloTexture7 = lumpFindResource__FPcT0(norrath_startup_fx_lump, norrath_startup_halo7_tex);
    haloTexture8 = lumpFindResource__FPcT0(norrath_startup_fx_lump, norrath_startup_halo8_tex);
    engineDisplayDisable = 4;
    engineSetScreenColor__Fiiii(-128, -128, -128, 0);
    simpleDrawLoop__FPFv_i(displaySOELogo__Fv);
    engineDisplayDisable = 4;
    if (skipIntro == 0) {
        engineSetScreenColor__Fiiii(-128, -128, -128, 0);
        simpleDrawLoop__FPFv_i(displaySBLogoScreen__Fv);
    }
    MC_Configure__Fv();
    simpleDrawLoop__FPFv_i(mcProcess__Fv);
    engineDisplayDisable = 5;
    if (*reinterpret_cast<unsigned*>(pad + 8) == 16398u) worldWaterBumpSpeed = 1;
    char* first_match = 0;
    if (argc > 1) {
        for (int index = 1; index < argc; ++index) {
            if (strcmp(norrath_startup_net_argument, argv[index]) == 0) {
                first_match = argv[index];
                break;
            }
        }
    }
    if (first_match != 0) netNetworkMode = 1;
    max_shifted = static_cast<unsigned>(sceSifQueryMaxFreeMemSize()) >> 10;
    total_shifted = static_cast<unsigned>(sceSifQueryTotalFreeMemSize()) >> 10;
    scePrintf(norrath_startup_pre_main_loop_memory_text, max_shifted, total_shifted);
    _engineCheckStack__FPCci(norrath_startup_core_source_name, 2950);
    initChars__FiPPc(argc, argv);
    for (;;) {
        gameInit__Fv();
        lumpClear__Fi(5);
        char* second_match = 0;
        if (argc > 1) {
            for (int index = 1; index < argc; ++index) {
                if (strcmp(norrath_startup_main_menu_argument, argv[index]) == 0) {
                    second_match = argv[index];
                    break;
                }
            }
        }
        if (second_match != 0) frontEndInit__Fi(0);
        if (netNetworkMode == 0) {
            frontEndInit__Fi(0);
            if (fromSRSButEnteringGame != 0) {
                fromSRSButEnteringGame = 0;
            } else if (fromSRS != 0) {
                frontEndStartInNetMenu__Fv();
                fromSRS = 0;
            }
        }
        gameLoop__Fv();
        if (netInitState != 0) netFree__Fb(0);
        if (coreLaunchSRS == 0) continue;
        launchSRS__Fv();
    }
}
}

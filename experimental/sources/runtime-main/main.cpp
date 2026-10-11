// Observed main at 0x002d0818, 1140 bytes. It does not return.
// Addresses passed to printf, lump, strcmp, and the stack check have no ELF
// symbol. They stay separate unresolved objects; their bytes are not copied.
// engineAddTask is declared and not defined.
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

extern char norrath_main_sdata_004866a0[];
extern char norrath_main_sdata_004867d0[];
extern char norrath_main_sdata_004867d8[];
extern char norrath_main_rodata_0047afe8[];
extern char norrath_main_rodata_0047aff8[];
extern char norrath_main_rodata_0047b008[];
extern char norrath_main_rodata_0047b018[];
extern char norrath_main_rodata_0047b028[];
extern char norrath_main_rodata_0047b038[];
extern char norrath_main_rodata_0047b048[];
extern char norrath_main_rodata_0047b058[];
extern char norrath_main_rodata_0047b068[];
extern char norrath_main_rodata_0047b078[];
extern char norrath_main_rodata_0047b088[];
extern char norrath_main_rodata_0047b098[];
extern char norrath_main_rodata_0047b0a8[];
extern char norrath_main_rodata_0047b0b8[];
extern char norrath_main_rodata_0047b0d0[];
extern char norrath_main_rodata_0047b0e0[];
extern char norrath_main_rodata_0047b0f0[];
extern char norrath_main_rodata_0047b100[];
extern char norrath_main_rodata_0047b110[];
extern char norrath_main_rodata_0047b120[];
extern char norrath_main_rodata_0047b138[];

int main(int argc, char** argv) {
    union {
        unsigned bits;
        float value;
    } literal;
    literal.bits = 0x3f7fffffu;
    __main();
    engineInit__Ff(literal.value);
    _engineCheckStack__FPCci(norrath_main_sdata_004866a0, 2648);
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
    scePrintf(norrath_main_rodata_0047b048, max_shifted, total_shifted);
    DLG_Init__Fv();
    gameobjInit__Fv();
    gameBoot__Fv();
    gameInit__Fv();
    volatile unsigned char* copied = reinterpret_cast<volatile unsigned char*>(levelName);
    const volatile unsigned char* source =
        reinterpret_cast<const volatile unsigned char*>(norrath_main_rodata_0047b058);
    for (int byte = 0; byte < 9; ++byte) copied[byte] = source[byte];
    copied[80] = 0xffu;
    engineAddTask__FPFv_viPci(debugCheckForPauseMenuStart__Fv, 110, norrath_main_rodata_0047b068, 0);
    enginePrintTasks__Fv();
    lumpLoad__FPc(norrath_main_rodata_0047b078);
    lumpLoad__FPc(norrath_main_rodata_0047b088);
    defaultFont = lumpFindResource__FPcT0(norrath_main_rodata_0047b088, norrath_main_rodata_0047b098);
    mediumFont = lumpFindResource__FPcT0(norrath_main_rodata_0047b088, norrath_main_rodata_0047b0a8);
    smallFont = lumpFindResource__FPcT0(norrath_main_rodata_0047b088, norrath_main_rodata_0047b0b8);
    lumpLoad__FPc(norrath_main_rodata_0047b0d0);
    lumpLoad__FPc(norrath_main_rodata_0047b0e0);
    lumpLock__Fi(100);
    lumpLoad__FPc(norrath_main_sdata_004867d0);
    lumpLock__Fi(50);
    haloTexture1 = lumpFindResource__FPcT0(norrath_main_rodata_0047b0f0, norrath_main_rodata_0047b100);
    haloTexture2 = lumpFindResource__FPcT0(norrath_main_sdata_004867d0, norrath_main_rodata_0047afe8);
    haloTexture3 = lumpFindResource__FPcT0(norrath_main_sdata_004867d0, norrath_main_rodata_0047aff8);
    haloTexture4 = lumpFindResource__FPcT0(norrath_main_sdata_004867d0, norrath_main_rodata_0047b008);
    haloTexture5 = lumpFindResource__FPcT0(norrath_main_sdata_004867d0, norrath_main_rodata_0047b018);
    haloTexture6 = lumpFindResource__FPcT0(norrath_main_sdata_004867d0, norrath_main_rodata_0047b028);
    haloTexture7 = lumpFindResource__FPcT0(norrath_main_sdata_004867d0, norrath_main_rodata_0047b038);
    haloTexture8 = lumpFindResource__FPcT0(norrath_main_sdata_004867d0, norrath_main_rodata_0047b110);
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
            if (strcmp(norrath_main_sdata_004867d8, argv[index]) == 0) {
                first_match = argv[index];
                break;
            }
        }
    }
    if (first_match != 0) netNetworkMode = 1;
    max_shifted = static_cast<unsigned>(sceSifQueryMaxFreeMemSize()) >> 10;
    total_shifted = static_cast<unsigned>(sceSifQueryTotalFreeMemSize()) >> 10;
    scePrintf(norrath_main_rodata_0047b120, max_shifted, total_shifted);
    _engineCheckStack__FPCci(norrath_main_sdata_004866a0, 2950);
    initChars__FiPPc(argc, argv);
    for (;;) {
        gameInit__Fv();
        lumpClear__Fi(5);
        char* second_match = 0;
        if (argc > 1) {
            for (int index = 1; index < argc; ++index) {
                if (strcmp(norrath_main_rodata_0047b138, argv[index]) == 0) {
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

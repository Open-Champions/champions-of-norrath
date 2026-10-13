// Observed: Init__9VIStationiPCcPcT3T2T2i at 0x010179c8 is 1260 bytes. Arguments after
// the object: an int mode, a const char* A, a char* B, a char* C, a const char* D, a
// const char* E and an int F. Byte offsets are from the object; the object is larger
// than 97 KiB. It returns 0 on success and -1 on failure; the text it prints is shown
// quoted, and every failure path after the raster step returns -1.
//  1. word 0 = mode; word 0x17a30 = 0; strcpy(this + 4, A); word 0xb81c = 0;
//     printf("station::init()\n"); sceGsSyncV(0) twice.
//  2. strcpy(this + 1028, B); strcpy(this + 1284, C); printf("%s, %s\n", B copy, C copy)
//     (the two copied buffers); sceGsSyncV(0) twice.
//  3. memset 64 bytes at 0x17a34 and at 0x17a74 to zero; strcpy D into 0x17a34 if D is
//     nonzero; strcpy E into 0x17a74 if E is nonzero; word 0x17ab4 = F.
//  4. Init__5VIPadii(this + 0xb470, 0, 0); Init__10VIKeyboard(this + 0xb4d0); word
//     0xb468 = 1; printf("Pad/Kb init\n"); sceGsSyncV(0) twice.
//  5. Init__8VIRasterPviii(this + 1552, 0, 640, 448, 0); a negative result prints
//     "raster init failed\n" and returns -1. Otherwise printf("Raster Init\n"), sceGsSyncV
//     twice, VIDisplayLoadingScreen__Fb(0), printf("Loading Screen\n"), sceGsSyncV twice.
//  6. Each of these is tested for a negative result (return -1, no message):
//     Init__9VICollide(this + 21104); Init__12VIDictionaryi(this + 22160, 0);
//     Init__13VISoundDevice(this + 22184); Init__7VIScene...(this + 26672,
//     GetTime__7VITimer(), dictionary, raster, sound device, collide);
//     Init__4VIUIP8VIRasterP13VISoundDeviceP12VIDictionary(this + 0xb3f0, raster,
//     sound device, dictionary).
//  7. VISetWindowUI__FP4VIUI(ui); printf("Here, ui = %i\n", ui). A VIESFParse and a
//     VICSFFile are built on the stack; VICSFFile::Parse is called with the paths
//     "data2\\ArialUI.csf", "data2\\ui.csf", "data2\\statui.csf", "data2\\pregame.csf"
//     and "data2\\conui.csf", each with the parser and the scene (this + 26672). The first
//     negative result destroys the parser (_$_10VIESFParse(parser, 2)) and returns -1; the
//     file object is never destroyed.
//  8. SetTimeOfDay__7VIScenef(scene, 12.0f); SetWireframe__8VIRasteri(raster, 0);
//     SetClearColor__8VIRasterRC10VIColor32F(raster, {0, 0, 0, 1.0f}); word 0xb794 = 0;
//     Init__15VIWindowManagerP5VIPadP10VIKeyboard(this + 0xb798, pad, keyboard);
//     SetAppPtr__5VIWndPv(this + 0xb820, this); SetStationName__20VIWndGenericRendererPCc
//     (renderer, B); SetPassword__20VIWndGenericRendererPCc(renderer, C);
//     Init__20VIWndGenericRenderer(renderer).
//  9. A 2048-byte halfword buffer is cleared and filled by snprintf16__8VIStringPUsiPCce
//     (buffer, 2048, "Connecting to SOEGames.net..."); SetAppPtr__5VIWndPv(this + 0x15408,
//     this); Init__9VIWndEula; SetText__9VIWndEulaPCUsi(wnd, buffer,
//     strlen16__8VIStringPCUs(buffer)); SetRoot__15VIWindowManagerP5VIWnd(manager,
//     renderer); SetFocus__5VIWndP5VIWndb(manager, renderer, true).
// 10. Init__7VIFaderP8VIRasterP12VIDictionaryf(this + 0xb448, raster, dictionary, 1.0f);
//     Begin__7VIFaderff(fader, 0.0f, 0.5f); _$_10VIESFParse(parser, 2); return 0.
// Inferred: the buffer after "station::init" is the start-up/connect page; the object
// groups pad, keyboard, raster, collide, dictionary, sound device, scene, UI, window
// manager, renderer, EULA window and fader by the member names of the called routines.
// Unknown: the field meanings of the words at 0, 0xb81c, 0x17a30, 0xb468, 0xb794, 0x17ab4
// and the sizes of the parser (at most 1072 bytes) and file (at most 16 bytes) objects.
extern "C" {
void* strcpy(char* destination, const char* source);
void* memset(void* destination, int value, unsigned count);
int printf(const char* format, ...);
int sceGsSyncV(int mode);
long long GetTime__7VITimer();
int VIDisplayLoadingScreen__Fb(bool flag);

int Init__5VIPadii(void* pad, int a, int b);
int Init__10VIKeyboard(void* keyboard);
int Init__8VIRasterPviii(void* raster, void* arg, int width, int height, int flags);
int Init__9VICollide(void* collide);
int Init__12VIDictionaryi(void* dictionary, int value);
int Init__13VISoundDevice(void* device);
int Init__7VIScenelR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(void* scene, long time, void* dictionary, void* raster, void* device, void* collide);
int Init__4VIUIP8VIRasterP13VISoundDeviceP12VIDictionary(void* ui, void* raster, void* device, void* dictionary);
void VISetWindowUI__FP4VIUI(void* ui);
void __10VIESFParse(void* parser);
void _$_10VIESFParse(void* parser, int flags);
void __9VICSFFile(void* file);
int Parse__9VICSFFilePCcR10VIESFParseR7VIScene(void* file, const char* path, void* parser, void* scene);
void SetTimeOfDay__7VIScenef(void* scene, float time);
void SetWireframe__8VIRasteri(void* raster, int mode);
void SetClearColor__8VIRasterRC10VIColor32F(void* raster, const float* color);
void Init__15VIWindowManagerP5VIPadP10VIKeyboard(void* manager, void* pad, void* keyboard);
void SetAppPtr__5VIWndPv(void* window, void* app);
void SetStationName__20VIWndGenericRendererPCc(void* renderer, const char* name);
void SetPassword__20VIWndGenericRendererPCc(void* renderer, const char* password);
void Init__20VIWndGenericRenderer(void* renderer);
int snprintf16__8VIStringPUsiPCce(unsigned short* buffer, int size, const char* format, ...);
void Init__9VIWndEula(void* window);
int strlen16__8VIStringPCUs(const unsigned short* text);
void SetText__9VIWndEulaPCUsi(void* window, const unsigned short* text, int length);
void SetRoot__15VIWindowManagerP5VIWnd(void* manager, void* window);
void SetFocus__5VIWndP5VIWndb(void* window, void* target, bool flag);
void Init__7VIFaderP8VIRasterP12VIDictionaryf(void* fader, void* raster, void* dictionary, float value);
void Begin__7VIFaderff(void* fader, float a, float b);

int Init__9VIStationiPCcPcT3T2T2i(void* object, int mode, const char* a, char* b, char* c, const char* d, const char* e, int f) {
    char* base = static_cast<char*>(object);
    alignas(16) char parser[1072];
    alignas(16) char file[16];
    alignas(16) float color[4];
    alignas(16) unsigned short text[1024];
    char* name_b = base + 1028;
    char* name_c = base + 1284;
    char* hint_d = base + 0x17a34;
    char* hint_e = base + 0x17a74;
    void* raster = base + 1552;
    void* collide = base + 21104;
    void* dictionary = base + 22160;
    void* device = base + 22184;
    void* scene = base + 26672;
    void* ui = base + 0xb3f0;
    void* pad = base + 0xb470;
    void* keyboard = base + 0xb4d0;
    void* manager = base + 0xb798;
    void* renderer = base + 0xb820;
    void* eula = base + 0x15408;
    void* fader = base + 0xb448;
    *reinterpret_cast<int*>(base) = mode;
    *reinterpret_cast<int*>(base + 0x17a30) = 0;
    strcpy(base + 4, a);
    *reinterpret_cast<int*>(base + 0xb81c) = 0;
    printf("station::init()\n");
    sceGsSyncV(0);
    sceGsSyncV(0);
    strcpy(name_b, b);
    strcpy(name_c, c);
    printf("%s, %s\n", name_b, name_c);
    sceGsSyncV(0);
    sceGsSyncV(0);
    memset(hint_d, 0, 64);
    memset(hint_e, 0, 64);
    if (d != nullptr) strcpy(hint_d, d);
    if (e != nullptr) strcpy(hint_e, e);
    *reinterpret_cast<int*>(base + 0x17ab4) = f;
    Init__5VIPadii(pad, 0, 0);
    Init__10VIKeyboard(keyboard);
    *reinterpret_cast<int*>(base + 0xb468) = 1;
    printf("Pad/Kb init\n");
    sceGsSyncV(0);
    sceGsSyncV(0);
    if (Init__8VIRasterPviii(raster, nullptr, 640, 448, 0) < 0) {
        printf("raster init failed\n");
        return -1;
    }
    printf("Raster Init\n");
    sceGsSyncV(0);
    sceGsSyncV(0);
    VIDisplayLoadingScreen__Fb(false);
    printf("Loading Screen\n");
    sceGsSyncV(0);
    sceGsSyncV(0);
    if (Init__9VICollide(collide) < 0) return -1;
    if (Init__12VIDictionaryi(dictionary, 0) < 0) return -1;
    if (Init__13VISoundDevice(device) < 0) return -1;
    if (Init__7VIScenelR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(scene, static_cast<long>(GetTime__7VITimer()), dictionary, raster, device, collide) < 0) return -1;
    if (Init__4VIUIP8VIRasterP13VISoundDeviceP12VIDictionary(ui, raster, device, dictionary) < 0) return -1;
    VISetWindowUI__FP4VIUI(ui);
    printf("Here, ui = %i\n", ui);
    __10VIESFParse(parser);
    __9VICSFFile(file);
    if (Parse__9VICSFFilePCcR10VIESFParseR7VIScene(file, "data2\\ArialUI.csf", parser, scene) < 0 ||
        Parse__9VICSFFilePCcR10VIESFParseR7VIScene(file, "data2\\ui.csf", parser, scene) < 0 ||
        Parse__9VICSFFilePCcR10VIESFParseR7VIScene(file, "data2\\statui.csf", parser, scene) < 0 ||
        Parse__9VICSFFilePCcR10VIESFParseR7VIScene(file, "data2\\pregame.csf", parser, scene) < 0 ||
        Parse__9VICSFFilePCcR10VIESFParseR7VIScene(file, "data2\\conui.csf", parser, scene) < 0) {
        _$_10VIESFParse(parser, 2);
        return -1;
    }
    SetTimeOfDay__7VIScenef(scene, 12.0f);
    SetWireframe__8VIRasteri(raster, 0);
    color[0] = 0.0f;
    color[1] = 0.0f;
    color[2] = 0.0f;
    color[3] = 1.0f;
    SetClearColor__8VIRasterRC10VIColor32F(raster, color);
    *reinterpret_cast<int*>(base + 0xb794) = 0;
    Init__15VIWindowManagerP5VIPadP10VIKeyboard(manager, pad, keyboard);
    SetAppPtr__5VIWndPv(renderer, object);
    SetStationName__20VIWndGenericRendererPCc(renderer, b);
    SetPassword__20VIWndGenericRendererPCc(renderer, c);
    Init__20VIWndGenericRenderer(renderer);
    memset(text, 0, 2048);
    snprintf16__8VIStringPUsiPCce(text, 2048, "Connecting to SOEGames.net...");
    SetAppPtr__5VIWndPv(eula, object);
    Init__9VIWndEula(eula);
    SetText__9VIWndEulaPCUsi(eula, text, strlen16__8VIStringPCUs(text));
    SetRoot__15VIWindowManagerP5VIWnd(manager, renderer);
    SetFocus__5VIWndP5VIWndb(manager, renderer, true);
    Init__7VIFaderP8VIRasterP12VIDictionaryf(fader, raster, dictionary, 1.0f);
    Begin__7VIFaderff(fader, 0.0f, 0.5f);
    _$_10VIESFParse(parser, 2);
    return 0;
}
}

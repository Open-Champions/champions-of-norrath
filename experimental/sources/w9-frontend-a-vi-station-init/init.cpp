// Observed: Init__9VIStationiPCcPcT3T2T2i at 0x010179c8 is 1260 bytes. Arguments are (this, a
// word, a string A, a string B, a string C, a string D, a string E, a word); the mangled name
// marks A, D and E as const and B and C as mutable. Steps, in order:
//  1. The word at this + 0 is set to the first argument and the word at this + 0x17a30 to 0.
//     strcpy(this + 4, A). The word at this + 0xb81c is set to 0. printf("station::init()\n"),
//     then sceGsSyncV(0) twice. strcpy(this + 0x404, B); strcpy(this + 0x504, C);
//     printf("%s, %s\n", this + 0x404, this + 0x504); sceGsSyncV(0) twice.
//  2. memset of 64 bytes at this + 0x17a34 and at this + 0x17a74 to zero; strcpy of D and E into
//     them when D respectively E is not null; the last word argument is stored at this + 0x17ab4.
//  3. Init__5VIPadii(this + 0xb470, 0, 0); Init__10VIKeyboard(this + 0xb4d0); the word at
//     this + 0xb468 becomes 1; printf("Pad/Kb init\n"); sceGsSyncV(0) twice.
//  4. Init__8VIRasterPviii(this + 0x610, 0, 640, 448, 0). A negative result prints "raster init
//     failed\n" and returns -1. Otherwise printf("Raster Init\n"), sceGsSyncV(0) twice,
//     VIDisplayLoadingScreen__Fb(0), printf("Loading Screen\n"), sceGsSyncV(0) twice.
//  5. With early return of -1 (no message) on any negative result: Init__9VICollide(this + 0x5270);
//     Init__12VIDictionaryi(this + 0x5690, 0); Init__13VISoundDevice(this + 0x56a8);
//     Init__7VIScenelR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(this + 0x6830,
//     VITimer::GetTime(), dictionary, raster, sound, collide);
//     Init__4VIUIP8VIRasterP13VISoundDeviceP12VIDictionary(this + 0xb3f0, raster, sound,
//     dictionary). Then VISetWindowUI__FP4VIUI(this + 0xb3f0) and printf("Here, ui = %i\n",
//     this + 0xb3f0).
//  6. Two stack objects are constructed (__10VIESFParse at stack offset 0, __9VICSFFile at 1072).
//     Parse__9VICSFFilePCcR10VIESFParseR7VIScene is called on the scene with "data2\\ArialUI.csf",
//     "data2\\ui.csf", "data2\\statui.csf", "data2\\pregame.csf" and "data2\\conui.csf" in that
//     order; the first negative result calls _$_10VIESFParse(object, 2) and returns -1.
//  7. SetTimeOfDay__7VIScenef(scene, 12.0f); SetWireframe__8VIRasteri(raster, 0); SetClearColor
//     with a stack color of floats 0, 0, 0, 1; the word at this + 0xb794 becomes 0;
//     Init__15VIWindowManagerP5VIPadP10VIKeyboard(this + 0xb798, this + 0xb470, this + 0xb4d0).
//     On the window this + 0xb820: SetAppPtr__5VIWndPv(window, this),
//     SetStationName__20VIWndGenericRendererPCc(window, B), SetPassword__20VIWndGenericRendererPCc
//     (window, C), Init__20VIWndGenericRenderer(window).
//  8. A 2048-byte stack buffer at stack offset 1104 is zeroed with memset and filled by
//     snprintf16__8VIStringPUsiPCce(buffer, 2048, "Connecting to SOEGames.net..."). On the window
//     this + 0x15408: SetAppPtr(window, this), Init__9VIWndEula(window), SetText__9VIWndEulaPCUsi
//     (window, buffer, strlen16__8VIStringPCUs(buffer)). SetRoot__15VIWindowManagerP5VIWnd and
//     SetFocus__5VIWndP5VIWndb(…, 1) of the window manager onto this + 0xb820.
//     Init__7VIFaderP8VIRasterP12VIDictionaryf(this + 0xb448, raster, dictionary, 1.0f);
//     Begin__7VIFaderff(fader, 0.0f, 0.5f); _$_10VIESFParse(parse object, 2). It returns 0.
// Inferred: the sub-object offsets are those Process and Free use; 0 means success, -1 failure.
// Unknown: the member types, the stack object sizes, the meaning of the first word argument and
// of the words at this + 0xb468 and 0xb794, and why this routine omits the sound flush and
// splash effect set-up that VISetup::Init performs.

extern "C" {
int printf(const char* format, ...);
char* strcpy(char* to, const char* from);
void* memset(void* to, int value, unsigned size);
int sceGsSyncV(int mode);
int Init__5VIPadii(void* pad, int a, int b);
void Init__10VIKeyboard(void* keyboard);
int Init__8VIRasterPviii(void* raster, void* surface, int width, int height, int flag);
void VIDisplayLoadingScreen__Fb(bool flag);
int Init__9VICollide(void* collide);
int Init__12VIDictionaryi(void* dictionary, int value);
int Init__13VISoundDevice(void* sound);
long GetTime__7VITimer();
int Init__7VIScenelR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(void* scene, long time, void* dictionary,
                                                                        void* raster, void* sound, void* collide);
int Init__4VIUIP8VIRasterP13VISoundDeviceP12VIDictionary(void* ui, void* raster, void* sound, void* dictionary);
void VISetWindowUI__FP4VIUI(void* ui);
void* __10VIESFParse(void* object);
void* __9VICSFFile(void* object);
int Parse__9VICSFFilePCcR10VIESFParseR7VIScene(void* file, const char* path, void* parse, void* scene);
void _$_10VIESFParse(void* object, int flags);
void SetTimeOfDay__7VIScenef(void* scene, float time);
void SetWireframe__8VIRasteri(void* raster, int value);
void SetClearColor__8VIRasterRC10VIColor32F(void* raster, void* color);
void Init__15VIWindowManagerP5VIPadP10VIKeyboard(void* manager, void* pad, void* keyboard);
void SetAppPtr__5VIWndPv(void* window, void* application);
void SetStationName__20VIWndGenericRendererPCc(void* window, const char* name);
void SetPassword__20VIWndGenericRendererPCc(void* window, const char* password);
void Init__20VIWndGenericRenderer(void* window);
int snprintf16__8VIStringPUsiPCce(unsigned short* buffer, int size, const char* format, ...);
int strlen16__8VIStringPCUs(const unsigned short* text);
void Init__9VIWndEula(void* window);
void SetText__9VIWndEulaPCUsi(void* window, const unsigned short* text, int length);
void SetRoot__15VIWindowManagerP5VIWnd(void* manager, void* window);
void SetFocus__5VIWndP5VIWndb(void* manager, void* window, bool flag);
void Init__7VIFaderP8VIRasterP12VIDictionaryf(void* fader, void* raster, void* dictionary, float value);
void Begin__7VIFaderff(void* fader, float from, float to);
}

namespace {
bool parse_failed(void* file, const char* path, void* parse, void* scene) {
    return Parse__9VICSFFilePCcR10VIESFParseR7VIScene(file, path, parse, scene) < 0;
}
}

extern "C" int Init__9VIStationiPCcPcT3T2T2i(void* self, int first, const char* a, char* b, char* c, const char* d,
                                             const char* e, int last) {
    unsigned char* const base = static_cast<unsigned char*>(self);
    unsigned char parse_object[1072];
    unsigned char file_object[32];
    unsigned short text[1024];
    float color[4];
    *reinterpret_cast<int*>(base) = first;
    *reinterpret_cast<int*>(base + 0x17a30) = 0;
    strcpy(reinterpret_cast<char*>(base + 4), a);
    *reinterpret_cast<int*>(base + 0xb81c) = 0;
    printf("station::init()\n");
    sceGsSyncV(0);
    sceGsSyncV(0);
    strcpy(reinterpret_cast<char*>(base + 0x404), b);
    strcpy(reinterpret_cast<char*>(base + 0x504), c);
    printf("%s, %s\n", reinterpret_cast<char*>(base + 0x404), reinterpret_cast<char*>(base + 0x504));
    sceGsSyncV(0);
    sceGsSyncV(0);
    memset(base + 0x17a34, 0, 64);
    memset(base + 0x17a74, 0, 64);
    if (d != 0) {
        strcpy(reinterpret_cast<char*>(base + 0x17a34), d);
    }
    if (e != 0) {
        strcpy(reinterpret_cast<char*>(base + 0x17a74), e);
    }
    *reinterpret_cast<int*>(base + 0x17ab4) = last;
    void* const pad = base + 0xb470;
    void* const keyboard = base + 0xb4d0;
    Init__5VIPadii(pad, 0, 0);
    Init__10VIKeyboard(keyboard);
    *reinterpret_cast<int*>(base + 0xb468) = 1;
    printf("Pad/Kb init\n");
    sceGsSyncV(0);
    sceGsSyncV(0);
    void* const raster = base + 0x610;
    if (Init__8VIRasterPviii(raster, 0, 640, 448, 0) < 0) {
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
    void* const collide = base + 0x5270;
    if (Init__9VICollide(collide) < 0) {
        return -1;
    }
    void* const dictionary = base + 0x5690;
    if (Init__12VIDictionaryi(dictionary, 0) < 0) {
        return -1;
    }
    void* const sound = base + 0x56a8;
    if (Init__13VISoundDevice(sound) < 0) {
        return -1;
    }
    void* const scene = base + 0x6830;
    if (Init__7VIScenelR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(scene, GetTime__7VITimer(), dictionary, raster,
                                                                            sound, collide) < 0) {
        return -1;
    }
    void* const ui = base + 0xb3f0;
    if (Init__4VIUIP8VIRasterP13VISoundDeviceP12VIDictionary(ui, raster, sound, dictionary) < 0) {
        return -1;
    }
    VISetWindowUI__FP4VIUI(ui);
    printf("Here, ui = %i\n", ui);
    __10VIESFParse(parse_object);
    __9VICSFFile(file_object);
    if (parse_failed(file_object, "data2\\ArialUI.csf", parse_object, scene) ||
        parse_failed(file_object, "data2\\ui.csf", parse_object, scene) ||
        parse_failed(file_object, "data2\\statui.csf", parse_object, scene) ||
        parse_failed(file_object, "data2\\pregame.csf", parse_object, scene) ||
        parse_failed(file_object, "data2\\conui.csf", parse_object, scene)) {
        _$_10VIESFParse(parse_object, 2);
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
    void* const manager = base + 0xb798;
    Init__15VIWindowManagerP5VIPadP10VIKeyboard(manager, pad, keyboard);
    void* const window = base + 0xb820;
    SetAppPtr__5VIWndPv(window, self);
    SetStationName__20VIWndGenericRendererPCc(window, b);
    SetPassword__20VIWndGenericRendererPCc(window, c);
    Init__20VIWndGenericRenderer(window);
    memset(text, 0, sizeof text);
    snprintf16__8VIStringPUsiPCce(text, 2048, "Connecting to SOEGames.net...");
    void* const eula = base + 0x15408;
    SetAppPtr__5VIWndPv(eula, self);
    Init__9VIWndEula(eula);
    SetText__9VIWndEulaPCUsi(eula, text, strlen16__8VIStringPCUs(text));
    SetRoot__15VIWindowManagerP5VIWnd(manager, window);
    SetFocus__5VIWndP5VIWndb(manager, window, true);
    void* const fader = base + 0xb448;
    Init__7VIFaderP8VIRasterP12VIDictionaryf(fader, raster, dictionary, 1.0f);
    Begin__7VIFaderff(fader, 0.0f, 0.5f);
    _$_10VIESFParse(parse_object, 2);
    return 0;
}

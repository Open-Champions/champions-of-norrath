// Observed: OnDrawSelf__12VIWndOptions at 0x010c3e00 is 1292 bytes. With (this), ui and focus being the words
// _5VIWnd$mUI (0x01192c70) and _5VIWnd$mFocus (0x01192c74), g = VIGetGameOptions__Fv() (called first) and
// music = this + 1200 (an embedded VIBattleMusic):
//  1. when the word at +1196 is nonzero and the word at +1184 is zero: RemoveChild__5VIWndP5VIWnd(this, it) and
//     the word is cleared;
//  2. when the first word of the file-local table sBattleMusicText (0x011e8bd0) is zero, six file-local text
//     tables are filled with GetString__13VIStringTableii(ui, id, the word at ui + 48) results, ui being
//     re-read for each call. sBattleMusicText[0..2] get ids 517, 795, 796; when VIGetFeatureFlags__Fv() & 0x100,
//     [3..9] get 797, 798, 799, 1576, 1577, 1578, 1630. Then, in call order: sSoundOutputText[0..1] 781, 804;
//     sCompactKeyboardText[0..1] 802, 755; sOnOffText[0..1] 552, 551; sCameraTypeText[0..1] 805, 68;
//     sCameraAxisText[0..1] 784, 779 (all 0x011e8bf8..0x011e8c1f, two words each);
//  3. when focus differs from this, ResetTickPulse__10VIUIWindow(this + 40) is called;
//  4. the word at +1144 (mode) decides the music: for 2, when VIGetFeatureFlags() & 0x100: g byte 4 below 9
//     -> Play__13VIBattleMusici(music, that byte), else Stop__13VIBattleMusicb(music, 0); without the flag:
//     Play(music, -1) when g byte 4 equals the word at music + 4, else Play(music, g byte 4). Any other mode
//     calls StopNow__13VIBattleMusic(music). Then Process__13VIBattleMusic(music);
//  5. Begin2D__4VIUI(ui); DrawMainMenu__12VIWndOptions(this); mode 0 / 1 / 2 call DrawGameOptions /
//     DrawInterfaceOptions / DrawSoundOptions__12VIWndOptions(this), other modes none;
//  6. when the word at +1188 (count) is nonzero: it is decremented; w = this + 776 gets
//     Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(w, ui, &{0, 0}, &{words at +52, +56}, 400, 3, 0),
//     Open__10VIUIWindowb(w, 1), Draw__10VIUIWindow(w) and TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(w,
//     GetString(ui, 1615, col), 40, 0, width - 80, 3, 0, 8) with width the word at w + 20; when the count is
//     then zero SaveAndExit__12VIWndOptions(this) is called;
//  7. when the word at +1192 is nonzero: the same window is Init-ed with (440, 6, 0), Open(w, 1), Draw(w) and
//     TextBox(w, GetString(ui, 1616, col), 40, 0, width - 80, 4, 0, 8), then
//     TextBox(w, T__10VIUIWindowPCc(w, "|qOk|w"), 40, 5, width - 80, 1, 0, 8);
//  8. End2D__4VIUI(ui), with the ui value read at the start.
// Inferred: g is the game-options record and this window caches its menu texts on the first draw; the count
// at +1188 is a countdown after which the window saves and exits; the file-local table names are the
// original symbol spellings.
// Unknown: field names; what the second words of each text table are used for beyond selection; the
// TextBox integer arguments (register order only); whether sBattleMusicText is really ten words (the image
// symbol size is not used: the stores through [9] are observed); the return type (void).
extern "C" {
unsigned char* VIGetGameOptions__Fv();
unsigned VIGetFeatureFlags__Fv();
void RemoveChild__5VIWndP5VIWnd(void* self, void* child);
void ResetTickPulse__10VIUIWindow(void* window);
void Play__13VIBattleMusici(void* music, int track);
void Stop__13VIBattleMusicb(void* music, bool flag);
void StopNow__13VIBattleMusic(void* music);
void Process__13VIBattleMusic(void* music);
void Begin2D__4VIUI(void* ui);
void End2D__4VIUI(void* ui);
void DrawMainMenu__12VIWndOptions(void* self);
void DrawGameOptions__12VIWndOptions(void* self);
void DrawInterfaceOptions__12VIWndOptions(void* self);
void DrawSoundOptions__12VIWndOptions(void* self);
void SaveAndExit__12VIWndOptions(void* self);
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* window, unsigned ui, const int* position, const int* size,
                                              int a, int b, int c);
void Open__10VIUIWindowb(void* window, bool flag);
void Draw__10VIUIWindow(void* window);
void TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(void* window, const unsigned short* text, int a, int b, int c, int d,
                                               int color, unsigned e);
const unsigned short* T__10VIUIWindowPCc(void* window, const char* key);
const unsigned short* GetString__13VIStringTableii(const void* table, int id, int column);
extern unsigned _5VIWnd$mUI;
extern unsigned _5VIWnd$mFocus;
extern unsigned sBattleMusicText[10] __asm__("VIWndOptions$sBattleMusicText");
extern unsigned sSoundOutputText[2] __asm__("VIWndOptions$sSoundOutputText");
extern unsigned sCompactKeyboardText[2] __asm__("VIWndOptions$sCompactKeyboardText");
extern unsigned sOnOffText[2] __asm__("VIWndOptions$sOnOffText");
extern unsigned sCameraTypeText[2] __asm__("VIWndOptions$sCameraTypeText");
extern unsigned sCameraAxisText[2] __asm__("VIWndOptions$sCameraAxisText");

static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(static_cast<unsigned long>(word));
}
static inline unsigned word_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
static inline unsigned text_for(int id) {
    const void* table = pointer_of(_5VIWnd$mUI);
    return word_of(GetString__13VIStringTableii(table, id, static_cast<const int*>(table)[12]));
}
static inline const unsigned short* string_for(int id) {
    const void* table = pointer_of(_5VIWnd$mUI);
    return GetString__13VIStringTableii(table, id, static_cast<const int*>(table)[12]);
}

void OnDrawSelf__12VIWndOptions(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    int* words = static_cast<int*>(self);
    void* ui = pointer_of(_5VIWnd$mUI);
    unsigned char* g = VIGetGameOptions__Fv();
    if (words[299] != 0 && words[296] == 0) {
        RemoveChild__5VIWndP5VIWnd(self, pointer_of(static_cast<unsigned>(words[299])));
        words[299] = 0;
    }
    if (sBattleMusicText[0] == 0) {
        sBattleMusicText[0] = text_for(517);
        sBattleMusicText[1] = text_for(795);
        sBattleMusicText[2] = text_for(796);
        if (VIGetFeatureFlags__Fv() & 0x100) {
            sBattleMusicText[3] = text_for(797);
            sBattleMusicText[4] = text_for(798);
            sBattleMusicText[5] = text_for(799);
            sBattleMusicText[6] = text_for(1576);
            sBattleMusicText[7] = text_for(1577);
            sBattleMusicText[8] = text_for(1578);
            sBattleMusicText[9] = text_for(1630);
        }
        sSoundOutputText[0] = text_for(781);
        sSoundOutputText[1] = text_for(804);
        sCompactKeyboardText[0] = text_for(802);
        sCompactKeyboardText[1] = text_for(755);
        sOnOffText[0] = text_for(552);
        sOnOffText[1] = text_for(551);
        sCameraTypeText[0] = text_for(805);
        sCameraTypeText[1] = text_for(68);
        sCameraAxisText[0] = text_for(784);
        sCameraAxisText[1] = text_for(779);
    }
    if (_5VIWnd$mFocus != word_of(self)) ResetTickPulse__10VIUIWindow(bytes + 40);
    void* music = bytes + 1200;
    if (words[286] == 2) {
        if (VIGetFeatureFlags__Fv() & 0x100) {
            const unsigned track = g[4];
            if (track < 9) Play__13VIBattleMusici(music, static_cast<int>(track));
            else Stop__13VIBattleMusicb(music, false);
        } else {
            const unsigned track = g[4];
            if (static_cast<int>(track) == *reinterpret_cast<int*>(bytes + 1204)) Play__13VIBattleMusici(music, -1);
            else Play__13VIBattleMusici(music, static_cast<int>(track));
        }
    } else {
        StopNow__13VIBattleMusic(music);
    }
    Process__13VIBattleMusic(music);
    Begin2D__4VIUI(ui);
    DrawMainMenu__12VIWndOptions(self);
    switch (words[286]) {
    case 0:
        DrawGameOptions__12VIWndOptions(self);
        break;
    case 1:
        DrawInterfaceOptions__12VIWndOptions(self);
        break;
    case 2:
        DrawSoundOptions__12VIWndOptions(self);
        break;
    default:
        break;
    }
    if (words[297] != 0) {
        words[297] = words[297] - 1;
        void* window = bytes + 776;
        const int position[2] = {0, 0};
        const int size[2] = {words[13], words[14]};
        Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, _5VIWnd$mUI, position, size, 400, 3, 0);
        Open__10VIUIWindowb(window, true);
        Draw__10VIUIWindow(window);
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, string_for(1615), 40, 0,
                                                  *reinterpret_cast<int*>(bytes + 776 + 20) - 80, 3, 0, 8);
        if (words[297] == 0) SaveAndExit__12VIWndOptions(self);
    }
    if (words[298] != 0) {
        void* window = bytes + 776;
        const int position[2] = {0, 0};
        const int size[2] = {words[13], words[14]};
        Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, _5VIWnd$mUI, position, size, 440, 6, 0);
        Open__10VIUIWindowb(window, true);
        Draw__10VIUIWindow(window);
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, string_for(1616), 40, 0,
                                                  *reinterpret_cast<int*>(bytes + 776 + 20) - 80, 4, 0, 8);
        const unsigned short* ok = T__10VIUIWindowPCc(window, "|qOk|w");
        TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi(window, ok, 40, 5, *reinterpret_cast<int*>(bytes + 776 + 20) - 80, 1, 0, 8);
    }
    End2D__4VIUI(ui);
}
}

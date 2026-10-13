// Observed: OnKeyDown__12VIWndConnecti at 0x010a1e28 is 608 bytes. With (this, key), mUI being the word
// _5VIWnd$mUI (0x01192c70): nothing happens when the word at +1956 is nonzero. Otherwise:
//  - key 136 / 137: nothing when the word at +1920 is nonzero; else the word at +1916 becomes
//    (it + count - 1) % count / (it + 1) % count (signed, count being the word at +1968; a zero count traps),
//    then SoundPlay__4VIUI11VIUISoundId(mUI, 11);
//  - key 134: nothing when +1920 is nonzero; else SoundPlay(mUI, 15), the word at +1920 is set to 1,
//    BeginConnect__12VIWndConnect(this), then with w = this + 1544: Close(w, 1); width =
//    GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCc(mUI + 56, word at mUI + 72, word at mUI + 80, the
//    text at (word at +1972) + 620 * (word at +1916) + 260); then Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(w,
//    mUI, &{0, 0}, &{word at +1188, word at +1192}, max(width, 300) + 120, 4, 0), and Open(w, 0);
//  - key 135: when +1920 is zero: SoundPlay(mUI, 8), the word at +28 is cleared and SetFocus__5VIWndP5VIWndb(this,
//    the word at +20, 1). When +1920 is nonzero and the word at +1964 is zero: ezNetCtlDownInterface(0), the
//    word at +1920 is cleared, SoundPlay(mUI, 8), and sceGsSyncV(0) is called 60 times. When both are
//    nonzero nothing happens;
//  - every other key does nothing.
// Inferred: +1916 is the selected entry, +1968 the entry count and +1972 the entry array (stride 620 bytes, a
// text at +260); +1920 says a connection attempt is running; 134 starts it, 135 cancels or leaves.
// Unknown: key names, the meaning of +1964, +1956, 120, 4 and the sound numbers; the return type (void).
extern "C" {
void SoundPlay__4VIUI11VIUISoundId(const void* ui, int sound);
void SetFocus__5VIWndP5VIWndb(void* self, void* target, bool flag);
void BeginConnect__12VIWndConnect(void* self);
void Close__10VIUIWindowb(void* window, bool flag);
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* window, unsigned ui, const int* position, const int* size,
                                              int a, int b, int c);
void Open__10VIUIWindowb(void* window, bool flag);
int GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCc(void* font, void* raster, void* dictionary, const char* text);
int ezNetCtlDownInterface(int value);
int sceGsSyncV(int mode);
extern unsigned _5VIWnd$mUI;

static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(static_cast<unsigned long>(word));
}

void OnKeyDown__12VIWndConnecti(void* self, int key) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    int* words = static_cast<int*>(self);
    if (words[489] != 0) return;
    if (key == 135) {
        if (words[480] == 0) {
            SoundPlay__4VIUI11VIUISoundId(pointer_of(_5VIWnd$mUI), 8);
            words[7] = 0;
            SetFocus__5VIWndP5VIWndb(self, pointer_of(static_cast<unsigned>(words[5])), true);
        } else if (words[491] == 0) {
            ezNetCtlDownInterface(0);
            words[480] = 0;
            SoundPlay__4VIUI11VIUISoundId(pointer_of(_5VIWnd$mUI), 8);
            for (int repeat = 0; repeat < 60; ++repeat) sceGsSyncV(0);
        }
    } else if (key == 134) {
        if (words[480] != 0) return;
        const unsigned ui = _5VIWnd$mUI;
        unsigned char* ui_bytes = static_cast<unsigned char*>(pointer_of(ui));
        SoundPlay__4VIUI11VIUISoundId(ui_bytes, 15);
        words[480] = 1;
        BeginConnect__12VIWndConnect(self);
        void* window = bytes + 1544;
        Close__10VIUIWindowb(window, true);
        const char* text = static_cast<const char*>(pointer_of(static_cast<unsigned>(words[493]))) + 620 * words[479] + 260;
        const int width = GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCc(
            ui_bytes + 56, pointer_of(*reinterpret_cast<unsigned*>(ui_bytes + 72)),
            pointer_of(*reinterpret_cast<unsigned*>(ui_bytes + 80)), text);
        const int position[2] = {0, 0};
        const int size[2] = {words[297], words[298]};
        Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, ui, position, size, (width < 300 ? 300 : width) + 120, 4, 0);
        Open__10VIUIWindowb(window, false);
    } else if (key == 136) {
        if (words[480] != 0) return;
        const int count = words[492];
        words[479] = (words[479] + count - 1) % count;
        SoundPlay__4VIUI11VIUISoundId(pointer_of(_5VIWnd$mUI), 11);
    } else if (key == 137) {
        if (words[480] != 0) return;
        words[479] = (words[479] + 1) % words[492];
        SoundPlay__4VIUI11VIUISoundId(pointer_of(_5VIWnd$mUI), 11);
    }
}
}

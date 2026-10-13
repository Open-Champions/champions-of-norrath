// Observed: OnDrawSelf__11VIWndSplash at 0x010d5ed8 is 1516 bytes. app = GetAppPtr__5VIWnd(this); ui = the
// word _5VIWnd$mUI (0x01192c70); text(id) = GetString__13VIStringTableii(ui, id, word at ui+48); w is the
// embedded UI window at this+40; msg is the embedded message window at this+4776; conn is at this+416.
// In order:
//  1. Let pad = the word at app+0xaeb0. When pad is zero and the local word lastPadState.2196 (0x011afad8) is
//     nonzero: EndConnect__12VIWndConnect(conn); store 0 at conn+28; if the focus word _5VIWnd$mFocus equals
//     McErrWnd__Fv(), call this's table entry at byte offset 48 (OnMessage) twice, as (this; McErrWnd(), 4354,
//     0, 0) and (this; McErrWnd(), 4358, 10, 0). Then in both cases Setup__12VIWndMessage(msg, text(767),
//     text(1862), 0, &{320, 246}, 400), store 1 at msg+28 and SetFocus__5VIWndP5VIWndb(this, msg, true).
//  2. Store pad into lastPadState.2196. When mFocus is not this, ResetTickPulse__10VIUIWindow(w).
//  3. When the word at this+9648 is nonzero: Setup(msg, text(272), text(809), 0, &{320, 246}, 320); otherwise,
//     when the word at this+9652 is nonzero: the same with text(808). After either: store 1 at msg+28,
//     SetFocus(this, msg, true), zero this+9648 and this+9652.
//  4. When the word at this+9644 is nonzero: RemoveChild__5VIWndP5VIWnd(this, it), store 0 there. Then store 1 at
//     this+9656.
//  5. Begin2D__4VIUI(ui); Blit__4VIUIiG8VIVect2I16VIAlphaBlendModefUi twice with ids VIHashResourceID of the strings
//     "c:\tmp\srs\conbk00.tga" at {0, 0} and "c:\tmp\srs\conbk10.tga" at {320, 0} (mode 0, scale 1.0, flags 0);
//     SetTextColor__6VIFontRC9VIColor32(ui+56, bytes {255, 226, 97, 255}); DrawString__6VIFontP8VIRasterP12VIDictionaryPCUsii(
//     ui+56, ui[72], ui[80], text(217), 265, 378); BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui, 176,
//     {237, 380}, 0, 1.0, 0); DrawString(..., text(212), 356, 378); BlitEx(ui, 173, {328, 380}, 0, 1.0, 0).
//  6. Two TextBox__10VIUIWindowPCUsiiiiRC10VIColor32FUi(w, buffer, -320, row, 640, 1, colour, 9) calls (row 0 and
//     1). Let sel be the word at this+408. First buffer: when sel is zero, str8to16__8VIStringPUsUiPCc(buf, 512, "|q"),
//     else an empty string; then strcat16(buf, 512, text(790)); when sel is zero strcat16(buf, 512, "|w"). Colour is
//     white {1,1,1,1} when sel is nonzero, else yellow {1,1,0,1}. Second buffer: the same with sel equal to 1
//     (selecting "|q"/"|w"), text(1635), and white unless sel equals 1.
//  7. When the word at this+9660 equals 2: store 0 there and call SelectPlay__11VIWndSplash(this). End2D__4VIUI(ui).
// Inferred: a start screen drawn each frame with two background tiles and two selectable lines; the strings "|q"
// and "|w" look like text escapes for a highlighted entry; the word at +28 of an embedded window is a shown flag.
// The original also builds an unused five-integer array from half the window width and a zero pair; those
// stack stores are not reproduced.
// Unknown: the names of the words at app+0xaeb0, this+9644/9648/9652/9656/9660, and the meaning of 4354 and 4358.
extern "C" {
extern unsigned _5VIWnd$mUI;
extern unsigned _5VIWnd$mFocus;
extern int norrath_splash_last_pad_state __asm__("lastPadState.2196");

void* GetAppPtr__5VIWnd(void* self);
void EndConnect__12VIWndConnect(void* connect);
void* McErrWnd__Fv();
const unsigned short* GetString__13VIStringTableii(void* table, int id, int arg);
struct Pair {
    int a, b;
};
void Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(void* window, const unsigned short* first,
                                                              const unsigned short* second, int prompt,
                                                              const Pair* rect, int size);
void SetFocus__5VIWndP5VIWndb(void* self, void* window, bool force);
void ResetTickPulse__10VIUIWindow(void* window);
void RemoveChild__5VIWndP5VIWnd(void* self, void* child);
void Begin2D__4VIUI(unsigned ui);
void End2D__4VIUI(unsigned ui);
unsigned VIHashResourceID(const char* name);
void Blit__4VIUIiG8VIVect2I16VIAlphaBlendModefUi(unsigned ui, int id, const Pair* position, int mode, float scale,
                                                  unsigned flags);
void BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(unsigned ui, int id, const Pair* position, int mode,
                                                             float scale, unsigned flags);
struct Color32 {
    unsigned char r, g, b, a;
};
struct Color32F {
    float r, g, b, a;
};
void SetTextColor__6VIFontRC9VIColor32(void* font, const Color32* color);
void DrawString__6VIFontP8VIRasterP12VIDictionaryPCUsii(void* font, void* raster, void* dictionary,
                                                         const unsigned short* text, int x, int y);
void str8to16__8VIStringPUsUiPCc(unsigned short* destination, unsigned size, const char* source);
void strcat16__8VIStringPUsUiPCUs(unsigned short* destination, unsigned size, const unsigned short* source);
void strcat16__8VIStringPUsUiPCc(unsigned short* destination, unsigned size, const char* source);
void TextBox__10VIUIWindowPCUsiiiiRC10VIColor32FUi(void* window, const unsigned short* text, int a, int b, int c,
                                                   int d, const Color32F* color, unsigned flags);
void SelectPlay__11VIWndSplash(void* self);

struct VtEntry {
    short delta;
    short index;
    void (*function)(void*, void*, int, int, int);
};

static inline int& word(void* base, int offset) {
    return *reinterpret_cast<int*>(static_cast<unsigned char*>(base) + offset);
}
static inline void* pointer_of(unsigned value) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(value));
}
static inline const unsigned short* text(int id) {
    void* ui = pointer_of(_5VIWnd$mUI);
    return GetString__13VIStringTableii(ui, id, word(ui, 48));
}
static inline void send_message(void* target, void* from, int message, int a, int b) {
    unsigned char* object = static_cast<unsigned char*>(target);
    const VtEntry* entry = reinterpret_cast<const VtEntry*>(
        static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(object + 32))) + 6;
    entry->function(object + entry->delta, from, message, a, b);
}
static inline void show_message(void* self, int first, int second, int prompt, int width) {
    unsigned char* message = static_cast<unsigned char*>(self) + 4776;
    const unsigned short* a = text(first);
    const unsigned short* b = text(second);
    const Pair rect = {320, 246};
    Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(message, a, b, prompt, &rect, width);
    word(message, 28) = 1;
    SetFocus__5VIWndP5VIWndb(self, message, true);
}

void OnDrawSelf__11VIWndSplash(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    const Color32F white = {1.0f, 1.0f, 1.0f, 1.0f};
    const Color32F yellow = {1.0f, 1.0f, 0.0f, 1.0f};
    unsigned char* app = static_cast<unsigned char*>(GetAppPtr__5VIWnd(self));
    const int pad = *reinterpret_cast<int*>(app + 0xaeb0);
    const unsigned ui = _5VIWnd$mUI;
    if (pad == 0 && norrath_splash_last_pad_state != 0) {
        void* connect = bytes + 416;
        EndConnect__12VIWndConnect(connect);
        word(connect, 28) = 0;
        if (reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(_5VIWnd$mFocus)) == McErrWnd__Fv()) {
            send_message(self, McErrWnd__Fv(), 4354, 0, 0);
            send_message(self, McErrWnd__Fv(), 4358, 10, 0);
        }
        show_message(self, 767, 1862, 0, 400);
    }
    norrath_splash_last_pad_state = pad;
    void* window = bytes + 40;
    if (reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(_5VIWnd$mFocus)) != self)
        ResetTickPulse__10VIUIWindow(window);
    if (word(self, 9648) != 0) {
        show_message(self, 272, 809, 0, 320);
        word(self, 9648) = 0;
        word(self, 9652) = 0;
    } else if (word(self, 9652) != 0) {
        show_message(self, 272, 808, 0, 320);
        word(self, 9648) = 0;
        word(self, 9652) = 0;
    }
    if (word(self, 9644) != 0) {
        RemoveChild__5VIWndP5VIWnd(self, pointer_of(static_cast<unsigned>(word(self, 9644))));
        word(self, 9644) = 0;
    }
    word(self, 9656) = 1;
    void* uiobject = pointer_of(ui);
    void* font = static_cast<unsigned char*>(uiobject) + 56;
    Begin2D__4VIUI(ui);
    const Pair origin = {0, 0};
    Blit__4VIUIiG8VIVect2I16VIAlphaBlendModefUi(ui, static_cast<int>(VIHashResourceID("c:\\tmp\\srs\\conbk00.tga")),
                                                &origin, 0, 1.0f, 0);
    const Pair right = {320, 0};
    Blit__4VIUIiG8VIVect2I16VIAlphaBlendModefUi(ui, static_cast<int>(VIHashResourceID("c:\\tmp\\srs\\conbk10.tga")),
                                                &right, 0, 1.0f, 0);
    const Color32 gold = {255, 226, 97, 255};
    SetTextColor__6VIFontRC9VIColor32(font, &gold);
    void* raster = pointer_of(static_cast<unsigned>(word(uiobject, 72)));
    void* dictionary = pointer_of(static_cast<unsigned>(word(uiobject, 80)));
    DrawString__6VIFontP8VIRasterP12VIDictionaryPCUsii(font, raster, dictionary, text(217), 265, 378);
    const Pair firstIcon = {237, 380};
    BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui, 176, &firstIcon, 0, 1.0f, 0);
    DrawString__6VIFontP8VIRasterP12VIDictionaryPCUsii(font, raster, dictionary, text(212), 356, 378);
    const Pair secondIcon = {328, 380};
    BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(ui, 173, &secondIcon, 0, 1.0f, 0);
    unsigned short buffer[256];
    if (word(self, 408) == 0) str8to16__8VIStringPUsUiPCc(buffer, 512, "|q");
    else buffer[0] = 0;
    strcat16__8VIStringPUsUiPCUs(buffer, 512, text(790));
    if (word(self, 408) == 0) strcat16__8VIStringPUsUiPCc(buffer, 512, "|w");
    TextBox__10VIUIWindowPCUsiiiiRC10VIColor32FUi(window, buffer, -320, 0, 640, 1,
                                                  word(self, 408) != 0 ? &white : &yellow, 9);
    if (word(self, 408) == 1) str8to16__8VIStringPUsUiPCc(buffer, 512, "|q");
    else buffer[0] = 0;
    strcat16__8VIStringPUsUiPCUs(buffer, 512, text(1635));
    if (word(self, 408) == 1) strcat16__8VIStringPUsUiPCc(buffer, 512, "|w");
    TextBox__10VIUIWindowPCUsiiiiRC10VIColor32FUi(window, buffer, -320, 1, 640, 1,
                                                  word(self, 408) != 1 ? &white : &yellow, 9);
    if (word(self, 9660) == 2) {
        word(self, 9660) = 0;
        SelectPlay__11VIWndSplash(self);
    }
    End2D__4VIUI(ui);
}
}

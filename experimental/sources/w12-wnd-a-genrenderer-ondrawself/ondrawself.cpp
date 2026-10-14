// Observed: OnDrawSelf__20VIWndGenericRenderer at 0x010bd370 is 628 bytes. It first calls
// GetAppPtr__5VIWnd(this). mUI is the word _5VIWnd$mUI (0x01192c70); GetString__13VIStringTableii is
// called as (mUI, id, word at mUI+48). Then:
//  1. When the word at this+1412 is nonzero: printf("!!!Removing the child\n"),
//     RemoveChild__5VIWndP5VIWnd(this, that word), store 0 there, and return.
//  2. When the word at this+1088 is nonzero: Render__9VIGUIPage(page at this+52), then the dialog step.
//  3. Otherwise, when the word at this+1404 is zero it calls PostInit__20VIWndGenericRenderer(this);
//     if the word is still zero it returns. When the global mcerror equals 1 only the dialog step runs.
//     Else, when the page word is zero it returns; otherwise it counts renderCount.3634 up to 1000,
//     calls Render__9VIGUIPage(page), and, if the word at app+0xb4c8 is zero, does the "stop" sequence
//     (below) with texts 1862 (SetText__11VIGUIDialogPCUsb(dialog, text, 0)) and 1863
//     (Show__11VIGUIDialogPCUsPCc(dialog, text, 0)). Then the dialog step, then a stack VINetworkStatus
//     (108 bytes: construct, Initialize, CheckNetwork); when CheckNetwork returns nonzero: the stop
//     sequence, printf("Network disconnected\n"), SetText with text 1395, Show with text 1306 and the
//     string "DisconnectConfirmed"; when it returns zero: process__16VIRealmInterface(this+1880).
//     The status object is destroyed with flags 2 afterwards.
//  The dialog step: dialog = word at this+56; if nonzero and IsOpen__11VIGUIDialog(dialog) is nonzero,
//  Render__11VIGUIDialog(dialog).
//  The stop sequence: StopEula__9VIStation(app); store this at app+0xb81c; store 0 at word +28 of the
//  object whose pointer is at this+36; SetFocus__5VIWndP5VIWndb(app+0xb798, this, true).
// Inferred: the renderer draws the active GUI page and dialog each frame and, when the network
// link is lost, stops the EULA flow and shows a disconnect dialog; the word at +28 is a shown flag.
// Unknown: the names of the words at this+1088, +1404, +1412 and app+0xb4c8, the type of the object at
// this+36, and the meaning of the string "DisconnectConfirmed" as an action name.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<__UINTPTR_TYPE__>(norrath_host_pointer(word))
#else
#define NORRATH_WORD_ADDRESS(word) static_cast<__UINTPTR_TYPE__>(word)
#endif
extern "C" {
extern unsigned _5VIWnd$mUI;
extern int mcerror;
extern int norrath_generic_renderer_render_count __asm__("renderCount.3634");

void* GetAppPtr__5VIWnd(void* self);
int printf(const char* format, ...);
void RemoveChild__5VIWndP5VIWnd(void* self, void* child);
void Render__9VIGUIPage(void* page);
void PostInit__20VIWndGenericRenderer(void* self);
int IsOpen__11VIGUIDialog(void* dialog);
void Render__11VIGUIDialog(void* dialog);
void StopEula__9VIStation(void* station);
void SetFocus__5VIWndP5VIWndb(void* self, void* window, bool force);
const unsigned short* GetString__13VIStringTableii(void* table, int id, int arg);
void SetText__11VIGUIDialogPCUsb(void* dialog, const unsigned short* text, bool flag);
void Show__11VIGUIDialogPCUsPCc(void* dialog, const unsigned short* text, const char* action);
void __15VINetworkStatus(void* status);
void Initialize__15VINetworkStatus(void* status);
int CheckNetwork__15VINetworkStatus(void* status);
void process__16VIRealmInterface(void* realm);
void _$_15VINetworkStatus(void* status, int flags);

static inline unsigned& word(void* base, int offset) {
    return *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(base) + offset);
}
static inline void* pointer_of(unsigned value) {
    return reinterpret_cast<void*>(NORRATH_WORD_ADDRESS(value));
}
static inline void render_dialog(void* self) {
    void* dialog = pointer_of(word(self, 56));
    if (dialog != 0 && IsOpen__11VIGUIDialog(dialog)) Render__11VIGUIDialog(dialog);
}
static inline void stop_sequence(void* self, unsigned char* app) {
    StopEula__9VIStation(app);
    void* object = pointer_of(word(self, 36));
    *reinterpret_cast<unsigned*>(app + 0xb81c) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(self));
    word(object, 28) = 0;
    SetFocus__5VIWndP5VIWndb(app + 0xb798, self, true);
}
static inline const unsigned short* text(int id) {
    void* ui = pointer_of(_5VIWnd$mUI);
    return GetString__13VIStringTableii(ui, id, static_cast<int>(word(ui, 48)));
}

void OnDrawSelf__20VIWndGenericRenderer(void* self) {
    unsigned char* app = static_cast<unsigned char*>(GetAppPtr__5VIWnd(self));
    if (word(self, 1412) != 0) {
        printf("!!!Removing the child\n");
        RemoveChild__5VIWndP5VIWnd(self, pointer_of(word(self, 1412)));
        word(self, 1412) = 0;
        return;
    }
    if (word(self, 1088) != 0) {
        Render__9VIGUIPage(pointer_of(word(self, 52)));
        render_dialog(self);
        return;
    }
    if (word(self, 1404) == 0) PostInit__20VIWndGenericRenderer(self);
    if (word(self, 1404) == 0) return;
    if (mcerror == 1) {
        render_dialog(self);
        return;
    }
    void* page = pointer_of(word(self, 52));
    if (page == 0) return;
    if (norrath_generic_renderer_render_count < 1000) norrath_generic_renderer_render_count += 1;
    Render__9VIGUIPage(page);
    if (*reinterpret_cast<int*>(app + 0xb4c8) == 0) {
        stop_sequence(self, app);
        SetText__11VIGUIDialogPCUsb(pointer_of(word(self, 56)), text(1862), false);
        Show__11VIGUIDialogPCUsPCc(pointer_of(word(self, 56)), text(1863), 0);
    }
    render_dialog(self);
    unsigned status[27];
    __15VINetworkStatus(status);
    Initialize__15VINetworkStatus(status);
    if (CheckNetwork__15VINetworkStatus(status) != 0) {
        stop_sequence(self, app);
        printf("Network disconnected\n");
        SetText__11VIGUIDialogPCUsb(pointer_of(word(self, 56)), text(1395), false);
        Show__11VIGUIDialogPCUsPCc(pointer_of(word(self, 56)), text(1306), "DisconnectConfirmed");
    } else {
        process__16VIRealmInterface(static_cast<unsigned char*>(self) + 1880);
    }
    _$_15VINetworkStatus(status, 2);
}
}

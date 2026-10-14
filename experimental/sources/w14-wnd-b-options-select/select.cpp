// Observed: Select__12VIWndOptions at 0x010c5118 is 556 bytes. Words are named by byte offset in this; mUI is the
// word _5VIWnd$mUI (0x01192c70); sub is the window object at this + 408.
//  - When the word at +1144 (the section) is not -1: SoundPlay__4VIUI11VIUISoundId(mUI, 9) and the result is 0.
//  - Otherwise SoundPlay(mUI, 8), the word at +1148 is cleared, and the word at +1180 (the top-level cursor)
//    decides:
//      0: section = 0, 1: section = 1, 2: section = 2 (stored before anything else); then
//         Close__10VIUIWindowb(sub, 1), Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(sub, mUI, {0, 0}, {the words at +52
//         and +56}, 440, kind, 0), Open__10VIUIWindowb(sub, 0) and SetTitle__10VIUIWindowPCc(sub, "") (the title
//         is the empty string at 0x011d5430); result 0. kind is 6 for cursor 0, 7 for cursor 2, and for cursor 1
//         it is 8 when (VIGetFeatureFlags__Fv() & 0x200) is nonzero (the call comes after Close) and 7 otherwise;
//      3: nothing is changed and the result is 1;
//      any other value: the result is 0.
// Inferred: this activates the highlighted entry of the options top menu: entries 0..2 open a sub-page in the
// window at +408 (kind 6, 7 or 8 picks the page layout), entry 3 asks the caller to act (the key handler treats
// a result of 1 as "open the confirmation prompt"). The flag bit 0x200 adds a page variant for cursor 1.
// Unknown: field names, what the kinds 6..8 and the size 440 mean, what feature bit 0x200 is, and the return
// type beyond int.
extern "C" {
unsigned VIGetFeatureFlags__Fv();
void SoundPlay__4VIUI11VIUISoundId(const void* ui, int sound);
void Close__10VIUIWindowb(void* window, bool flag);
void Open__10VIUIWindowb(void* window, bool flag);
void Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(void* window, unsigned ui, const int* position, const int* size,
                                              int a, int b, int c);
void SetTitle__10VIUIWindowPCc(void* window, const char* text);
extern unsigned _5VIWnd$mUI;

static inline const void* ui_pointer() { return reinterpret_cast<const void*>(static_cast<unsigned long>(_5VIWnd$mUI)); }

static void open_page(int* words, int kind) {
    void* window = reinterpret_cast<char*>(words) + 408;
    const int position[2] = {0, 0};
    const int size[2] = {words[13], words[14]};
    Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii(window, _5VIWnd$mUI, position, size, 440, kind, 0);
    Open__10VIUIWindowb(window, false);
    SetTitle__10VIUIWindowPCc(window, "");
}

int Select__12VIWndOptions(void* self) {
    int* words = static_cast<int*>(self);
    void* window = reinterpret_cast<char*>(self) + 408;
    if (words[286] != -1) {
        SoundPlay__4VIUI11VIUISoundId(ui_pointer(), 9);
        return 0;
    }
    SoundPlay__4VIUI11VIUISoundId(ui_pointer(), 8);
    words[287] = 0;
    const int cursor = words[295];
    if (cursor == 1) {
        words[286] = 1;
        Close__10VIUIWindowb(window, true);
        open_page(words, (VIGetFeatureFlags__Fv() & 0x200) != 0 ? 8 : 7);
        return 0;
    }
    if (cursor == 0) {
        words[286] = 0;
        Close__10VIUIWindowb(window, true);
        open_page(words, 6);
        return 0;
    }
    if (cursor == 2) {
        words[286] = 2;
        Close__10VIUIWindowb(window, true);
        open_page(words, 7);
        return 0;
    }
    return cursor == 3 ? 1 : 0;
}
}

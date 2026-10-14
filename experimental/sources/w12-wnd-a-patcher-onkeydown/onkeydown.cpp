// Observed: OnKeyDown__12VIWndPatcheri at 0x010cc350 is 660 bytes. With (this, key); mUI is the word
// _5VIWnd$mUI (0x01192c70). m is the word at this+1148, sel the word at this+1144, and the embedded
// read-message window is at this+2312 (Message__C16VIWndReadMessage returns its text at +412):
//  - 138 and 139 act only when m is 4 and the message text's first unit is nonzero. For 138, when sel
//    is above zero it decrements sel, calls ResetTickPulse__10VIUIWindow(this+776) and SoundPlay(mUI, 11);
//    when sel is zero or below it calls SoundPlay(mUI, 9) only. For 139, the mirror: when sel is zero or
//    below it increments sel, ResetTickPulse(this+776), SoundPlay(mUI, 11); otherwise SoundPlay(mUI, 9).
//  - 134: when m is 4 and the word at this+1164 is zero: if sel is zero, store 1 at (this+2312)+28,
//    SetFocus__5VIWndP5VIWndb(this, this+2312, true), SoundPlay(mUI, 15); if sel is nonzero, SoundPlay(mUI, 11)
//    when the word at this+1156 is zero, then SetFocus(this, 0, true). Otherwise, when m is 2 or 7: store 1
//    at this+1156, SoundPlay(mUI, 9), SetFocus(this, 0, true).
//  - 132: when m is 4 and the words at this+1164 and this+1156 are both zero: store 1 at this+1160,
//    SoundPlay(mUI, 15); fetch two strings with GetString__13VIStringTableii(mUI, 785, word at mUI+48) and
//    (mUI, 810, word at mUI+48); copy the two words at this+788 to a stack pair; call
//    Setup__12VIWndMessage(this+1176, first, second, 2, &pair, 400); store 1 at (this+1176)+28 and call
//    SetFocus(this, this+1176, true).
//  - any other key: nothing.
// Inferred: 138/139 are up and down, 134 confirm, 132 a secondary button; m is a screen mode (4 = a
// two-choice prompt with a message window; 2 and 7 are modes where confirm just finishes); the word at +28 of
// a child window acts as its shown flag, as read by OnDraw__5VIWnd. The GetString first argument is the
// UI pointer itself as in the image; this is passed through unchanged.
// Unknown: key names, the meaning of the words at +1156, +1160 and +1164, and the type of the UI pointer
// that GetString receives.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<__UINTPTR_TYPE__>(norrath_host_pointer(word))
#else
#define NORRATH_WORD_ADDRESS(word) static_cast<__UINTPTR_TYPE__>(word)
#endif
extern "C" {
extern unsigned _5VIWnd$mUI;
const unsigned short* Message__C16VIWndReadMessage(const void* window);
void SoundPlay__4VIUI11VIUISoundId(unsigned ui, int sound);
void SetFocus__5VIWndP5VIWndb(void* self, void* window, bool force);
void ResetTickPulse__10VIUIWindow(void* window);
const unsigned short* GetString__13VIStringTableii(void* table, int id, int arg);

struct Pair {
    int a, b;
};
void Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(void* window, const unsigned short* first,
                                                              const unsigned short* second, int prompt,
                                                              const Pair* rect, int size);

static inline int& word(void* base, int offset) {
    return *reinterpret_cast<int*>(static_cast<unsigned char*>(base) + offset);
}

void OnKeyDown__12VIWndPatcheri(void* self, int key) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    const int mode = word(self, 1148);
    switch (key) {
    case 138:
        if (mode != 4) return;
        if (*Message__C16VIWndReadMessage(bytes + 2312) == 0) return;
        if (word(self, 1144) <= 0) {
            SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 9);
        } else {
            word(self, 1144) = word(self, 1144) - 1;
            ResetTickPulse__10VIUIWindow(bytes + 776);
            SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 11);
        }
        return;
    case 139:
        if (mode != 4) return;
        if (*Message__C16VIWndReadMessage(bytes + 2312) == 0) return;
        if (word(self, 1144) > 0) {
            SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 9);
        } else {
            word(self, 1144) = word(self, 1144) + 1;
            ResetTickPulse__10VIUIWindow(bytes + 776);
            SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 11);
        }
        return;
    case 134:
        if (mode == 4 && word(self, 1164) == 0) {
            if (word(self, 1144) == 0) {
                word(bytes + 2312, 28) = 1;
                SetFocus__5VIWndP5VIWndb(self, bytes + 2312, true);
                SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 15);
                return;
            }
            if (word(self, 1156) == 0) SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 11);
            SetFocus__5VIWndP5VIWndb(self, 0, true);
            return;
        }
        if (mode == 2 || mode == 7) {
            word(self, 1156) = 1;
            SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 9);
            SetFocus__5VIWndP5VIWndb(self, 0, true);
        }
        return;
    case 132:
        if (mode != 4) return;
        if (word(self, 1164) != 0) return;
        if (word(self, 1156) != 0) return;
        word(self, 1160) = 1;
        SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 15);
        {
            void* message = bytes + 1176;
            void* ui = reinterpret_cast<void*>(NORRATH_WORD_ADDRESS(_5VIWnd$mUI));
            const int arg = *reinterpret_cast<int*>(static_cast<unsigned char*>(ui) + 48);
            const unsigned short* first = GetString__13VIStringTableii(ui, 785, arg);
            const unsigned short* second = GetString__13VIStringTableii(ui, 810, arg);
            const Pair rect = *reinterpret_cast<const Pair*>(bytes + 788);
            Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii(message, first, second, 2, &rect, 400);
            word(message, 28) = 1;
            SetFocus__5VIWndP5VIWndb(self, message, true);
        }
        return;
    default:
        return;
    }
}
}

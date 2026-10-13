// Observed: OnKeyDown__12VIWndMessagei at 0x010c1198 is 336 bytes. With (this, key), n is the word at +984 and
// mUI is the word _5VIWnd$mUI (0x01192c70):
//  - key 136: nothing when n <= 0; otherwise n is decremented, ResetTickPulse__10VIUIWindow(this + 40) is called
//    and SoundPlay__4VIUI11VIUISoundId(mUI, 11) is tail-called;
//  - key 137: nothing unless n < 0, or n <= 0 when the word at +1004 is nonzero; otherwise n is incremented and
//    the same two calls follow;
//  - key 134: the word at +28 is cleared; the object P at the word +20 is called through the table word at
//    P + 32: the entry at byte offset 48 (halfword adjustment, then function address) is called with
//    (P + adjustment, this, 307, n, 0). Then SoundPlay(mUI, 13) when the word at +1004 is zero, else
//    SoundPlay(mUI, 8) when the re-read n is nonzero and 13 when it is zero;
//  - every other key does nothing.
// Inferred: n is a two-way selection (0 or 1; +1004 says whether the second choice exists); 134 confirms the
// choice by sending message 307 (the number the generic renderer also handles) to the word at +20; the sound
// numbers are plain integers (no names are known).
// Unknown: key names, the meaning of 307, and the return type (void).
extern "C" {
void ResetTickPulse__10VIUIWindow(void* window);
void SoundPlay__4VIUI11VIUISoundId(const void* ui, int sound);
extern unsigned _5VIWnd$mUI;

struct VtEntry {
    short delta;
    short index;
    void (*function)(void*, void*, int, int, void*);
};

static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(static_cast<unsigned long>(word));
}
static inline void play(int sound) {
    SoundPlay__4VIUI11VIUISoundId(pointer_of(_5VIWnd$mUI), sound);
}

void OnKeyDown__12VIWndMessagei(void* self, int key) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    int* words = static_cast<int*>(self);
    if (key == 136) {
        const int n = words[246];
        if (n <= 0) return;
        words[246] = n - 1;
        ResetTickPulse__10VIUIWindow(bytes + 40);
        play(11);
    } else if (key == 137) {
        const int n = words[246];
        if (words[251] != 0 ? n > 0 : n >= 0) return;
        words[246] = n + 1;
        ResetTickPulse__10VIUIWindow(bytes + 40);
        play(11);
    } else if (key == 134) {
        unsigned char* parent = static_cast<unsigned char*>(pointer_of(static_cast<unsigned>(words[5])));
        words[7] = 0;
        const VtEntry* entry = reinterpret_cast<const VtEntry*>(
            static_cast<unsigned char*>(pointer_of(*reinterpret_cast<unsigned*>(parent + 32))) + 48);
        entry->function(parent + entry->delta, self, 307, words[246], nullptr);
        if (words[251] != 0) play(words[246] == 0 ? 13 : 8);
        else play(13);
    }
}
}

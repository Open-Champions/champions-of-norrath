// Observed: Free__12VIWndOptions at 0x010c6360 is 104 bytes. With the object as this and the
// embedded message window at this+1232: if the word at (this+1232)+20 equals this, it calls
// RemoveChild__5VIWndP5VIWnd(this, this+1232) and Free__12VIWndMessage(this+1232). Then if the word
// at this+1196 is nonzero it calls RemoveChild__5VIWndP5VIWnd(this, word) and stores 0 there.
// Finally it calls Free__13VIBattleMusic(this+1200). Nothing is returned.
// Inferred: the word at +20 of a window is its parent. Unknown: what the word at +1196 is.
extern "C" {
void RemoveChild__5VIWndP5VIWnd(void* self, void* child);
void Free__12VIWndMessage(void* self);
void Free__13VIBattleMusic(void* self);

static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word));
}

void Free__12VIWndOptions(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    unsigned char* message = bytes + 1232;
    if (*reinterpret_cast<unsigned*>(message + 20) == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(self))) {
        RemoveChild__5VIWndP5VIWnd(self, message);
        Free__12VIWndMessage(message);
    }
    if (unsigned child = *reinterpret_cast<unsigned*>(bytes + 1196)) {
        RemoveChild__5VIWndP5VIWnd(self, pointer_of(child));
        *reinterpret_cast<unsigned*>(bytes + 1196) = 0;
    }
    Free__13VIBattleMusic(bytes + 1200);
}
}

// Observed: Update__15VIWindowManager at 0x0109b350 is 680 bytes. With the manager as this and
// the focus word mFocus (_5VIWnd$mFocus, 0x01192c74), in order:
//  - If mFocus is nonzero and the keyboard word at this+44 is nonzero, it calls
//    Read__10VIKeyboardR12VIConsoleKey(keyboard, &key) with a 16-byte-or-smaller buffer on the
//    stack. If the result is 1, it re-reads mFocus and, when still nonzero, calls the table entry
//    at byte offset 40 (OnKeyDown__5VIWndi in the base table) when key byte 1 is nonzero, else the
//    entry at offset 32 (OnKeyUp__5VIWndi), with the key's byte 0 as the argument. Table words
//    sit at object+32 and each entry's halfword adjustment is added to the object.
//  - Then, if mFocus is nonzero and the pad word at this+40 is nonzero, it calls
//    Read__5VIPadR12VIConsolePad(pad, &now) where the buffer is 84 bytes: a word followed by 20
//    floats. If the result is nonzero it compares, for i = 0 to 19, the float at this+52+4*i (the
//    previous state) with now's float i. If exactly one of them is nonzero, it re-reads mFocus
//    and, when nonzero, calls OnKeyDown (offset 40) if the new float is nonzero, else OnKeyUp
//    (offset 32), with argument i + 128. Finally the 84 bytes of now are copied to this+48.
//  - Then, whatever happened above (including when mFocus was null), if the word at this+36 (the
//    root set by SetRoot__15VIWindowManagerP5VIWnd) is nonzero and the word at root+28 is
//    nonzero, the root's table entry at offset 16 (OnDraw__5VIWnd in the base table) is called
//    with the adjusted root only.
//  - It returns 1 when mFocus is nonzero (read after the calls), else 0.
// Inferred: Init__15VIWindowManagerP5VIPadP10VIKeyboard clears the same 84 bytes at this+48; the
// names OnKeyDown, OnKeyUp and OnDraw come from the base class table symbols and are not
// otherwise confirmed for derived classes. Unknown: the layout of VIConsoleKey beyond bytes 0
// and 1, and the meaning of the first word of VIConsolePad.
extern "C" {
struct VIConsoleKey {
    unsigned char code;
    unsigned char pressed;
    unsigned char rest[14];
};
struct VIConsolePad {
    unsigned head;
    float value[20];
};
struct VtEntry {
    short delta;
    short index;
    void (*function)(void*, int);
};

int Read__10VIKeyboardR12VIConsoleKey(void* keyboard, VIConsoleKey* key);
int Read__5VIPadR12VIConsolePad(void* pad, VIConsolePad* state);
extern unsigned _5VIWnd$mFocus;

static inline void* pointer_of(unsigned word) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word));
}
static inline void call_entry(void* target, int entry_index, int argument) {
    unsigned char* object = static_cast<unsigned char*>(target);
    const VtEntry* entry =
        reinterpret_cast<const VtEntry*>(pointer_of(*reinterpret_cast<unsigned*>(object + 32))) + entry_index;
    entry->function(object + entry->delta, argument);
}

int Update__15VIWindowManager(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    if (_5VIWnd$mFocus != 0) {
        const unsigned keyboard = *reinterpret_cast<unsigned*>(bytes + 44);
        if (keyboard != 0) {
            VIConsoleKey key;
            if (Read__10VIKeyboardR12VIConsoleKey(pointer_of(keyboard), &key) == 1) {
                if (key.pressed != 0) {
                    if (_5VIWnd$mFocus != 0) call_entry(pointer_of(_5VIWnd$mFocus), 5, key.code);
                } else {
                    if (_5VIWnd$mFocus != 0) call_entry(pointer_of(_5VIWnd$mFocus), 4, key.code);
                }
            }
        }
        const unsigned pad = *reinterpret_cast<unsigned*>(bytes + 40);
        if (pad != 0) {
            VIConsolePad now;
            if (Read__5VIPadR12VIConsolePad(pointer_of(pad), &now) != 0) {
                VIConsolePad* const previous = reinterpret_cast<VIConsolePad*>(bytes + 48);
                for (int i = 0; i < 20; ++i) {
                    const bool was_down = previous->value[i] != 0.0f;
                    const bool is_down = now.value[i] != 0.0f;
                    if (was_down != is_down) {
                        if (!(now.value[i] == 0.0f)) {
                            if (_5VIWnd$mFocus != 0) call_entry(pointer_of(_5VIWnd$mFocus), 5, i + 128);
                        } else {
                            if (_5VIWnd$mFocus != 0) call_entry(pointer_of(_5VIWnd$mFocus), 4, i + 128);
                        }
                    }
                }
                *previous = now;
            }
        }
    }
    const unsigned root = *reinterpret_cast<unsigned*>(bytes + 36);
    if (root != 0 && *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(pointer_of(root)) + 28) != 0) {
        unsigned char* object = static_cast<unsigned char*>(pointer_of(root));
        const VtEntry* entry =
            reinterpret_cast<const VtEntry*>(pointer_of(*reinterpret_cast<unsigned*>(object + 32))) + 2;
        reinterpret_cast<void (*)(void*)>(entry->function)(object + entry->delta);
    }
    return _5VIWnd$mFocus != 0;
}
}

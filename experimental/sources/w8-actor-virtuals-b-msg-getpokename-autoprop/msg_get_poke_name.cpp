// Observed msg_getPokeName__8AutoPropPUsi at 0x00220a38, 392 bytes.
// Returns 0 immediately when bit 4 (value 0x10) of the word at object+284 is clear.
// Otherwise it walks the object's "key" tags: for index 0, 1, ... while findTagString
// finds a tag, it calls scriptStart on that tag and stops at the first call that
// returns 0 (locked).
//   Locked: if a "lockMessage" tag exists, its text-table entry is copied into the
//     16-bit name buffer; otherwise a diagnostic "<name>: Missing lock message." is
//     formatted into a 64-byte local (never used afterwards) and the object's name
//     text at object+76 is copied into the buffer with the narrow-to-wide copy.
//     Returns 0 either way.
//   Unlocked (no more key tags): if a "message" tag exists, its text-table entry is
//     copied and 1 is returned. Otherwise the tag named "message<N>", where N is the
//     word at object+376, is tried the same way. With neither tag, returns 0.
// The image keeps these text constants in its shared string area (0x485550, 0x470f38,
// 0x470f48, 0x470f68, 0x485558); this source holds its own copies.
// Unknown: what scriptStart's result means beyond zero/nonzero, and what the +376 word is.

extern "C" {
char* findTagString__10GameObjectPci(void* object, const char* tag, int index);
int scriptStart__FPc(char* script);
unsigned short* textGetString__FPc(char* key);
void ustrcpy__FPUsT0(unsigned short* destination, const unsigned short* source);
void ustrcpy__FPUsPc(unsigned short* destination, const char* source);
int format_text(char* buffer, const char* format, ...) asm("sprintf");
}

extern "C" int msg_getPokeName__8AutoPropPUsi(void* self, unsigned short* name, int index) {
    (void)index;
    typedef int Word __attribute__((may_alias));
    unsigned char* object = static_cast<unsigned char*>(self);
    if ((*reinterpret_cast<const Word*>(object + 284) & 0x10) == 0) return 0;
    int tag_index = 0;
    for (;;) {
        char* key = findTagString__10GameObjectPci(self, "key", tag_index);
        ++tag_index;
        if (key == nullptr) break;
        if (scriptStart__FPc(key) == 0) {
            char* text = findTagString__10GameObjectPci(self, "lockMessage", 0);
            if (text != nullptr) {
                ustrcpy__FPUsT0(name, textGetString__FPc(text));
            } else {
                char diagnostic[64];
                format_text(diagnostic, "%s: Missing lock message.", reinterpret_cast<char*>(object + 76));
                ustrcpy__FPUsPc(name, reinterpret_cast<const char*>(object + 76));
            }
            return 0;
        }
    }
    char numbered[64];
    format_text(numbered, "message%d", *reinterpret_cast<const Word*>(object + 376));
    if (findTagString__10GameObjectPci(self, "message", 0) != nullptr) {
        ustrcpy__FPUsT0(name, textGetString__FPc(findTagString__10GameObjectPci(self, "message", 0)));
        return 1;
    }
    if (findTagString__10GameObjectPci(self, numbered, 0) != nullptr) {
        ustrcpy__FPUsT0(name, textGetString__FPc(findTagString__10GameObjectPci(self, numbered, 0)));
        return 1;
    }
    return 0;
}

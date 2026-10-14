// Observed init__4itemv at 0x00107b68, 384 bytes. Two table passes.
//   1. _4item$globalItem (0x0031f540) is an array of 16-byte entries ending with an
//      entry whose first word is zero. For each entry, with L the lump name
//      "equipmnt.lmp": word +8 = lumpFindResource__FPcT0(L, entry word +0) and word
//      +12 = lumpFindResource__FPcT0(L, entry word +4), the first lookup first.
//   2. _4item$itemTable (0x0032b320) is an array of 64-byte entries ending with an
//      entry whose first word is zero. For each entry a 72-byte local description
//      is prepared: bytes +32 and +33 = 0xff; the halfwords at +36, +44, +52, +60
//      (the four 8-byte modifier records' ids) = -1; the halfwords at +16..+22 and
//      +24..+30 = 0. Then create__4itemPcPQ24item11descriptionii(entry word +0,
//      description, -1, 0) runs; if the signed byte at entry +30 is 4, entry word
//      +56 = lumpFindResource__FPcT0(L, "compbow_shot.anm"). Last, the halfword at
//      description +4 is set to 1 (it is not set before the first call, and nothing
//      reads the description afterwards except the next iteration's create call).
// Unknown: the meaning of the entry fields beyond their use here, and whether the
// last store is a leftover. The two tables are initialized data not authored here.
extern "C" {
extern unsigned char _4item$globalItem[];
extern unsigned char _4item$itemTable[];
void* lumpFindResource__FPcT0(char*, char*);
void* create__4itemPcPQ24item11descriptionii(char*, unsigned char*, int, int);

static inline unsigned& field(unsigned char* base, int off) { return *reinterpret_cast<unsigned*>(base + off); }
static inline char* text(unsigned word) { return reinterpret_cast<char*>(static_cast<__UINTPTR_TYPE__>(word)); }
static inline unsigned handle(void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }

void init__4itemv() {
    unsigned char description[72];
    char* lump = const_cast<char*>("equipmnt.lmp");
    for (unsigned char* e = _4item$globalItem; field(e, 0) != 0; e += 16) {
        field(e, 8) = handle(lumpFindResource__FPcT0(lump, text(field(e, 0))));
        field(e, 12) = handle(lumpFindResource__FPcT0(lump, text(field(e, 4))));
    }
    for (unsigned char* e = _4item$itemTable; field(e, 0) != 0; e += 64) {
        for (int i = 0; i < 4; i++) *reinterpret_cast<short*>(description + 36 + 8 * i) = -1;
        description[32] = 0xff;
        description[33] = 0xff;
        for (int i = 0; i < 4; i++) {
            *reinterpret_cast<short*>(description + 16 + 2 * i) = 0;
            *reinterpret_cast<short*>(description + 24 + 2 * i) = 0;
        }
        create__4itemPcPQ24item11descriptionii(text(field(e, 0)), description, -1, 0);
        if (static_cast<signed char>(e[30]) == 4) {
            field(e, 56) = handle(lumpFindResource__FPcT0(lump, const_cast<char*>("compbow_shot.anm")));
        }
        *reinterpret_cast<short*>(description + 4) = 1;
    }
}
}

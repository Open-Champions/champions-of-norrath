// Observed mtcInitialize__Fv at 0x00266ee8, 316 bytes. Its only authored caller is the
// megatile in-place initializer, which calls it last.
// The word mgrg (0x00485c54) points to a loaded megatile block. From byte 784 of that
// block there is a run of 16-byte records, at most eight, ending early at the first
// record whose first (signed) byte is zero. A record is:
//   +0  signed byte kind
//   +4  a string pointer when kind is 1, else two halfwords (modulus, remainder) when kind is 3
//   +8  word copied to mtcStart when the record is chosen
//   +12 word copied to mtcExit when the record is chosen
// Records are tried in order; the first chosen one stores mtcExit then mtcStart
// (0x00485c7c, 0x00485c78) and the routine returns. The test per kind:
//   kind 1: chosen when strcmp(levelName + 161, string) is zero.
//   kind 3: chosen when the first byte of levelName + 161 is a digit (ctype bit 4 of
//           _ctype_[1 + c]) and atoi(levelName + 161) % modulus == remainder (signed
//           halfwords, signed remainder, no zero test on the modulus).
//   any other kind (0 does not occur in the loop, negative, 2, 4 and above): chosen.
// An unchosen record moves on to the next; with none chosen nothing is stored.
// The global mgrg is read again after each call, as in the image.
// Inferred: levelName + 161 is the second of two 161-byte strings in the 322-byte
// levelName buffer (the draw-dome and HUD-message sources use the offsets 80 and 81 of
// the same buffer). In the image the chosen-after-strcmp path reuses the register that
// held the first read of mgrg across the call; this version re-reads the global instead,
// which differs only if strcmp changed that register or the global.
// Unknown: what the kinds mean, and the names of the record fields.
extern "C" {
extern void* mgrg;
extern char levelName[];
extern int mtcStart;
extern int mtcExit;
extern const unsigned char _ctype_[];
int strcmp(const char* left, const char* right);
int atoi(const char* text);
}

struct MtcRecord {
    signed char kind;
    unsigned char pad[3];
    union {
        const char* name;
        struct {
            short modulus;
            short remainder;
        } wrap;
    } arg;
    int start;
    int exit;
};
struct MtcBlock {
    unsigned char before_records[784];
    MtcRecord records[8];
};
#if defined(__mips__)
static_assert(sizeof(MtcRecord) == 16 && __builtin_offsetof(MtcRecord, arg) == 4 && __builtin_offsetof(MtcRecord, start) == 8 &&
                  __builtin_offsetof(MtcRecord, exit) == 12 && __builtin_offsetof(MtcBlock, records) == 784,
              "observed record layout");
#endif

namespace {
MtcBlock* block() { return static_cast<MtcBlock*>(mgrg); }
}

extern "C" void mtcInitialize__Fv() {
    if (block()->records[0].kind == 0) return;
    int index = 0;
    for (;;) {
        const MtcRecord* record = &block()->records[index];
        const signed char kind = record->kind;
        bool chosen;
        if (kind == 1) {
            chosen = strcmp(levelName + 161, record->arg.name) == 0;
        } else if (kind == 3) {
            chosen = false;
            if ((_ctype_[1 + levelName[161]] & 4u) != 0u) {
                const int value = atoi(levelName + 161);
                record = &block()->records[index];
                chosen = value % record->arg.wrap.modulus == record->arg.wrap.remainder;
            }
        } else {
            chosen = true;
        }
        if (chosen) {
            record = &block()->records[index];
            mtcExit = record->exit;
            mtcStart = record->start;
            return;
        }
        if (++index >= 8) return;
        if (block()->records[index].kind == 0) return;
    }
}

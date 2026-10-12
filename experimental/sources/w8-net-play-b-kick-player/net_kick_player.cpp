// Observed netKickPlayer at 0x0012a190, 184 bytes. Nothing happens unless the
// argument is below nmPartners (signed) and netGetLocalPlayerNm(0) returns
// zero. For that partner record (stride 17632) the signed bytes at 17620,
// 17621 and 17622 must all be zero; if any is nonzero the function returns.
// Otherwise it stores byte 1 at 17622, word -1 at 17624, word
// 1 at 17628, zeroes the eight bytes kickState with memset (after the partner stores), then sets the
// first word of kickState to 1.
// Inferred: kickState is a two-word object; the stores mark the partner as
// being kicked (byte 17622 is also tested by netInContact).
// Unknown: who consumes kickState and the words at 17624 and 17628.
extern "C" {
extern int nmPartners;
extern unsigned char partners[];
extern unsigned kickState[2];
int netGetLocalPlayerNm__Fi(int player);
void* memset(void* destination, int value, unsigned long length);
}

extern "C" void netKickPlayer__Fi(int player) {
    if (!(player < nmPartners)) return;
    if (netGetLocalPlayerNm__Fi(0) != 0) return;
    unsigned char* record = partners + player * 17632;
    if (static_cast<signed char>(record[17620]) != 0) return;
    if (static_cast<signed char>(record[17621]) != 0) return;
    if (static_cast<signed char>(record[17622]) != 0) return;
    record[17622] = 1;
    *reinterpret_cast<int*>(record + 17624) = -1;
    *reinterpret_cast<int*>(record + 17628) = 1;
    memset(kickState, 0, 8);
    kickState[0] = 1;
}

// Observed netInContact at 0x00135278, 108 bytes. It walks the first
// nmPartners partner records (stride 17632) in order. A record is accepted if
// any of the signed bytes at 17620, 17621, 17622 is nonzero; otherwise it is
// accepted only if the word at 17616 is nonzero, and a zero word makes the
// function return 0 at once. If every record is accepted, or nmPartners is not
// positive, it returns 1. netClearContact clears the word at 17616 and
// netDisconnectPlayer sets the byte at 17621.
// Inferred: bytes 17620..17622 are other reasons a peer needs no contact
// (for example already disconnected); the first is unexplained.
// Unknown: the meaning of each field.
extern "C" {
extern int nmPartners;
extern unsigned char partners[];
}

extern "C" int netInContact__Fv() {
    int count = nmPartners;
    for (int i = 0; i < count; ++i) {
        const unsigned char* record = partners + i * 17632;
        if (static_cast<signed char>(record[17620]) != 0) continue;
        if (static_cast<signed char>(record[17621]) != 0) continue;
        if (static_cast<signed char>(record[17622]) != 0) continue;
        if (*reinterpret_cast<const int*>(record + 17616) == 0) return 0;
    }
    return 1;
}

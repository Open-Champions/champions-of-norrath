// Observed netGetNmConnectedPartners__Fv at 0x001352e8, 76 bytes. It walks the
// first nmPartners records of the 17632-byte partner array (a signed count; zero
// or less returns 0) and counts the records whose bytes at offsets 17621 and
// 17622 are both zero. The byte at 17621 is the one netPlayerIsConnected reads.
// Inferred: the byte at 17622 is a second per-partner mark that excludes a
// record from the count; its meaning is not established.
// Unknown: what writes the byte at 17622.
extern "C" {
extern int nmPartners;
extern unsigned char partners[];
}

extern "C" int netGetNmConnectedPartners__Fv() {
    int count = 0;
    for (int i = 0; i < nmPartners; ++i) {
        const unsigned char* record = partners + i * 17632;
        if (static_cast<signed char>(record[17621]) == 0 &&
            static_cast<signed char>(record[17622]) == 0)
            count = count + 1;
    }
    return count;
}

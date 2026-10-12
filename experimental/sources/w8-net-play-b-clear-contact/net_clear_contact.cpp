// Observed netClearContact at 0x00135238, 60 bytes. For each of the first
// nmPartners partner records (stride 17632) it stores zero to the word at
// offset 17616. Nothing runs when nmPartners is not positive. netInContact
// reads the same word.
// Inferred: none. Unknown: the meaning of the word.
extern "C" {
extern int nmPartners;
extern unsigned char partners[];
}

extern "C" void netClearContact__Fv() {
    int count = nmPartners;
    for (int i = 0; i < count; ++i)
        *reinterpret_cast<int*>(partners + i * 17632 + 17616) = 0;
}

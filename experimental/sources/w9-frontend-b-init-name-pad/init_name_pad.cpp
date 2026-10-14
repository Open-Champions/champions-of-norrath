// Observed: initNamePad__Fv at 0x0027f1b0 is 268 bytes and calls nothing. It
// fills the byte array fontGrid (at 0x008d5ab8, 128 bytes in the image's symbol
// gap) from index 0: the 26 letters 'a' to 'z', then the 10 digits '0' to '9',
// then the bytes of a 28-character string copied from read-only data (all
// Latin-1 accented letters: C1 C4 C8 C9 D3 D6 DC DF E0 E1 E2 E4 E7 E8 E9 EA EC
// ED EF F1 F2 F3 F4 F6 F9 FA FB FC), stopping at its terminator. It stores the
// number of bytes written in nameGridCount and 10 in nameGridW. nameGridCount is
// zero while the first loop starts and is updated after each of the three
// groups.
// Inferred: fontGrid is the character grid of the name entry pad and nameGridW
// is its row width, from the names and the 10 columns that match the digit row.
// Unknown: the encoding the font uses for the accented bytes and whether the
// grid is read anywhere past nameGridCount.

extern "C" unsigned char fontGrid[];
extern "C" int nameGridCount;
extern "C" int nameGridW;

extern "C" void initNamePad__Fv() {
    char extra[] = "\xc1\xc4\xc8\xc9\xd3\xd6\xdc\xdf\xe0\xe1\xe2\xe4\xe7\xe8\xe9\xea\xec\xed\xef\xf1\xf2\xf3\xf4\xf6\xf9\xfa\xfb\xfc";
    int n = 0;
    nameGridCount = 0;
    for (int i = 0; i < 26; ++i) fontGrid[n++] = static_cast<unsigned char>('a' + i);
    nameGridCount = n;
    for (int i = 0; i < 10; ++i) fontGrid[n++] = static_cast<unsigned char>('0' + i);
    nameGridCount = n;
    for (int i = 0; extra[i] != 0; ++i) fontGrid[n++] = static_cast<unsigned char>(extra[i]);
    nameGridCount = n;
    nameGridW = 10;
}

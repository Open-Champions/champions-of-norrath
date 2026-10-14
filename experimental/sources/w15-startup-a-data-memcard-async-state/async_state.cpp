// Observed, from the pinned SLUS_205.65 symbol table: sAsyncMode (0x011e8fec) and sAsyncFd
// (0x011e8ff0) are adjacent local words of the memory-card file code, both zero in the image;
// sProductId (0x011afa78) is 32 bytes (the next symbol, sProductTitle, is 0x20 later) holding
// the 20-character text below followed by NULs. VIMemCardReadFileAsync formats
// "/%s/%s" with sProductId and the file name as the card path.
// Authored: two zeroed words and the 32-byte string under the image's local names; being
// authored as globals they are importable (the image keeps them local).
// Inferred: the product id text is the memory-card directory name of the save data.
// Unknown: the readers of sAsyncMode, and whether sProductId is written anywhere at run time.
extern "C" {
int sAsyncMode = 0;
int sAsyncFd = 0;
char sProductId[32] = "BASLUS-20470EQONLINE";
}

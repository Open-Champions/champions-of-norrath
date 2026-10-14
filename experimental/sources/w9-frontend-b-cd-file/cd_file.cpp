// Observed: gCdFile is a zero-initialised object at 0x011e7d68 in the pinned
// SLUS-20565 image's uninitialised data, followed by htmlStartCloseIndex at
// 0x011e7d90, so the space reserved for it is 40 bytes. The authored start-up
// code reads its first word after sceCdSearchFile(gCdFile, name) succeeds and
// passes it to sceCdRead.
// Inferred: it is a file-search result record filled in by sceCdSearchFile (the
// first word being the start sector); it is declared here as ten words, the
// whole reserved space.
// Unknown: the layout beyond the first word, and the real size of the record the
// disc library writes.
extern "C" {
unsigned gCdFile[10];
}

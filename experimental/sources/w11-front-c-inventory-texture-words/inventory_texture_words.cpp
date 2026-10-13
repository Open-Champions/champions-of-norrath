// Observed inventory interface texture words, all 4-byte pointers in .data that are zero in
// the file:
//   tinyIcon     0x004271e0  36 bytes (9 words), symbol size 0x24
//   selectionBox 0x00427208  16 bytes (4 words), symbol size 0x10
//   taskBar      0x00427218  48 bytes (12 words), symbol size 0x30
//   rcursor      0x004848e8  4 bytes, between lcursor and marble
// invInitSharedInterfaceTextures fills them with texture pointers found by name at load
// time; shopDrawTaskBar, displayItemStats and the inventory screens read them.
// Inferred: they are texture pointers (they receive FIND results and are passed to texture
// draws). Unknown: which index maps to which tab or icon beyond what the filler stores.
extern "C" {
void* tinyIcon[9];
void* selectionBox[4];
void* taskBar[12];
void* rcursor;
}

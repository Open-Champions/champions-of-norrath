// Observed zero-filled storage of the text and texture-page code, with the
// original symbol, address and size.
//   textBody                     0x0042a500  2048 bytes, file bytes all zero (.data)
//   textBodyChars                0x00486b6c     4 bytes, .sbss
//   textPageDone                 0x00486b30     4 bytes, .sbss
//   realDlgPos                   0x00486b88     4 bytes, .sbss
//   shopActiveFrame              0x00484910     4 bytes, file bytes zero (.sdata)
//   textureUncompBufferLocked    0x004840a2     1 byte, file byte zero (.sdata)
//   texturePageTable             0x0034c030 40000 bytes, file bytes all zero (.data)
//   texturePageLoc               0x00355c70    68 bytes, file bytes all zero (.data)
//   texturePageBlock             0x006e9000    68 bytes used, .bss
//   texturePageSegLoc            0x006e8680  2176 bytes, .bss
// The word sizes of the three .sbss items come from the gaps to their
// neighbouring symbols. The element types are not established by this unit:
// uses seen in textureClear, textureInternalInit and the text functions are
// bytes (texturePageTable, texturePageSegLoc), words (texturePageBlock,
// texturePageLoc) and 16-bit units (textBody, by the 2048-byte size and the
// text functions). Here they are built in the same section as plain zeroed C++
// objects, so the .data items land in .bss in this build.
extern "C" {
unsigned short textBody[1024];
int textBodyChars;
int textPageDone;
int realDlgPos;
int shopActiveFrame;
unsigned char textureUncompBufferLocked;
unsigned char texturePageTable[40000];
int texturePageLoc[17];
int texturePageBlock[17];
unsigned char texturePageSegLoc[17][128];
}

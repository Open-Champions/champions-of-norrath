extern "C" {
extern unsigned short textBody[1024];
extern int textBodyChars, textPageDone, realDlgPos, shopActiveFrame;
extern unsigned char textureUncompBufferLocked;
extern unsigned char texturePageTable[40000];
extern int texturePageLoc[17], texturePageBlock[17];
extern unsigned char texturePageSegLoc[17][128];
}
static_assert(sizeof textBody == 2048, "textBody is 0x800 bytes");
static_assert(sizeof texturePageTable == 0x9c40, "texturePageTable is 0x9c40 bytes");
static_assert(sizeof texturePageLoc == 0x44, "texturePageLoc is 0x44 bytes");
static_assert(sizeof texturePageBlock == 0x44, "texturePageBlock uses 0x44 bytes");
static_assert(sizeof texturePageSegLoc == 0x880, "texturePageSegLoc spans 0x6e8680..0x6e8f00");
int main() {
    for (unsigned short u : textBody) if (u) return 1;
    for (unsigned char b : texturePageTable) if (b) return 2;
    for (int w : texturePageLoc) if (w) return 3;
    for (int w : texturePageBlock) if (w) return 4;
    for (auto& row : texturePageSegLoc) for (unsigned char b : row) if (b) return 5;
    if (textBodyChars || textPageDone || realDlgPos || shopActiveFrame || textureUncompBufferLocked) return 6;
    return 0;
}

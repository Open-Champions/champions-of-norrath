extern "C" {
extern unsigned char texmap[0x1900];
extern unsigned char texdecompress[0x30];
extern unsigned nmLumps;
extern int uncompSpin;
extern short texmapstart, texmapend;
extern int megatile_ramkmax;
extern unsigned char uncompMap[0x2800];
extern unsigned char segmentUsed[0x100];
}
int main() {
    for (unsigned char b : texmap) if (b) return 1;
    for (unsigned char b : texdecompress) if (b) return 2;
    if (nmLumps || uncompSpin || texmapstart || texmapend || megatile_ramkmax) return 3;
    for (unsigned char b : uncompMap) if (b) return 4;
    for (unsigned char b : segmentUsed) if (b) return 5;
    if (sizeof(texmapstart) != 2 || sizeof(nmLumps) != 4 || sizeof(megatile_ramkmax) != 4) return 6;
    return 0;
}

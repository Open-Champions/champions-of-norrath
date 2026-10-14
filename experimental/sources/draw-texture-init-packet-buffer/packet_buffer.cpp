// Observed packetBuffer at 0x0037f9e0. spellAlpha begins at 0x00408560,
// 560000 bytes later, and those image bytes are zero. initSupersample
// writes the first 192 bytes at runtime. The reviewed -G0 profile places
// this zero object in BSS.

extern "C" {
alignas(16) unsigned char packetBuffer[560000];
}

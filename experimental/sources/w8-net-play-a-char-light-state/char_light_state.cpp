// Zero-initialized objects, extents taken from the distance to the next
// observed symbol (inferred sizes; the original declares size 0 for the small
// ones): charAmbient, charDirect, charDirect2 are 8 bytes each (0x486850,
// 0x486858, 0x486860); charScaleR/G/B are 4 bytes each (0x486868..0x486870);
// charDirectDir and charDirectDir2 are 16 bytes each (0x4f40d0, 0x4f40e0);
// frameStatePacket is 0x600 bytes (0x793860, next symbol taskList at 0x793e60);
// commandQueue is 0x1000 bytes (0x42f440) and is entirely zero in the image.
extern "C" {
unsigned char charAmbient[8] = {};
unsigned char charDirect[8] = {};
unsigned char charDirect2[8] = {};
float charScaleR = 0.0f;
float charScaleG = 0.0f;
float charScaleB = 0.0f;
unsigned char charDirectDir[16] = {};
unsigned char charDirectDir2[16] = {};
unsigned char frameStatePacket[0x600] = {};
alignas(16) unsigned char commandQueue[0x1000] = {};
}

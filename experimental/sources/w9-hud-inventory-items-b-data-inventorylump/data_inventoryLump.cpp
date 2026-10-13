// Observed inventoryLump at 0x00484820, an initialised data object. It is nm-sized 4; the image word is zero.
// The type and meaning are not established; the object is 4 bytes with the image contents.
extern "C" {
extern unsigned char inventoryLump[4];
alignas(4) unsigned char inventoryLump[4];
}

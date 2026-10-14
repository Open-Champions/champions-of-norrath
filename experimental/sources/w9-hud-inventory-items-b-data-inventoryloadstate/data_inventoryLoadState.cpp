// Observed inventoryLoadState at 0x00484824, an initialised data object. It is nm-sized 4; the image word is zero.
// The type and meaning are not established; the object is 4 bytes with the image contents.
extern "C" {
extern unsigned char inventoryLoadState[4];
alignas(4) unsigned char inventoryLoadState[4];
}

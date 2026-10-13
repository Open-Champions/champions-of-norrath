// Observed invGrid at 0x00484544, an initialised data object. It is nm-sized 4; the image word is zero. inventoryLoad__Fv stores a pointer 0x300000 above the cache buffer here.
// The type and meaning are not established; the object is 4 bytes with the image contents.
extern "C" {
extern unsigned char invGrid[4];
alignas(4) unsigned char invGrid[4];
}

// Observed hudHeaderUni at 0x004282e0, an initialised data object. It is nm-sized 0x100; the image bytes are all zero.
// The type and meaning are not established; the object is 256 bytes with the image contents.
extern "C" {
extern unsigned char hudHeaderUni[256];
alignas(4) unsigned char hudHeaderUni[256];
}

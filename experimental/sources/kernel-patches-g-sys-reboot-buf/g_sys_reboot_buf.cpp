// Observed: g_SysRebootBuf at 0x008e4548. The symbol size is 0; the next
// symbol, userarea, is 512 bytes later, which is also the byte count
// sceSifInitRebootNotify zero-fills. The image bytes are zero.
// Inferred: the extent is 512 bytes.
// Unknown: whether a reboot consumer reads it.
extern "C" {
alignas(8) unsigned char g_SysRebootBuf[512];
}

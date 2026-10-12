// Observed: g_AlarmBuf at 0x008e4140. The symbol size is 0; g_pFreeAlarm
// starts 1024 bytes later. _InitAlarm chains 64 entries at a 16-byte stride
// from it. The address is 64-byte aligned. The image bytes are zero.
// Inferred: the extent is 1024 bytes. Alignment 16 follows the entry stride.
// Unknown: none.
extern "C" {
alignas(16) unsigned char g_AlarmBuf[1024];
}

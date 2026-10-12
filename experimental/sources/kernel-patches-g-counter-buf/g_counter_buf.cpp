// Observed: g_CounterBuf at 0x008e2140. The symbol size is 0; g_AlarmBuf starts
// 8192 bytes later, which is also the byte count InitTimer zero-fills. The
// address is 64-byte aligned and InitTimer chains entries at a 64-byte
// stride. The image bytes are zero.
// Inferred: the extent is 8192 bytes. Alignment 64 follows the address.
// Unknown: none.
extern "C" {
alignas(64) unsigned char g_CounterBuf[8192];
}

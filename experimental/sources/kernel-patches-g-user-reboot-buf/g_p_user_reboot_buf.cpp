// Observed: g_pUserRebootBuf at 0x004459c8, a 4-byte word. The image bytes are
// zero. sceSifInitRebootNotify stores zero to it.
// Inferred: none.
// Unknown: what a nonzero value would point at.
extern "C" {
void* g_pUserRebootBuf = 0;
}

// Observed: g_iUserRebootSize at 0x004459cc, a 4-byte word. The image bytes are
// zero. sceSifInitRebootNotify stores zero to it.
// Inferred: none.
// Unknown: its unit.
extern "C" {
int g_iUserRebootSize = 0;
}

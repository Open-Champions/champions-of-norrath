// Observed: g_pFreeAlarm at 0x008e4540, a 4-byte word; g_SysRebootBuf starts
// 8 bytes later. _InitAlarm stores the address of g_AlarmBuf in it. The image
// bytes are zero.
// Inferred: none.
// Unknown: nothing reads it in the code selected so far.
extern "C" {
unsigned g_pFreeAlarm = 0;
}

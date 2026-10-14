// Observed _alarm_bind at 0x002d7f90, 64 bytes, a local routine of the SIF RPC
// library. It is the handler that _request_bind passes to iSetTimerAlarm (delay
// 0x800) when its first send fails, with the 64-byte reply packet as the argument.
//
// Only the fourth argument register is read: it is the reply packet the alarm was
// armed with. The routine sends it again with
// isceSifSendCmd(0x80000008, reply, 64, 0, 0, 0) and returns 0x800 when that
// send reports failure (zero) and 0 when it succeeds. The first three arguments are
// ignored. That the return value re-arms the alarm for the same delay is inferred
// from this pattern, not from the timer service, which is not authored here.
extern "C" {
int isceSifSendCmd(unsigned command, void* packet, int size, unsigned word_a, unsigned word_b, unsigned word_c);

unsigned _alarm_bind(int, unsigned short, unsigned short, void* reply) {
    return isceSifSendCmd(0x80000008u, reply, 64, 0, 0, 0) == 0 ? 0x800u : 0u;
}
}

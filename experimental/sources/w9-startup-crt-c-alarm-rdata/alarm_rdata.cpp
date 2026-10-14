// Observed _alarm_rdata at 0x002d7cd8, 64 bytes, a local routine of the SIF RPC
// library. It is the handler that _request_rdata passes to iSetTimerAlarm (delay
// 0x800) when its first send fails, with the 64-byte reply packet as the argument.
//
// Only the fourth argument register is read: it is the reply packet. The routine
// sends it again with isceSifSendCmd(0x80000008, reply, 64, reply +36, reply +40,
// reply +44), the three words _request_rdata stored there, and returns 0x800 when
// that send reports failure (zero) and 0 when it succeeds. The first three
// arguments are ignored. That the return value re-arms the alarm is inferred from
// this pattern, not from the timer service, which is not authored here.
extern "C" {
struct RpcDataReply {
    unsigned words_0_to_8[9];
    unsigned word_9;
    unsigned word_10;
    unsigned word_11;
};

#if defined(__mips__)
static_assert(__builtin_offsetof(RpcDataReply, word_9) == 36, "layout");
static_assert(__builtin_offsetof(RpcDataReply, word_11) == 44, "layout");
#endif

int isceSifSendCmd(unsigned command, void* packet, int size, unsigned word_a, unsigned word_b, unsigned word_c);

unsigned _alarm_rdata(int, unsigned short, unsigned short, RpcDataReply* reply) {
    return isceSifSendCmd(0x80000008u, reply, 64, reply->word_9, reply->word_10, reply->word_11) == 0 ? 0x800u : 0u;
}
}

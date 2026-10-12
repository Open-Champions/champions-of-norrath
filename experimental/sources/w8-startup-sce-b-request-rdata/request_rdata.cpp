// Observed _request_rdata at 0x002d7d18, 208 bytes, a local handler in the SIF RPC
// library. Arguments are an incoming packet (p) and the service list the handler
// was registered with (s).
//
// The reply packet comes from _sceRpcGetFPacket2(s, p.word4 >> 16) when bit 2 of
// p.word4 (offset 16) is set, and from _sceRpcGetFPacket(s) otherwise. It is
// filled with
//   reply +20 <- p +20     reply +28 <- p +28     reply +32 = 0x8000000c
//   reply +36 <- p +32     reply +40 <- p +36     reply +44 <- p +40
// and sent with isceSifSendCmd(0x80000008, reply, 64, p +32, p +36, p +40). A
// zero send result arms iSetTimerAlarm(0x800, _alarm_rdata, reply) and a nonzero
// result ends the handler.
extern "C" {
struct RpcDataPacket {
    unsigned words_0_to_3[4];
    unsigned flags;
    unsigned word_5;
    unsigned word_6;
    unsigned word_7;
    unsigned word_8;
    unsigned word_9;
    unsigned word_10;
};

struct RpcDataReply {
    unsigned words_0_to_4[5];
    unsigned word_5;
    unsigned word_6;
    unsigned word_7;
    unsigned command;
    unsigned word_9;
    unsigned word_10;
    unsigned word_11;
};

#if defined(__mips__)
static_assert(__builtin_offsetof(RpcDataPacket, flags) == 16, "layout");
static_assert(__builtin_offsetof(RpcDataPacket, word_10) == 40, "layout");
static_assert(__builtin_offsetof(RpcDataReply, command) == 32, "layout");
static_assert(__builtin_offsetof(RpcDataReply, word_11) == 44, "layout");
#endif

RpcDataReply* _sceRpcGetFPacket(void* services);
RpcDataReply* _sceRpcGetFPacket2(void* services, unsigned index);
int isceSifSendCmd(unsigned command, void* packet, int size, unsigned word_a, unsigned word_b, unsigned word_c);
int iSetTimerAlarm(unsigned time, void* handler, void* argument);
void _alarm_rdata();

void _request_rdata(RpcDataPacket* packet, void* services) {
    RpcDataReply* reply;
    if (packet->flags & 4u)
        reply = _sceRpcGetFPacket2(services, packet->flags >> 16);
    else
        reply = _sceRpcGetFPacket(services);
    reply->word_5 = packet->word_5;
    reply->word_7 = packet->word_7;
    reply->command = 0x8000000cu;
    reply->word_9 = packet->word_8;
    reply->word_10 = packet->word_9;
    reply->word_11 = packet->word_10;
    if (isceSifSendCmd(0x80000008u, reply, 64, packet->word_8, packet->word_9, packet->word_10) == 0)
        iSetTimerAlarm(0x800, reinterpret_cast<void*>(_alarm_rdata), reply);
}
}

// Observed _request_bind at 0x002d7fd0, 204 bytes, a local handler in the SIF RPC
// library. Arguments are an incoming bind packet and the service list the
// handler was registered with (called s here).
//
// It takes a 64-byte reply packet from _sceRpcGetFPacket(s) and fills it:
//   reply +28 <- packet +28     reply +20 <- packet +20     reply +32 = 0x80000009
//   found = _search_svdata(packet +32, s)
//   reply +36 = found, reply +40 = found +8, or both 0 when nothing is found.
// isceSifSendCmd(0x80000008, reply, 64, 0, 0, 0) sends it. A zero result from
// the send arms iSetTimerAlarm(0x800, _alarm_bind, reply) to try again later. A
// nonzero result ends the handler.
extern "C" {
struct RpcFoundBlock {
    unsigned word_0;
    unsigned word_1;
    unsigned word_2;
};

struct RpcBindReply {
    unsigned words_0_to_4[5];
    unsigned word_5;
    unsigned word_6;
    unsigned word_7;
    unsigned command;
    RpcFoundBlock* found;
    unsigned found_word_2;
};

struct RpcBindPacket {
    unsigned words_0_to_4[5];
    unsigned word_5;
    unsigned word_6;
    unsigned word_7;
    unsigned service_id;
};

#if defined(__mips__)
static_assert(__builtin_offsetof(RpcBindReply, command) == 32, "layout");
static_assert(__builtin_offsetof(RpcBindReply, found) == 36, "layout");
static_assert(__builtin_offsetof(RpcBindReply, found_word_2) == 40, "layout");
static_assert(__builtin_offsetof(RpcBindPacket, service_id) == 32, "layout");
#endif

RpcBindReply* _sceRpcGetFPacket(void* services);
RpcFoundBlock* _search_svdata(unsigned service_id, void* services);
int isceSifSendCmd(unsigned command, void* packet, int size, unsigned word_a, unsigned word_b, unsigned word_c);
int iSetTimerAlarm(unsigned time, void* handler, void* argument);
void _alarm_bind();

void _request_bind(RpcBindPacket* packet, void* services) {
    RpcBindReply* reply = _sceRpcGetFPacket(services);
    reply->word_7 = packet->word_7;
    reply->word_5 = packet->word_5;
    reply->command = 0x80000009u;
    RpcFoundBlock* found = _search_svdata(packet->service_id, services);
    if (found != 0) {
        reply->found = found;
        reply->found_word_2 = found->word_2;
    } else {
        reply->found = 0;
        reply->found_word_2 = 0;
    }
    if (isceSifSendCmd(0x80000008u, reply, 64, 0, 0, 0) == 0)
        iSetTimerAlarm(0x800, reinterpret_cast<void*>(_alarm_bind), reply);
}
}

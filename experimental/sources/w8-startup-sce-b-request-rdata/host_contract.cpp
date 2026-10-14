#include "request_rdata.cpp"

#include <cstdio>
#include <cstring>

static RpcDataReply plain_reply, indexed_reply;
static int plain_calls, indexed_calls;
static void* services_plain;
static void* services_indexed;
static unsigned index_seen;
static int send_result;
static unsigned sent_command, sent_extra[3];
static void* sent_packet;
static int sent_size;
static RpcDataReply snapshot;
static int alarms;
static unsigned alarm_time;
static void* alarm_handler;
static void* alarm_argument;

extern "C" RpcDataReply* _sceRpcGetFPacket(void* services) {
    ++plain_calls; services_plain = services;
    std::memset(&plain_reply, 0xee, sizeof plain_reply);
    return &plain_reply;
}
extern "C" RpcDataReply* _sceRpcGetFPacket2(void* services, unsigned index) {
    ++indexed_calls; services_indexed = services; index_seen = index;
    std::memset(&indexed_reply, 0xdd, sizeof indexed_reply);
    return &indexed_reply;
}
extern "C" int isceSifSendCmd(unsigned command, void* packet, int size, unsigned a, unsigned b, unsigned c) {
    sent_command = command; sent_packet = packet; sent_size = size;
    sent_extra[0] = a; sent_extra[1] = b; sent_extra[2] = c;
    std::memcpy(&snapshot, packet, sizeof snapshot);
    return send_result;
}
extern "C" int iSetTimerAlarm(unsigned time, void* handler, void* argument) {
    ++alarms; alarm_time = time; alarm_handler = handler; alarm_argument = argument; return 0;
}
extern "C" void _alarm_rdata() {}

int main() {
    int services;
    RpcDataPacket packet = {};
    packet.word_5 = 0x50; packet.word_7 = 0x70; packet.word_8 = 0x80; packet.word_9 = 0x90; packet.word_10 = 0xa0;

    // Bit 2 clear: plain reply packet, the high half of the flags is ignored.
    packet.flags = 0x00030003u;
    send_result = 1;
    _request_rdata(&packet, &services);
    if (plain_calls != 1 || indexed_calls != 0 || services_plain != &services) return 1;
    if (snapshot.word_5 != 0x50 || snapshot.word_7 != 0x70 || snapshot.command != 0x8000000cu) return 2;
    if (snapshot.word_9 != 0x80 || snapshot.word_10 != 0x90 || snapshot.word_11 != 0xa0) return 3;
    if (sent_command != 0x80000008u || sent_packet != &plain_reply || sent_size != 64) return 4;
    if (sent_extra[0] != 0x80 || sent_extra[1] != 0x90 || sent_extra[2] != 0xa0 || alarms != 0) return 5;

    // Bit 2 set: indexed packet with flags >> 16.
    packet.flags = 0x00070004u;
    _request_rdata(&packet, &services);
    if (indexed_calls != 1 || services_indexed != &services || index_seen != 7 || plain_calls != 1) return 6;
    if (sent_packet != &indexed_reply || snapshot.command != 0x8000000cu) return 7;
    packet.flags = 0xffff0004u;
    _request_rdata(&packet, &services);
    if (index_seen != 0xffffu) return 8;

    // Refused send arms the retry on the reply that was built.
    send_result = 0;
    packet.flags = 0;
    _request_rdata(&packet, &services);
    if (alarms != 1 || alarm_time != 0x800 || alarm_argument != &plain_reply) return 9;
    if (alarm_handler != reinterpret_cast<void*>(_alarm_rdata)) return 10;
    std::puts("_request_rdata passed");
    return 0;
}

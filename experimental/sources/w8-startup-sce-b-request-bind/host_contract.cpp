#include "request_bind.cpp"

#include <cstdio>
#include <cstring>

static RpcBindReply reply_storage;
static void* services_seen;
static void* services_seen_search;
static unsigned searched_id;
static RpcFoundBlock* search_result;
static int send_result;
static unsigned sent_command;
static void* sent_packet;
static int sent_size;
static unsigned extras[3];
static int alarms;
static unsigned alarm_time;
static void* alarm_handler;
static void* alarm_argument;
static RpcBindReply snapshot;

extern "C" RpcBindReply* _sceRpcGetFPacket(void* services) {
    services_seen = services;
    std::memset(&reply_storage, 0xab, sizeof reply_storage);
    return &reply_storage;
}
extern "C" RpcFoundBlock* _search_svdata(unsigned id, void* services) {
    searched_id = id;
    services_seen_search = services;
    return search_result;
}
extern "C" int isceSifSendCmd(unsigned command, void* packet, int size, unsigned a, unsigned b, unsigned c) {
    sent_command = command; sent_packet = packet; sent_size = size;
    extras[0] = a; extras[1] = b; extras[2] = c;
    std::memcpy(&snapshot, packet, sizeof snapshot);
    return send_result;
}
extern "C" int iSetTimerAlarm(unsigned time, void* handler, void* argument) {
    ++alarms; alarm_time = time; alarm_handler = handler; alarm_argument = argument; return 0;
}
extern "C" void _alarm_bind() {}

int main() {
    int services;
    RpcBindPacket packet = {};
    packet.word_5 = 0x55; packet.word_7 = 0x77; packet.service_id = 0x80000003u;
    RpcFoundBlock found = {1, 2, 0x2222};

    search_result = &found; send_result = 1;
    _request_bind(&packet, &services);
    if (services_seen != &services || services_seen_search != &services || searched_id != 0x80000003u) return 1;
    if (snapshot.word_7 != 0x77 || snapshot.word_5 != 0x55 || snapshot.command != 0x80000009u) return 2;
    if (snapshot.found != &found || snapshot.found_word_2 != 0x2222) return 3;
    if (sent_command != 0x80000008u || sent_packet != &reply_storage || sent_size != 64) return 4;
    if (extras[0] != 0 || extras[1] != 0 || extras[2] != 0 || alarms != 0) return 5;

    // Not found: both reply words zero even though the packet memory held junk.
    search_result = 0; send_result = 5;
    _request_bind(&packet, &services);
    if (snapshot.found != 0 || snapshot.found_word_2 != 0 || alarms != 0) return 6;

    // Send refused: retry armed with the same reply.
    send_result = 0;
    _request_bind(&packet, &services);
    if (alarms != 1 || alarm_time != 0x800 || alarm_argument != &reply_storage) return 7;
    if (alarm_handler != reinterpret_cast<void*>(_alarm_bind)) return 8;
    std::puts("_request_bind passed");
    return 0;
}

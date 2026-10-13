// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp   (the source is included)
#include "sif_m_request_end.cpp"

static int events[16];
static int event_count;
static unsigned gp_seen;
static void* argument_seen;
static unsigned* freed;
static int callback_runs;

static void note(int e) { events[event_count++] = e; }

extern "C" void norrath_request_end_call_with_gp(void (*callback)(void*), void* argument, unsigned gp) {
    gp_seen = gp;
    argument_seen = argument;
    callback(argument);
}
extern "C" int iSignalSema(int semaphore) { note(100 + semaphore); return 0; }
extern "C" void _sceRpcFreePacket(unsigned* packet) { note(200); freed = packet; }
static void callback(void*) { ++callback_runs; note(300); }

static unsigned held[16];

static SifMRpcClientBlock make_client(int first, int second) {
    SifMRpcClientBlock client = {};
    client.packet = held;
    client.semaphore = first;
    client.second_semaphore = second;
    client.word_5 = 0x55;
    client.word_9 = 0x99;
    client.word_11 = 0xbb;
    return client;
}
static void reset() { event_count = 0; callback_runs = 0; freed = 0; gp_seen = 0; argument_seen = 0; }

int main() {
    int token;
    SifMRpcEndPacket packet = {};
    packet.word_9 = 0x1234; packet.word_10 = 0x5678;

    // 0x8000001a with a callback: runs under the saved gp, then second semaphore, then first, then free.
    reset();
    SifMRpcClientBlock client = make_client(7, 9);
    client.callback = callback; client.callback_argument = &token; client.saved_gp = 0x4879f0;
    packet.client = &client; packet.command = 0x8000001au;
    norrath_sif_m_request_end(&packet, nullptr);
    if (callback_runs != 1 || gp_seen != 0x4879f0u || argument_seen != &token) return 1;
    if (event_count != 4 || events[0] != 300 || events[1] != 109 || events[2] != 107 || events[3] != 200) return 2;
    if (freed != held || client.packet != nullptr) return 3;
    if (client.word_9 != 0x99 || client.word_5 != 0x55 || client.word_11 != 0xbb) return 4;

    // 0x8000001a without a callback: no call, both semaphores still signalled; a negative first id is skipped.
    reset();
    client = make_client(-1, 4);
    packet.command = 0x8000001au;
    norrath_sif_m_request_end(&packet, nullptr);
    if (callback_runs != 0 || event_count != 2 || events[0] != 104 || events[1] != 200 || client.packet != nullptr) return 5;

    // 0x80000019: two words copied, then the second semaphore.
    reset();
    client = make_client(0, 5);
    packet.command = 0x80000019u;
    norrath_sif_m_request_end(&packet, nullptr);
    if (client.word_9 != 0x1234u || client.word_5 != 0x5678u || client.word_11 != 0xbbu) return 6;
    if (event_count != 3 || events[0] != 105 || events[1] != 100 || events[2] != 200) return 7;

    // 0x8000001d: word 11 takes packet word 9; the second semaphore is signalled.
    reset();
    client = make_client(2, 6);
    packet.command = 0x8000001du;
    norrath_sif_m_request_end(&packet, nullptr);
    if (client.word_11 != 0x1234u || client.word_9 != 0x99u || client.word_5 != 0x55u) return 8;
    if (event_count != 3 || events[0] != 106 || events[1] != 102 || events[2] != 200) return 9;

    // Anything else (including 0x8000001c and the neighbours) only signals the first semaphore and frees.
    const unsigned others[] = {0x8000001cu, 0x80000018u, 0x8000001bu, 0x8000001eu, 0u, 0xffffffffu, 0x8000000au};
    for (unsigned other : others) {
        reset();
        client = make_client(3, 8);
        client.callback = callback;
        packet.command = other;
        norrath_sif_m_request_end(&packet, nullptr);
        if (callback_runs != 0 || client.word_9 != 0x99 || client.word_5 != 0x55 || client.word_11 != 0xbb) return 10;
        if (event_count != 2 || events[0] != 103 || events[1] != 200 || client.packet != nullptr) return 11;
    }
    return 0;
}

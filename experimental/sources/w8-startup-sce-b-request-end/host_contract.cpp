#include "request_end.cpp"

#include <cstdio>

static int events[16];
static int event_count;
static unsigned gp_seen;
static void* argument_seen;
static int signalled;
static unsigned* freed;

static void note(int e) { events[event_count++] = e; }

extern "C" void norrath_request_end_call_with_gp(void (*callback)(void*), void* argument, unsigned gp) {
    gp_seen = gp;
    argument_seen = argument;
    callback(argument);
}
extern "C" int iSignalSema(int semaphore) { note(100 + semaphore); signalled = semaphore; return 0; }
extern "C" void _sceRpcFreePacket(unsigned* packet) { note(200); freed = packet; }

static int callback_runs;
static void callback(void*) { ++callback_runs; note(300); }

static unsigned held[16];

static RpcClientBlock make_client(int semaphore) {
    RpcClientBlock client = {};
    client.packet = held;
    client.semaphore = semaphore;
    client.word_5 = 0x55;
    client.word_9 = 0x99;
    return client;
}

static void reset() { event_count = 0; callback_runs = 0; signalled = -99; freed = 0; gp_seen = 0; argument_seen = 0; }

int main() {
    int token;
    RpcEndPacket packet = {};

    // End-of-call with a callback: runs under the client's gp, then signals and frees.
    reset();
    RpcClientBlock client = make_client(7);
    client.callback = callback;
    client.callback_argument = &token;
    client.saved_gp = 0x4879f0;
    packet.client = &client;
    packet.command = 0x8000000au;
    packet.word_9 = 1; packet.word_10 = 2;
    _request_end(&packet);
    if (callback_runs != 1 || gp_seen != 0x4879f0u || argument_seen != &token) return 1;
    if (event_count != 3 || events[0] != 300 || events[1] != 107 || events[2] != 200) return 2;
    if (freed != held || client.packet != 0) return 3;
    if (client.word_9 != 0x99 || client.word_5 != 0x55) return 4;

    // End-of-call without a callback and a negative semaphore: no signal.
    reset();
    client = make_client(-1);
    packet.command = 0x8000000au;
    _request_end(&packet);
    if (callback_runs != 0 || event_count != 1 || events[0] != 200 || client.packet != 0) return 5;

    // Bind reply copies two words and signals.
    reset();
    client = make_client(0);
    packet.command = 0x80000009u;
    packet.word_9 = 0x1234; packet.word_10 = 0x5678;
    _request_end(&packet);
    if (client.word_9 != 0x1234u || client.word_5 != 0x5678u || signalled != 0) return 6;
    if (event_count != 2 || events[0] != 100 || events[1] != 200) return 7;

    // Any other command only signals and frees; a callback present is not run.
    const unsigned others[] = {0x80000008u, 0x8000000bu, 0, 0xffffffffu};
    for (unsigned other : others) {
        reset();
        client = make_client(3);
        client.callback = callback;
        packet.command = other;
        _request_end(&packet);
        if (callback_runs != 0 || client.word_9 != 0x99 || client.word_5 != 0x55) return 8;
        if (event_count != 2 || events[0] != 103 || events[1] != 200 || client.packet != 0) return 9;
    }
    std::puts("_request_end passed");
    return 0;
}

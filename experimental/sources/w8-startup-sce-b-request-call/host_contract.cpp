#include "request_call.cpp"

#include <cstdio>

static int woken[4];
static int wake_count;

extern "C" int iWakeupThread(int thread) { woken[wake_count++] = thread; return 0; }

static RpcCallPacket make_packet(RpcServerBlock* server, unsigned base) {
    RpcCallPacket p = {};
    p.word_4 = base + 4; p.word_5 = base + 5; p.word_6 = base + 6; p.word_7 = base + 7;
    p.word_8 = base + 8; p.word_9 = base + 9; p.word_10 = base + 10; p.word_11 = base + 11;
    p.word_12 = base + 12;
    p.server = server;
    return p;
}

static bool copied(const RpcServerBlock& s, unsigned base) {
    return s.word_8 == base + 5 && s.word_7 == base + 7 && s.word_9 == base + 8 && s.word_3 == base + 9 &&
           s.word_10 == base + 10 && s.word_11 == base + 11 && s.word_12 == base + 12 && s.word_13 == base + 4;
}

int main() {
    RpcWorkQueue queue = {};
    RpcServerBlock first = {};
    RpcServerBlock second = {};
    first.queue = &queue;
    second.queue = &queue;
    first.word_6 = 0x66; first.word_14 = 0x77; first.word_0 = 0x11;

    // Empty queue, no thread to wake (thread id negative).
    queue.thread = -1;
    RpcCallPacket p1 = make_packet(&first, 100);
    _request_call(&p1);
    if (queue.head != &first || queue.tail != &first || wake_count != 0) return 1;
    if (!copied(first, 100) || first.word_6 != 0x66 || first.word_14 != 0x77 || first.word_0 != 0x11) return 2;

    // Second request is linked behind the first; running is zero so the thread wakes.
    queue.thread = 9;
    RpcCallPacket p2 = make_packet(&second, 200);
    _request_call(&p2);
    if (queue.head != &first || queue.tail != &second || first.next != &second) return 3;
    if (!copied(second, 200) || second.next != 0) return 4;
    if (wake_count != 1 || woken[0] != 9) return 5;

    // Running queue is not woken; thread 0 counts as a valid id.
    queue.running = 1;
    RpcCallPacket p3 = make_packet(&first, 300);
    _request_call(&p3);
    if (wake_count != 1 || queue.tail != &first) return 6;
    queue.running = 0;
    queue.thread = 0;
    _request_call(&p3);
    if (wake_count != 2 || woken[1] != 0) return 7;
    std::puts("_request_call passed");
    return 0;
}

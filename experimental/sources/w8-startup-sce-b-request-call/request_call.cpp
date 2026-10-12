// Observed _request_call at 0x002d81e0, 140 bytes, a local handler in the SIF RPC
// library. The incoming packet's word 13 (offset 52) points at a server block,
// and that block's word 16 (offset 64) points at its work queue. The block is
// appended to the queue (queue head at offset 12, tail at offset 16, and the
// previous tail's link at offset 60 of the block, or the head when the queue
// was empty) and takes these packet words:
//   block +32 <- packet +20      block +28 <- packet +28      block +36 <- packet +32
//   block +12 <- packet +36      block +40 <- packet +40      block +44 <- packet +44
//   block +48 <- packet +48      block +52 <- packet +16
// When the queue's word 0 is not negative and its word 1 is zero, the thread
// named by word 0 is woken with iWakeupThread (a tail call, so its result is the
// handler's result; nothing reads it). The queue word meanings are not named
// beyond this use.
extern "C" {
struct RpcServerBlock;

struct RpcWorkQueue {
    int thread;
    unsigned running;
    unsigned word_2;
    RpcServerBlock* head;
    RpcServerBlock* tail;
};

struct RpcServerBlock {
    unsigned word_0;
    unsigned word_1;
    unsigned word_2;
    unsigned word_3;
    unsigned word_4;
    unsigned word_5;
    unsigned word_6;
    unsigned word_7;
    unsigned word_8;
    unsigned word_9;
    unsigned word_10;
    unsigned word_11;
    unsigned word_12;
    unsigned word_13;
    unsigned word_14;
    RpcServerBlock* next;
    RpcWorkQueue* queue;
};

struct RpcCallPacket {
    unsigned words_0_to_3[4];
    unsigned word_4;
    unsigned word_5;
    unsigned word_6;
    unsigned word_7;
    unsigned word_8;
    unsigned word_9;
    unsigned word_10;
    unsigned word_11;
    unsigned word_12;
    RpcServerBlock* server;
};

#if defined(__mips__)
static_assert(__builtin_offsetof(RpcWorkQueue, head) == 12, "layout");
static_assert(__builtin_offsetof(RpcWorkQueue, tail) == 16, "layout");
static_assert(__builtin_offsetof(RpcServerBlock, next) == 60, "layout");
static_assert(__builtin_offsetof(RpcServerBlock, queue) == 64, "layout");
static_assert(__builtin_offsetof(RpcCallPacket, word_12) == 48, "layout");
static_assert(__builtin_offsetof(RpcCallPacket, server) == 52, "layout");
#endif

int iWakeupThread(int thread);

void _request_call(RpcCallPacket* packet) {
    RpcServerBlock* server = packet->server;
    RpcWorkQueue* queue = server->queue;
    if (queue->head == 0)
        queue->head = server;
    else
        queue->tail->next = server;
    queue->tail = server;
    server->word_8 = packet->word_5;
    server->word_7 = packet->word_7;
    server->word_9 = packet->word_8;
    server->word_3 = packet->word_9;
    server->word_10 = packet->word_10;
    server->word_11 = packet->word_11;
    server->word_12 = packet->word_12;
    server->word_13 = packet->word_4;
    if (queue->thread >= 0 && queue->running == 0) iWakeupThread(queue->thread);
}
}

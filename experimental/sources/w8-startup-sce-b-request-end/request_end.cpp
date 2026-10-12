// Observed _request_end at 0x002d7c08, 204 bytes, a local handler in the SIF RPC
// library (a second, longer copy of the same name at 0x002e47c8 belongs to the
// CD library and is not this function). It receives an incoming command packet
// whose word 7 (offset 28) points at a client block and whose word 8 (offset 32)
// is the command id.
//
//   0x8000000a  When the client's callback (offset 28) is nonzero it runs with
//               the client's argument (offset 32) while the global pointer holds
//               the client's saved gp (offset 24); the old gp is restored after.
//               The semaphore path below follows either way.
//   0x80000009  The client's word 9 (offset 36) takes packet word 9 and the
//               client's word 5 (offset 20) takes packet word 10.
//   any other   Nothing is copied.
//
// Every command then signals the client's semaphore (offset 8) with iSignalSema
// when that id is not negative, frees the packet the client holds (offset 0) with
// _sceRpcFreePacket and clears that client pointer. The second argument the
// handler is registered with is not read.
extern "C" {
struct RpcClientBlock {
    unsigned* packet;
    unsigned word_1;
    int semaphore;
    unsigned word_3;
    unsigned word_4;
    unsigned word_5;
    unsigned saved_gp;
    void (*callback)(void*);
    void* callback_argument;
    unsigned word_9;
};

struct RpcEndPacket {
    unsigned words_0_to_6[7];
    RpcClientBlock* client;
    unsigned command;
    unsigned word_9;
    unsigned word_10;
};

#if defined(__mips__)
static_assert(__builtin_offsetof(RpcClientBlock, packet) == 0, "layout");
static_assert(__builtin_offsetof(RpcClientBlock, semaphore) == 8, "layout");
static_assert(__builtin_offsetof(RpcClientBlock, word_5) == 20, "layout");
static_assert(__builtin_offsetof(RpcClientBlock, saved_gp) == 24, "layout");
static_assert(__builtin_offsetof(RpcClientBlock, callback) == 28, "layout");
static_assert(__builtin_offsetof(RpcClientBlock, callback_argument) == 32, "layout");
static_assert(__builtin_offsetof(RpcClientBlock, word_9) == 36, "layout");
static_assert(__builtin_offsetof(RpcEndPacket, client) == 28, "layout");
static_assert(__builtin_offsetof(RpcEndPacket, command) == 32, "layout");
static_assert(__builtin_offsetof(RpcEndPacket, word_9) == 36, "layout");
static_assert(__builtin_offsetof(RpcEndPacket, word_10) == 40, "layout");
#endif

int iSignalSema(int semaphore);
void _sceRpcFreePacket(unsigned* packet);

#if !defined(__mips__)
void norrath_request_end_call_with_gp(void (*callback)(void*), void* argument, unsigned gp);
#endif

static void call_with_gp(void (*callback)(void*), void* argument, unsigned gp) {
#if defined(__mips__)
    unsigned long saved;
    __asm__ volatile("move %0, $gp\n\tmove $gp, %1" : "=&r"(saved) : "r"(gp) : "memory");
    callback(argument);
    __asm__ volatile("move $gp, %0" : : "r"(saved) : "memory");
#else
    norrath_request_end_call_with_gp(callback, argument, gp);
#endif
}

void _request_end(RpcEndPacket* packet) {
    RpcClientBlock* client = packet->client;
    const unsigned command = packet->command;
    if (command == 0x8000000au) {
        if (client->callback != 0) call_with_gp(client->callback, client->callback_argument, client->saved_gp);
    } else if (command == 0x80000009u) {
        client->word_9 = packet->word_9;
        client->word_5 = packet->word_10;
    }
    client = packet->client;
    if (client->semaphore >= 0) iSignalSema(client->semaphore);
    _sceRpcFreePacket(client->packet);
    client->packet = 0;
}
}

/* Observed local routine _request_end at 0x002e47c8, 288 bytes. The symbol
 * table has two routines of that name; this is the longer one, registered by
 * sceSifMInitRpc as the command handler for 0x80000018 (the shorter copy at
 * 0x002d7c08 is the one in w8-startup-sce-b-request-end). It is imported by
 * that registration as norrath_sif_m_request_end. It receives an incoming
 * command packet whose word 7 (offset 28) points at a client block and whose
 * word 8 (offset 32) is the command. The second argument is not read.
 *
 *   0x8000001a  When the client's callback (offset 28) is nonzero it runs with
 *               the client's argument (offset 32) while the global pointer holds
 *               the client's saved gp (offset 24), then the old gp is restored.
 *               Either way the client's second semaphore (offset 40) is signalled
 *               with iSignalSema.
 *   0x80000019  The client's word 9 (offset 36) takes packet word 9 (offset 36)
 *               and the client's word 5 (offset 20) takes packet word 10
 *               (offset 40); then the second semaphore is signalled.
 *   0x8000001d  The client's word 11 (offset 44) takes packet word 9; then the
 *               second semaphore is signalled.
 *   any other   Nothing is copied or signalled here.
 *
 * Every command then, with the client pointer read from the packet again,
 * signals the client's first semaphore (offset 8) when that id is not
 * negative, frees the packet the client holds (offset 0) with
 * _sceRpcFreePacket and clears that client pointer.
 * Inferred: 0x80000019, 0x8000001a and 0x8000001d are replies of the
 * multi-client RPC (bind, call and rdata or similar); the names are not
 * established here.
 */
extern "C" {
struct SifMRpcClientBlock {
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
    int second_semaphore;
    unsigned word_11;
};

struct SifMRpcEndPacket {
    unsigned words_0_to_6[7];
    SifMRpcClientBlock* client;
    unsigned command;
    unsigned word_9;
    unsigned word_10;
};

#if defined(__mips__)
static_assert(__builtin_offsetof(SifMRpcClientBlock, semaphore) == 8, "layout");
static_assert(__builtin_offsetof(SifMRpcClientBlock, word_5) == 20, "layout");
static_assert(__builtin_offsetof(SifMRpcClientBlock, saved_gp) == 24, "layout");
static_assert(__builtin_offsetof(SifMRpcClientBlock, callback) == 28, "layout");
static_assert(__builtin_offsetof(SifMRpcClientBlock, callback_argument) == 32, "layout");
static_assert(__builtin_offsetof(SifMRpcClientBlock, word_9) == 36, "layout");
static_assert(__builtin_offsetof(SifMRpcClientBlock, second_semaphore) == 40, "layout");
static_assert(__builtin_offsetof(SifMRpcClientBlock, word_11) == 44, "layout");
static_assert(__builtin_offsetof(SifMRpcEndPacket, client) == 28, "layout");
static_assert(__builtin_offsetof(SifMRpcEndPacket, command) == 32, "layout");
static_assert(__builtin_offsetof(SifMRpcEndPacket, word_9) == 36, "layout");
static_assert(__builtin_offsetof(SifMRpcEndPacket, word_10) == 40, "layout");
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

void norrath_sif_m_request_end(SifMRpcEndPacket* packet, void*) {
    SifMRpcClientBlock* client = packet->client;
    switch (packet->command) {
    case 0x8000001au:
        if (client->callback != 0) call_with_gp(client->callback, client->callback_argument, client->saved_gp);
        iSignalSema(client->second_semaphore);
        break;
    case 0x80000019u:
        client->word_9 = packet->word_9;
        client->word_5 = packet->word_10;
        iSignalSema(client->second_semaphore);
        break;
    case 0x8000001du:
        client->word_11 = packet->word_9;
        iSignalSema(client->second_semaphore);
        break;
    default:
        break;
    }
    client = packet->client;
    if (client->semaphore >= 0) iSignalSema(client->semaphore);
    _sceRpcFreePacket(client->packet);
    client->packet = 0;
}
}

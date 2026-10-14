#include "sif_alloc_iop_heap.cpp"

#include <cstdio>

int _bind;
unsigned char norrath_iop_heap_client[64];
int norrath_sound_iop_heap_receive_view[16];
int norrath_sound_iop_heap_send_view[16];
static int calls, result;
static void *g_client, *g_send, *g_recv, *g_done, *g_arg;
static unsigned g_cmd, g_mode;
static int g_ssize, g_rsize, sent_word;

extern "C" int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size,
                             void* receive, int receive_size, void* done, void* done_arg) {
    g_client = client; g_cmd = command; g_mode = mode; g_send = send; g_ssize = send_size;
    g_recv = receive; g_rsize = receive_size; g_done = done; g_arg = done_arg;
    sent_word = norrath_sound_iop_heap_send_view[0];
    calls += 1;
    return result;
}

int main() {
    _bind = -1;
    norrath_sound_iop_heap_receive_view[0] = 0x1234;
    if (sceSifAllocIopHeap(64) != nullptr || calls != 0) return 1;
    if (norrath_sound_iop_heap_send_view[0] != 0) return 2; /* nothing stored while unbound */
    _bind = 0;
    if (reinterpret_cast<unsigned long>(sceSifAllocIopHeap(64)) != 0x1234 || calls != 1) return 3;
    if (g_client != norrath_iop_heap_client || g_cmd != 1 || g_mode != 0) return 4;
    if (g_send != norrath_sound_iop_heap_send_view || g_recv != norrath_sound_iop_heap_receive_view) return 5;
    if (g_ssize != 4 || g_rsize != 4 || g_done || g_arg || sent_word != 64) return 6;
    result = -1;
    if (sceSifAllocIopHeap(8) != nullptr || norrath_sound_iop_heap_send_view[0] != 8) return 7;
    _bind = 5;
    result = 1;
    if (calls != 2 || reinterpret_cast<unsigned long>(sceSifAllocIopHeap(16)) != 0x1234 || calls != 3) return 8;
    std::puts("sif alloc iop heap contract passed");
    return 0;
}

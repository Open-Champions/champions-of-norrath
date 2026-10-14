// Host check only; not an EE build input. Pointers are 8 bytes on the host, so
// the address argument is checked through a small integer cast on the target
// only; here the stored word is compared to the low 32 bits.
#include <cstdint>
extern "C" {
int _bind;
unsigned char norrath_sound_iop_heap_client_view[64];
int norrath_sound_iop_heap_receive_view[16];
int norrath_sound_iop_heap_send_view[16];
int rpc_calls, rpc_result;
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* done, void* done_arg) {
    ++rpc_calls;
    if (client != norrath_sound_iop_heap_client_view || command != 2 || mode != 0 ||
        send != norrath_sound_iop_heap_send_view || send_size != 4 ||
        receive != norrath_sound_iop_heap_receive_view || receive_size != 4 || done || done_arg)
        return -1000;
    norrath_sound_iop_heap_receive_view[0] = 0x1234;
    return rpc_result;
}
int sceSifFreeSysMemory(void* address);
}
int main() {
    _bind = -1;
    if (sceSifFreeSysMemory(reinterpret_cast<void*>(8)) != 0 || rpc_calls != 0) return 1;
    _bind = 0;
    rpc_result = 0;
    if (sceSifFreeSysMemory(reinterpret_cast<void*>(0x40)) != 0x1234 || rpc_calls != 1) return 2;
    if (norrath_sound_iop_heap_send_view[0] != 0x40) return 3;
    rpc_result = -3;
    if (sceSifFreeSysMemory(0) != -1) return 4;
    return 0;
}

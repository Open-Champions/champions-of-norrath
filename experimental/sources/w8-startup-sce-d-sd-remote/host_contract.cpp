#define NORRATH_HOST_CONTRACT 1
#include "sd_remote.cpp"

#include <cstdio>
#include <cstring>
#include <vector>

alignas(64) unsigned norrath_sound_remote_packet_view[16];
alignas(64) unsigned char norrath_sound_remote_rpc_client_view[64];
unsigned norrath_sound_remote_end_function_word_view[1];
unsigned norrath_sound_remote_end_gp_word_view[1];
unsigned norrath_sound_remote_interrupt_functions_view[3];
unsigned norrath_sound_remote_interrupt_arguments_view[3];
unsigned norrath_sound_remote_interrupt_gps_view[3];
extern "C" unsigned norrath_host_gp = 0x7000;

static std::vector<void*> registry;
extern "C" unsigned norrath_host_word(const void* p) {
    for (size_t i = 0; i < registry.size(); ++i) if (registry[i] == p) return 0x1000u + static_cast<unsigned>(i) * 0x10u;
    registry.push_back(const_cast<void*>(p));
    return 0x1000u + static_cast<unsigned>(registry.size() - 1) * 0x10u;
}
extern "C" void* norrath_host_pointer(unsigned w) {
    if (w == 0) return nullptr;
    if (w < 0x1000u || (w - 0x1000u) / 0x10u >= registry.size()) return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(w)); /* opaque code address */
    return registry[(w - 0x1000u) / 0x10u];
}

struct Call {
    void* client; unsigned function, mode; void* send; int send_size; void* receive; int receive_size; void* end_function; void* end_parameter;
    unsigned gp; unsigned packet[16];
};
static Call calls[4];
static int ncalls;
static unsigned fill_receive_word;   /* written to the first receive word by the fake IOP */
extern "C" int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                             void* receive, int receive_size, void* end_function, void* end_parameter) {
    Call& c = calls[ncalls++];
    c = Call{client, function, mode, send, send_size, receive, receive_size, end_function, end_parameter, norrath_host_gp, {}};
    std::memcpy(c.packet, norrath_sound_remote_packet_view, sizeof c.packet);
    *static_cast<unsigned*>(receive) = fill_receive_word;
    return 0;
}

static void reset() {
    ncalls = 0;
    std::memset(norrath_sound_remote_packet_view, 0xee, sizeof norrath_sound_remote_packet_view);
    norrath_host_gp = 0x7000;
    norrath_sound_remote_end_function_word_view[0] = 0x00abc000u;
    norrath_sound_remote_end_gp_word_view[0] = 0x5500;
    fill_receive_word = 0x1111;
}

int main() {
    unsigned* P = norrath_sound_remote_packet_view;
    /* default command, synchronous: no end function, no gp swap, result packet word 0 */
    reset();
    int r = sceSdRemote(1, 0x1234, 11, 12, 13, 14, 15, 16);
    if (ncalls != 1 || r != 0x1111) return 1;
    Call& c = calls[0];
    if (c.client != norrath_sound_remote_rpc_client_view || c.function != 0x1234 || c.mode != 0) return 2;
    if (c.send != P || c.send_size != 64 || c.receive != P || c.receive_size != 16 || c.end_function || c.end_parameter != P) return 3;
    if (c.packet[0] != norrath_host_word(P) || c.packet[1] != 11 || c.packet[6] != 16 || c.packet[7] != 0xeeeeeeeeu) return 4;
    if (c.gp != 0x7000 || norrath_host_gp != 0x7000) return 5;
    /* asynchronous (flag 0): mode 1, end function, gp swapped during the call only */
    reset();
    r = sceSdRemote(0, 0x1234, 1, 2, 3, 4, 5, 6);
    if (calls[0].mode != 1 || calls[0].end_function != norrath_host_pointer(0x00abc000u) || calls[0].gp != 0x5500 || norrath_host_gp != 0x7000) return 6;
    /* zero end function word with flag 0: mode 1 but no gp swap */
    reset(); norrath_sound_remote_end_function_word_view[0] = 0;
    sceSdRemote(0, 0x1234, 1, 2, 3, 4, 5, 6);
    if (calls[0].mode != 1 || calls[0].end_function || calls[0].gp != 0x7000) return 7;
    /* 0x8130 group */
    for (unsigned cmd : {0x8130u, 0x81a0u, 0x81b0u}) {
        reset();
        unsigned data[4];
        r = sceSdRemote(1, cmd, 3, norrath_host_word(data), 0, 0, 0, 0);
        Call& d = calls[0];
        if (d.function != (cmd | 3u) || d.send != data || d.send_size != 64 || d.receive != P || d.receive_size != 64 || d.end_parameter != P) return 8;
        if (r != 0x1111) return 9;
    }
    /* 0x8140: result is zero; the area is send, receive and end parameter */
    reset();
    unsigned area[4];
    r = sceSdRemote(0, 0x8140, 0x10, norrath_host_word(area), 0, 0, 0, 0);
    if (r != 0 || calls[0].function != (0x10u | 0x8140u) || calls[0].send != area || calls[0].receive != area || calls[0].end_parameter != area) return 10;
    if (calls[0].send_size != 64 || calls[0].receive_size != 64 || calls[0].gp != 0x5500) return 11;
    /* 0x9000 range */
    for (unsigned cmd : {0x9000u, 0x9050u, 0x90f0u}) {
        reset();
        unsigned data[4];
        r = sceSdRemote(1, cmd, norrath_host_word(data), 24, 0, 0, 0, 0);
        if (calls[0].send != data || calls[0].send_size != 24 || calls[0].receive != P || calls[0].receive_size != 16 || calls[0].function != cmd || r != 0x1111) return 12;
    }
    for (unsigned cmd : {0x8fffu, 0x90f1u}) { /* just outside it: default */
        reset();
        sceSdRemote(1, cmd, 1, 2, 0, 0, 0, 0);
        if (calls[0].send != P || calls[0].send_size != 64) return 13;
    }
    /* 0x81c0 with a null receive pointer: the local result word is the receive area */
    reset();
    unsigned short target[8] = {};
    fill_receive_word = 0x2222;
    r = sceSdRemote(1, 0x81c0, norrath_host_word(target), 0xabcd1234u, 0, 0, 0, 0x9999);
    if (target[1] != 0x1234 || target[0] || target[2] || target[3]) return 14;
    if (calls[0].function != 0x81c0 || calls[0].send != target || calls[0].send_size != (0xabcd1234u + 1) * 8 % 0x100000000ull) return 15;
    if (calls[0].receive_size != 4 || r != 0x2222) return 16;
    /* 0x81d0 with a receive pointer */
    reset();
    unsigned out[2] = {0x3333, 0};
    unsigned wide[4] = {};
    r = sceSdRemote(0, 0x81d0, norrath_host_word(wide), 5, norrath_host_word(out), 8, 0x5a5a5a5au, 0);
    if (wide[1] != 0x5a5a5a5au || reinterpret_cast<unsigned short*>(wide)[1] != 5) return 17;
    if (calls[0].send_size != 48 || calls[0].receive != out || calls[0].receive_size != 8 || calls[0].end_parameter != P) return 18;
    if (r != 0x2222 && r != 0x1111) { /* the fake IOP wrote into out[0], which is what is returned */ }
    if (r != 0x1111) return 19;
    /* 0x8160: bookkeeping for index 0 and 1, then the generic call; old function is the result */
    reset();
    norrath_sound_remote_interrupt_functions_view[0] = 0xaaaa;
    norrath_sound_remote_interrupt_functions_view[1] = 0xbbbb;
    r = sceSdRemote(1, 0x8160, 0, 0x1357, 0x2468, 0, 0, 0);
    if (r != 0xaaaa || norrath_sound_remote_interrupt_functions_view[0] != 0x1357 || norrath_sound_remote_interrupt_arguments_view[0] != 0x2468 || norrath_sound_remote_interrupt_gps_view[0] != 0x7000) return 20;
    if (norrath_sound_remote_interrupt_functions_view[1] != 0xbbbb) return 21;
    if (calls[0].function != 0x8160 || calls[0].send != P || calls[0].send_size != 64 || calls[0].receive_size != 16) return 22;
    reset(); norrath_host_gp = 0x7100;
    r = sceSdRemote(1, 0x8160, 1, 0x1111, 0x2222, 0, 0, 0);
    if (r != 0xbbbb || norrath_sound_remote_interrupt_functions_view[1] != 0x1111 || norrath_sound_remote_interrupt_arguments_view[1] != 0x2222 || norrath_sound_remote_interrupt_gps_view[1] != 0x7100) return 23;
    /* 0x8170 */
    reset();
    norrath_sound_remote_interrupt_functions_view[2] = 0xcccc;
    r = sceSdRemote(1, 0x8170, 0x4444, 0x5555, 0, 0, 0, 0);
    if (r != 0xcccc || norrath_sound_remote_interrupt_functions_view[2] != 0x4444 || norrath_sound_remote_interrupt_arguments_view[2] != 0x5555 || norrath_sound_remote_interrupt_gps_view[2] != 0x7000) return 24;
    if (calls[0].function != 0x8170) return 25;
    std::puts("sd remote contract passed");
    return 0;
}

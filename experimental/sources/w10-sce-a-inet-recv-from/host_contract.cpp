// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp inet_recv_from.cpp
static int order[8], order_n, ms_results[2], ms_n, call_result, reply[8];
static int ms_off[2], ms_size[2], call_fn, call_send, call_recv, words_at_call[4];
static void *ms_dst[2], *ms_src[2], *call_buf;
extern "C" {
int CallRpc(void* object, int function, void* buffer, int s, int r) {
    (void)object; order[order_n++] = 2; call_buf = buffer; call_fn = function; call_send = s; call_recv = r;
    for (int i = 0; i < 4; ++i) words_at_call[i] = static_cast<int*>(buffer)[i];
    if (call_result >= 0) for (int i = 0; i < 8; ++i) static_cast<int*>(buffer)[i] = reply[i];
    return call_result;
}
int MemcpyS(void* object, void* destination, void* source, int word_offset, int size) {
    (void)object; order[order_n++] = 3;
    ms_dst[ms_n & 1] = destination; ms_src[ms_n & 1] = source; ms_off[ms_n & 1] = word_offset; ms_size[ms_n & 1] = size;
    return ms_results[ms_n++ & 1];
}
int sceInetRecvFrom(void* object, int* buffer, int handle, void* data, int size, int* io_word, void* address,
                    int* output_word, int ninth);
}
static void reset() { order_n = 0; ms_n = 0; call_result = 0; ms_results[0] = ms_results[1] = 0; reply[0] = 24; reply[1] = 321; reply[6] = 654; }
int main() {
    int object, buffer[32] = {}, io = 123, out = 5; char data[64], address[16];
    reset();
    int r = sceInetRecvFrom(&object, buffer, 9, data, 40, &io, address, &out, 0x99);
    if (r != 24 || io != 321 || out != 654) return 1;
    if (order_n != 3 || order[0] != 2 || order[1] != 3 || order[2] != 3) return 2;
    if (words_at_call[0] != 9 || words_at_call[1] != 123 || words_at_call[2] != 40 || words_at_call[3] != 0x99) return 3;
    if (call_fn != 13 || call_buf != buffer || call_send != 16 || call_recv != 68) return 4;
    if (ms_dst[0] != address || ms_src[0] != buffer || ms_off[0] != 2 || ms_size[0] != 16) return 5;
    if (ms_dst[1] != data || ms_src[1] != buffer || ms_off[1] != 7 || ms_size[1] != 24) return 6;
    // null optional words are skipped and word 1 is sent as zero
    reset(); io = 7; out = 8;
    r = sceInetRecvFrom(&object, buffer, 9, data, 40, nullptr, address, nullptr, 0);
    if (r != 24 || io != 7 || out != 8 || words_at_call[1] != 0) return 7;
    // call failure and negative reply
    reset(); call_result = -2;
    if (sceInetRecvFrom(&object, buffer, 9, data, 40, &io, address, &out, 0) != -542 || order_n != 1) return 8;
    reset(); reply[0] = -9; io = 7; out = 8;
    if (sceInetRecvFrom(&object, buffer, 9, data, 40, &io, address, &out, 0) != -9 || order_n != 1 || io != 7 || out != 8) return 9;
    // copy failures: the first one stops before the output word, the second one is returned
    reset(); ms_results[0] = -543; out = 8;
    if (sceInetRecvFrom(&object, buffer, 9, data, 40, &io, address, &out, 0) != -543 || order_n != 2 || out != 8) return 10;
    reset(); ms_results[1] = -4;
    if (sceInetRecvFrom(&object, buffer, 9, data, 40, &io, address, &out, 0) != -4 || order_n != 3) return 11;
    return 0;
}

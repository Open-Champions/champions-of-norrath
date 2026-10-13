// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp inet_send_to.cpp
static int order[8], order_n, md_results[2], md_n, call_result, reply0, reply1;
static int md_off[2], md_size[2], call_fn, call_send, call_recv, words_at_first[5];
static const void* md_src[2]; static void* call_buf;
extern "C" {
int CallRpc(void* object, int function, void* buffer, int s, int r) {
    (void)object; order[order_n++] = 2; call_buf = buffer; call_fn = function; call_send = s; call_recv = r;
    static_cast<int*>(buffer)[0] = reply0; static_cast<int*>(buffer)[1] = reply1;
    return call_result;
}
int MemcpyD(void* object, void* buffer, int word_offset, const void* source, int size) {
    (void)object; order[order_n++] = 1;
    if (md_n == 0) for (int i = 0; i < 5; ++i) words_at_first[i] = static_cast<int*>(buffer)[i];
    md_off[md_n & 1] = word_offset; md_src[md_n & 1] = source; md_size[md_n & 1] = size;
    return md_results[md_n++ & 1];
}
int sceInetSendTo(void* object, int* buffer, int handle, const void* data, int size, int* io_word, const void* address,
                  int eighth, int ninth);
}
int main() {
    int object, buffer[32] = {}, io = 123; char data[64], address[16];
    reply0 = 40; reply1 = 456;
    int r = sceInetSendTo(&object, buffer, 9, data, 20, &io, address, 0x88, 0x99);
    if (r != 40 || io != 456) return 1;
    if (order_n != 3 || order[0] != 1 || order[1] != 1 || order[2] != 2) return 2;
    if (words_at_first[0] != 9 || words_at_first[1] != 123 || words_at_first[2] != 20 || words_at_first[3] != 0x99 || words_at_first[4] != 0x88) return 3;
    if (md_off[0] != 5 || md_src[0] != address || md_size[0] != 16 || md_off[1] != 9 || md_src[1] != data || md_size[1] != 20) return 4;
    if (call_fn != 14 || call_buf != buffer || call_send != 56 || call_recv != 8) return 5;
    // null in/out word: word 1 is zero and nothing is written back
    order_n = 0; md_n = 0; io = 777; buffer[1] = 0x6666;
    r = sceInetSendTo(&object, buffer, 9, data, 4, nullptr, address, 1, 2);
    if (r != 40 || io != 777 || words_at_first[1] != 0) return 6;
    // failures: first copy, second copy, call
    order_n = 0; md_n = 0; md_results[0] = -543;
    if (sceInetSendTo(&object, buffer, 9, data, 4, &io, address, 1, 2) != -543 || order_n != 1) return 7;
    order_n = 0; md_n = 0; md_results[0] = 0; md_results[1] = -5;
    if (sceInetSendTo(&object, buffer, 9, data, 4, &io, address, 1, 2) != -5 || order_n != 2) return 8;
    order_n = 0; md_n = 0; md_results[1] = 0; call_result = -11; io = 555;
    if (sceInetSendTo(&object, buffer, 9, data, 4, &io, address, 1, 2) != -542 || order_n != 3 || io != 555) return 9;
    return 0;
}

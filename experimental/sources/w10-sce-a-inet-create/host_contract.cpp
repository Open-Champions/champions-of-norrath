// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp inet_create.cpp
static int memd_calls, call_calls, call_result, memd_result, call_fn, call_send, call_recv, memd_off, memd_size, reply, order[4], order_n;
static void *memd_obj, *memd_buf, *call_obj, *call_buf; static const void* memd_src;
extern "C" {
int CallRpc(void* object, int function, void* buffer, int send_size, int receive_size) {
    ++call_calls; order[order_n++] = 2; call_obj = object; call_fn = function; call_buf = buffer; call_send = send_size; call_recv = receive_size;
    static_cast<int*>(buffer)[0] = reply;
    return call_result;
}
int MemcpyD(void* object, void* buffer, int word_offset, const void* source, int size) {
    ++memd_calls; order[order_n++] = 1; memd_obj = object; memd_buf = buffer; memd_off = word_offset; memd_src = source; memd_size = size;
    return memd_result;
}
int sceInetCreate(void* object, int* buffer, const void* source);
}
int main() {
    int object, buffer[32], source[16];
    reply = 77;
    if (sceInetCreate(&object, buffer, source) != 77 || memd_calls != 1 || call_calls != 1 || order[0] != 1 || order[1] != 2) return 1;
    if (memd_obj != &object || memd_buf != buffer || memd_off != 1 || memd_src != source || memd_size != 64) return 2;
    if (call_obj != &object || call_buf != buffer || call_fn != 1 || call_send != 68 || call_recv != 4) return 3;
    reply = 0;                                              // a zero reply becomes -540
    if (sceInetCreate(&object, buffer, source) != -540) return 4;
    reply = -9;                                             // any other value, negative too, passes through
    if (sceInetCreate(&object, buffer, source) != -9) return 5;
    call_result = -1; reply = 5; call_calls = 0;
    if (sceInetCreate(&object, buffer, source) != -542) return 6;
    call_result = 0; memd_result = -543; call_calls = 0;     // a failed copy stops before the call and is returned
    if (sceInetCreate(&object, buffer, source) != -543 || call_calls != 0) return 7;
    memd_result = -1;
    if (sceInetCreate(&object, buffer, source) != -1) return 8;
    return 0;
}

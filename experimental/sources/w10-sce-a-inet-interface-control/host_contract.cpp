// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp inet_interface_control.cpp
static int order[8], order_n, md_result, ms_result, call_result, reply;
static int md_off, md_size, ms_off, ms_size, call_fn, call_send, call_recv, words_at_md[3];
static void *md_buf, *ms_dst, *ms_src, *call_buf, *obj_seen[3]; static const void* md_src;
extern "C" {
int CallRpc(void* object, int function, void* buffer, int s, int r) {
    order[order_n++] = 2; obj_seen[1] = object; call_buf = buffer; call_fn = function; call_send = s; call_recv = r;
    static_cast<int*>(buffer)[0] = reply;
    return call_result;
}
int MemcpyD(void* object, void* buffer, int word_offset, const void* source, int size) {
    order[order_n++] = 1; obj_seen[0] = object; md_buf = buffer; md_off = word_offset; md_src = source; md_size = size;
    for (int i = 0; i < 3; ++i) words_at_md[i] = static_cast<int*>(buffer)[i];
    return md_result;
}
int MemcpyS(void* object, void* destination, void* source, int word_offset, int size) {
    order[order_n++] = 3; obj_seen[2] = object; ms_dst = destination; ms_src = source; ms_off = word_offset; ms_size = size;
    return ms_result;
}
int sceInetInterfaceControl(void* object, int* buffer, int first, int second, void* data, int size);
}
int main() {
    int object, buffer[32] = {}; char data[64];
    reply = 12;
    if (sceInetInterfaceControl(&object, buffer, 5, 6, data, 20) != 12 || order_n != 3 || order[0] != 1 || order[1] != 2 || order[2] != 3) return 1;
    if (words_at_md[0] != 5 || words_at_md[1] != 6 || words_at_md[2] != 20) return 2;      // header stored before the copy
    if (md_buf != buffer || md_off != 3 || md_src != data || md_size != 20) return 3;
    if (call_fn != 9 || call_buf != buffer || call_send != 32 || call_recv != 32) return 4;
    if (ms_dst != data || ms_src != buffer || ms_off != 3 || ms_size != 20) return 5;
    for (void* o : obj_seen) if (o != &object) return 6;
    // each failure stops the sequence at its step and the right value comes back
    order_n = 0; md_result = -543;
    if (sceInetInterfaceControl(&object, buffer, 1, 2, data, 8) != -543 || order_n != 1) return 7;
    order_n = 0; md_result = 0; call_result = -1;
    if (sceInetInterfaceControl(&object, buffer, 1, 2, data, 8) != -542 || order_n != 2) return 8;
    order_n = 0; call_result = 0; ms_result = -543;
    if (sceInetInterfaceControl(&object, buffer, 1, 2, data, 8) != -543 || order_n != 3) return 9;
    ms_result = 0; reply = -2;                                // negative reply word is returned after a good copy-back
    if (sceInetInterfaceControl(&object, buffer, 1, 2, data, 0) != -2 || call_send != 12) return 10;
    return 0;
}

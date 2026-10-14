// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp inet_open.cpp
static int calls, result, fn, send_size, recv_size, reply, w0_at_call, w1_at_call; static void *seen_obj, *seen_buf;
extern "C" {
int CallRpc(void* object, int function, void* buffer, int s, int r) {
    ++calls; seen_obj = object; fn = function; seen_buf = buffer; send_size = s; recv_size = r;
    w0_at_call = static_cast<int*>(buffer)[0]; w1_at_call = static_cast<int*>(buffer)[1];
    static_cast<int*>(buffer)[0] = reply;
    return result;
}
int sceInetOpen(void* object, int* buffer, int first, int second);
}
int main() {
    int object, buffer[8] = {0, 0, 0x55, 0x55, 0, 0, 0, 0};
    reply = 31;
    if (sceInetOpen(&object, buffer, 0x1234, -7) != 31 || calls != 1) return 1;
    if (seen_obj != &object || seen_buf != buffer || fn != 2 || send_size != 8 || recv_size != 4) return 2;
    if (w0_at_call != 0x1234 || w1_at_call != -7 || buffer[2] != 0x55) return 3;
    result = -3; reply = 99;
    if (sceInetOpen(&object, buffer, 1, 2) != -542) return 4;
    result = 5; reply = -4;                                  // non-negative call result, negative reply passes through
    if (sceInetOpen(&object, buffer, 1, 2) != -4) return 5;
    return 0;
}

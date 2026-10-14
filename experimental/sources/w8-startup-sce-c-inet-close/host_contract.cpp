#include <cstdio>
#include "inet_close.cpp"
static int rpc_result, reply, calls, seen_function, seen_send, seen_receive, seen_b0, seen_b1;
static void* seen_object; static void* seen_buffer;
extern "C" int CallRpc(void* object, int function, void* buffer, int send_size, int receive_size) {
    ++calls; seen_object = object; seen_function = function; seen_buffer = buffer;
    seen_send = send_size; seen_receive = receive_size;
    seen_b0 = static_cast<int*>(buffer)[0]; seen_b1 = static_cast<int*>(buffer)[1];
    static_cast<int*>(buffer)[0] = reply;
    return rpc_result;
}
int main() {
    int buffer[4] = {}, object;
    rpc_result = 0; reply = 77;
    if (sceInetClose(&object, buffer, 5, 1000) != 77) return 1;
    if (calls != 1 || seen_object != &object || seen_buffer != buffer || seen_function != 3 || seen_send != 8 || seen_receive != 4) return 2;
    if (seen_b0 != 5 || seen_b1 != 1000 || buffer[1] != 1000) return 3;
    rpc_result = -1; reply = 9;
    if (sceInetClose(&object, buffer, 6, 7) != -542) return 4;
    rpc_result = 3; reply = -5;
    if (sceInetClose(&object, buffer, 6, 7) != -5) return 5;
    std::puts("sceInetClose contract passed");
    return 0;
}

#include <cstdio>
#include "down_interface.cpp"
static int rpc_result, reply, calls, seen_function, seen_send, seen_receive, seen_b0;
static void* seen_object; static void* seen_buffer;
extern "C" int CallRpc(void* object, int function, void* buffer, int send_size, int receive_size) {
    ++calls; seen_object = object; seen_function = function; seen_buffer = buffer;
    seen_send = send_size; seen_receive = receive_size;
    seen_b0 = static_cast<int*>(buffer)[0];
    static_cast<int*>(buffer)[0] = reply;
    return rpc_result;
}
int main() {
    int buffer[2] = {}, object;
    rpc_result = 0; reply = 41;
    if (sceInetCtlDownInterface(&object, buffer, 3) != 41) return 1;
    if (calls != 1 || seen_object != &object || seen_buffer != buffer || seen_function != 51 || seen_send != 4 || seen_receive != 4 || seen_b0 != 3) return 2;
    rpc_result = -2;
    if (sceInetCtlDownInterface(&object, buffer, 0) != -542) return 3;
    rpc_result = 1; reply = -9;
    if (sceInetCtlDownInterface(&object, buffer, 0) != -9) return 4;
    std::puts("sceInetCtlDownInterface contract passed");
    return 0;
}

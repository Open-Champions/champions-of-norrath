#include <cstdio>
#include "get_state.cpp"
static int rpc_result, reply0, reply1, calls, seen_function, seen_send, seen_receive, seen_b0, seen_b1;
static void* seen_object; static void* seen_buffer;
extern "C" int CallRpc(void* object, int function, void* buffer, int send_size, int receive_size) {
    ++calls; seen_object = object; seen_function = function; seen_buffer = buffer;
    seen_send = send_size; seen_receive = receive_size;
    int* words = static_cast<int*>(buffer);
    seen_b0 = words[0]; seen_b1 = words[1];
    words[0] = reply0; words[1] = reply1;
    return rpc_result;
}
int main() {
    int buffer[4] = {}, object, state = 6;
    rpc_result = 0; reply0 = 11; reply1 = 22;
    if (sceInetCtlGetState(&object, buffer, 2, &state) != 11) return 1;
    if (calls != 1 || seen_object != &object || seen_buffer != buffer || seen_function != 53 || seen_send != 8 || seen_receive != 8) return 2;
    if (seen_b0 != 2 || seen_b1 != 6 || state != 22) return 3;
    if (sceInetCtlGetState(&object, buffer, 4, nullptr) != 11 || seen_b0 != 4 || seen_b1 != 0) return 4;
    state = 99; rpc_result = -1;
    if (sceInetCtlGetState(&object, buffer, 2, &state) != -542 || state != 99) return 5;
    if (sceInetCtlGetState(&object, buffer, 2, nullptr) != -542) return 6;
    std::puts("sceInetCtlGetState contract passed");
    return 0;
}

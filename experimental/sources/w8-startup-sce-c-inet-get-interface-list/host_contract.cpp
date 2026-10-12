#include <cstdio>
#include "get_interface_list.cpp"
static int rpc_result, reply, mem_result, calls, mem_calls, seen_function, seen_send, seen_receive, seen_b0;
static void *seen_object, *seen_buffer, *mem_object, *mem_dest, *mem_src; static int mem_flag, mem_size;
extern "C" int CallRpc(void* object, int function, void* buffer, int send_size, int receive_size) {
    ++calls; seen_object = object; seen_function = function; seen_buffer = buffer;
    seen_send = send_size; seen_receive = receive_size;
    seen_b0 = static_cast<int*>(buffer)[0];
    static_cast<int*>(buffer)[0] = reply;
    return rpc_result;
}
extern "C" int MemcpyS(void* object, void* destination, void* source, int flag, int size) {
    ++mem_calls; mem_object = object; mem_dest = destination; mem_src = source; mem_flag = flag; mem_size = size;
    return mem_result;
}
int main() {
    int buffer[16] = {}, object, dest[16];
    rpc_result = 0; reply = 3; mem_result = 0;
    if (sceInetGetInterfaceList(&object, buffer, dest, 10) != 3) return 1;
    if (calls != 1 || seen_object != &object || seen_buffer != buffer || seen_function != 8 || seen_send != 4 || seen_receive != 44 || seen_b0 != 10) return 2;
    if (mem_calls != 1 || mem_object != &object || mem_dest != dest || mem_src != buffer || mem_flag != 1 || mem_size != 12) return 3;
    mem_calls = 0; reply = -7;
    if (sceInetGetInterfaceList(&object, buffer, dest, 2) != -7 || mem_calls != 0) return 4;
    rpc_result = -1; reply = 3;
    if (sceInetGetInterfaceList(&object, buffer, dest, 2) != -542 || mem_calls != 0) return 5;
    rpc_result = 0; mem_result = -4;
    if (sceInetGetInterfaceList(&object, buffer, dest, 2) != -4 || mem_calls != 1) return 6;
    reply = 0; mem_result = 0;
    if (sceInetGetInterfaceList(&object, buffer, dest, 2) != 0 || mem_size != 0) return 7;
    std::puts("sceInetGetInterfaceList contract passed");
    return 0;
}

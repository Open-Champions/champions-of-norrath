#include "unet_startup_async.cpp"

#include <cstdio>

int net_ready;
int unet_startup_thread_id;
unsigned char unet_startup_thread_stack[8300];
char _gp[4];
void unet_startup_thread__FPv(void*) {}

static int words[6], created, started, prio_id, prio_value, start_arg_null, order, id_at_start, create_result;
int CreateThread(int* w) {
    ++created;
    for (int i = 1; i <= 5; ++i) words[i] = w[i];
    return create_result;
}
int StartThread(int id, void* arg) {
    started = id;
    start_arg_null = arg == nullptr;
    id_at_start = unet_startup_thread_id;
    return 0;
}
int ChangeThreadPriority(int id, int value) {
    prio_id = id;
    prio_value = value;
    return 0;
}

int main() {
    net_ready = 1;
    unet_startup_async__Fv();
    if (created != 0) return 1;
    net_ready = 0;
    create_result = -1;
    unet_startup_async__Fv();
    if (created != 1 || unet_startup_thread_id != -1 || started != 0 || prio_value != 0) return 2;
    create_result = 9;
    unet_startup_async__Fv();
    if (unet_startup_thread_id != 9 || started != 9 || !start_arg_null || id_at_start != 9) return 3;
    if (prio_id != 9 || prio_value != 1) return 4;
    if (words[1] != static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(&unet_startup_thread__FPv))) return 5;
    const __UINTPTR_TYPE__ stack = (reinterpret_cast<__UINTPTR_TYPE__>(unet_startup_thread_stack) + 15u) & ~15u;
    if (words[2] != static_cast<int>(stack) || (words[2] & 15) != 0) return 6;
    if (words[3] != 8192 || words[5] != 10) return 7;
    if (words[4] != static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(&_gp))) return 8;
    std::puts("unet startup async contract passed");
    return 0;
}

#include "unet_startup_async_status.cpp"

#include <cstdio>

int net_ready;
int unet_startup_thread_id;
static int status, refers, terminated, deleted, net_ready_at_terminate, order_ok = 1, step;
int ReferThreadStatus(int thread, void* info) {
    ++refers;
    if (thread != unet_startup_thread_id || info != nullptr) order_ok = 0;
    return status;
}
int TerminateThread(int thread) {
    terminated = thread;
    net_ready_at_terminate = net_ready;
    if (step++ != 0) order_ok = 0;
    return 0;
}
int DeleteThread(int thread) {
    deleted = thread;
    if (step++ != 1) order_ok = 0;
    return 0;
}

int main() {
    unet_startup_thread_id = -1;
    if (unet_startup_async_status__Fv() != 0 || refers != 0 || net_ready != 0) return 1;
    unet_startup_thread_id = 4;
    status = 1;
    if (unet_startup_async_status__Fv() != 1 || refers != 1 || net_ready != 0 || unet_startup_thread_id != 4) return 2;
    status = 17;
    if (unet_startup_async_status__Fv() != 1 || terminated != 0) return 3;
    status = 16;
    if (unet_startup_async_status__Fv() != 0) return 4;
    if (net_ready != 1 || net_ready_at_terminate != 1 || terminated != 4 || deleted != 4) return 5;
    if (unet_startup_thread_id != -1 || !order_ok) return 6;
    std::puts("unet startup async status contract passed");
    return 0;
}

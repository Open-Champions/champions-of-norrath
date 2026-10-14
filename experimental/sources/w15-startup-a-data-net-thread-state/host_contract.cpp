#include "net_thread_state.cpp"

#include <cstdio>

int main() {
    if (sizeof net_ready != 4 || sizeof unet_startup_thread_id != 4) return 1;
    if (sizeof unet_startup_thread_stack != 8208) return 2;
    if (net_ready != 0 || unet_startup_thread_id != 0) return 3;
    for (unsigned char byte : unet_startup_thread_stack) if (byte != 0) return 4;
    // the rounded start used by unet_startup_async plus 8192 stays inside the object
    const __UINTPTR_TYPE__ start = reinterpret_cast<__UINTPTR_TYPE__>(unet_startup_thread_stack);
    const __UINTPTR_TYPE__ rounded = ((start + 15u) >> 4) << 4;
    if (rounded + 8192u > start + sizeof unet_startup_thread_stack) return 5;
    if (reinterpret_cast<void*>(&net_ready) == reinterpret_cast<void*>(&unet_startup_thread_id)) return 6;
    std::puts("net thread state contract passed");
    return 0;
}

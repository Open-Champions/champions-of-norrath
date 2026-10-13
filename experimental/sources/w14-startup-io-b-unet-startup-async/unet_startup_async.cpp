// Observed unet_startup_async__Fv at 0x0101c870, 168 bytes. When net_ready
// (0x0118e6b8) is nonzero nothing happens. Otherwise it builds a six-word
// thread record on its stack: word 1 is unet_startup_thread__FPv, word 2 is
// unet_startup_thread_stack rounded up to 16 bytes, word 3 is 8192, word 4 is
// _gp and word 5 is 10; word 0 is not stored. CreateThread's result is stored
// in unet_startup_thread_id (0x0118e6bc) even when it is -1. Only when it
// is not -1 does it call StartThread(id, 0) and then
// ChangeThreadPriority(unet_startup_thread_id, 1), reading the id back from
// memory. Unknown: the thread's behavior (unet_startup_thread__FPv is not
// authored here) and the size of the stack area.
extern "C" {
extern int net_ready;
extern int unet_startup_thread_id;
extern unsigned char unet_startup_thread_stack[];
extern char _gp[];
void unet_startup_thread__FPv(void* argument);
int CreateThread(int* words);
int StartThread(int identifier, void* argument);
int ChangeThreadPriority(int identifier, int priority);

void unet_startup_async__Fv() {
    if (net_ready != 0) return;
    int words[6];
    words[1] = static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(&unet_startup_thread__FPv));
    words[2] = static_cast<int>(((reinterpret_cast<__UINTPTR_TYPE__>(unet_startup_thread_stack) + 15u) >> 4) << 4);
    words[3] = 8192;
    words[4] = static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(&_gp));
    words[5] = 10;
    const int thread = CreateThread(words);
    unet_startup_thread_id = thread;
    if (thread == -1) return;
    StartThread(thread, nullptr);
    ChangeThreadPriority(unet_startup_thread_id, 1);
}
}

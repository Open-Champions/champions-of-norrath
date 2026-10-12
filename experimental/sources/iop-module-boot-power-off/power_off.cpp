// Observed PreparePowerOff__Fv at 0x002cc230, 140 bytes. It builds a
// semaphore record with 1000 at word 1, 0 at word 2, and 0 at word 5. The
// other three words are not stored. CreateSema's result is stored in sid at
// 0x00486db4 before CreateThread. The thread record stores PowerOffThread__FPv
// at word 1, the local stack at 0x008d8e50 at word 2, 8192 at word 3, _gp at
// word 4, and 1 at word 5. Word 0 is not stored. CreateThread's result is
// stored in tid at 0x00486db8 before StartThread(result, 0). It then calls
// sceCdPOffCallback(PowerOffHandler__FPv, 0). sid and tid are .sbss, st_size
// 0. tid's next symbol is 8 bytes later; only one word is stored. stack is
// .bss, st_size 0. already.1201 is 8240 bytes later. The thread size argument
// is 8192. The other 48 bytes are not named here.
extern "C" {
int sid = 0;
int tid = 0;
}
extern "C" char _gp[];
extern "C" void PowerOffThread__FPv(void* argument);
extern "C" void PowerOffHandler__FPv(void* argument);
extern "C" int CreateSema(int* words);
extern "C" int CreateThread(int* words);
extern "C" int StartThread(int identifier, int* argument);
extern "C" void sceCdPOffCallback(void (*handler)(void*), int argument);

static unsigned char stack[8192];

extern "C" void PreparePowerOff__Fv() {
    int sema_words[6];
    sema_words[1] = 1000;
    sema_words[2] = 0;
    sema_words[5] = 0;
    const int sema = CreateSema(sema_words);
    sid = sema;
    int thread_words[6];
    thread_words[1] = static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(&PowerOffThread__FPv));
    thread_words[2] = static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(stack));
    thread_words[3] = 8192;
    thread_words[4] = static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(&_gp));
    thread_words[5] = 1;
    const int thread = CreateThread(thread_words);
    tid = thread;
    StartThread(thread, nullptr);
    sceCdPOffCallback(PowerOffHandler__FPv, 0);
}

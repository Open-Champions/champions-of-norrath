// Observed InitThread at 0x002d5eb8. Only the stored parameter words are
// defined. The service's remaining parameter words, and the effects of the
// thread services, are unknown.
extern "C" {
extern int topId;
extern int topSema;
extern int topArg[];
extern unsigned char norrath_init_thread_stack[] asm("stack.15");
extern char _gp[];
void topThread();
int CreateSema(int* words);
int DeleteSema(int identifier);
int CreateThread(int* words);
int StartThread(int identifier, int* argument);
int GetThreadId();
int ChangeThreadPriority(int identifier, int priority);
int InitThread() {
    if (topId > 0) return -1;
    int sema_words[6];
    sema_words[1] = 255;
    sema_words[2] = 0;
    int sema = CreateSema(sema_words);
    topSema = sema;
    if (sema < 0) return -1;
    int thread_words[6];
    thread_words[1] = static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(&topThread));
    thread_words[2] = static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(&norrath_init_thread_stack));
    thread_words[3] = 1024;
    thread_words[4] = static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(&_gp));
    thread_words[5] = 0;
    int thread = CreateThread(thread_words);
    topId = thread;
    if (thread < 0) {
        DeleteSema(topSema);
        return -1;
    }
    topArg[0] = 0;
    topArg[1] = 0;
    StartThread(thread, topArg);
    ChangeThreadPriority(GetThreadId(), 1);
    return topId;
}
}

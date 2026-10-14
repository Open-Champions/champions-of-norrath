#include <cstdio>
extern "C" int topId = 0, topSema = 0, topArg[2] = {};
extern "C" unsigned char norrath_init_thread_stack[1024] asm("stack.15") = {};
extern "C" char _gp[4] = {};
extern "C" void topThread() {}
static int sema_result = 3, thread_result = 8, deleted = -1, started = -1, priority_id = -1, priority = -1, thread_calls;
static int sema_words[6], thread_words[6];
extern "C" int CreateSema(int* words) {
    sema_words[1] = words[1];
    sema_words[2] = words[2];
    return sema_result;
}
extern "C" int DeleteSema(int identifier) { deleted = identifier; return 0; }
extern "C" int CreateThread(int* words) {
    ++thread_calls;
    for (int i = 1; i < 6; ++i) thread_words[i] = words[i];
    return thread_result;
}
extern "C" int StartThread(int identifier, int* argument) {
    started = identifier;
    if (argument[0] != 0 || argument[1] != 0) return -1;
    return 0;
}
extern "C" int GetThreadId() { return 15; }
extern "C" int ChangeThreadPriority(int identifier, int rank) {
    priority_id = identifier;
    priority = rank;
    return 0;
}
extern "C" int InitThread();
int main() {
    topId = 4;
    if (InitThread() != -1 || thread_calls != 0) return 1;
    topId = 0;
    sema_result = -2;
    if (InitThread() != -1 || topSema != -2) return 2;
    sema_result = 3;
    thread_result = -5;
    if (InitThread() != -1 || topId != -5 || deleted != 3) return 3;
    topId = 0;
    thread_result = 8;
    deleted = -1;
    if (InitThread() != 8 || started != 8 || priority_id != 15 || priority != 1) return 4;
    if (sema_words[1] != 255 || sema_words[2] != 0) return 5;
    if (thread_words[3] != 1024 || thread_words[5] != 0) return 6;
    if (thread_words[1] != static_cast<int>(reinterpret_cast<unsigned long>(&topThread))) return 7;
    if (thread_words[2] != static_cast<int>(reinterpret_cast<unsigned long>(&norrath_init_thread_stack))) return 8;
    if (thread_words[4] != static_cast<int>(reinterpret_cast<unsigned long>(&_gp))) return 9;
    std::puts("thread service orchestration passed");
}

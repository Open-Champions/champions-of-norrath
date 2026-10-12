#include "power_off.cpp"

#include <cstdio>
#include <cstdlib>

extern "C" char _gp[4] = {};
extern "C" void PowerOffThread__FPv(void*) {}
extern "C" void PowerOffHandler__FPv(void*) {}

static int sema_words[6];
static int thread_words[6];
static int sema_sid_during_thread;
static int tid_during_start;
static int* start_argument;
static int started_id;
static void (*callback)(void*);
static int callback_argument;
static int order;

extern "C" int CreateSema(int* words) {
    if (order != 0) return -1;
    order = 1;
    sema_words[1] = words[1];
    sema_words[2] = words[2];
    sema_words[5] = words[5];
    return 42;
}
extern "C" int CreateThread(int* words) {
    if (order != 1) return -1;
    order = 2;
    sema_sid_during_thread = sid;
    for (int index = 1; index <= 5; ++index) thread_words[index] = words[index];
    return 7;
}
extern "C" int StartThread(int identifier, int* argument) {
    if (order != 2) return -1;
    order = 3;
    tid_during_start = tid;
    started_id = identifier;
    start_argument = argument;
    return 0;
}
extern "C" void sceCdPOffCallback(void (*handler)(void*), int argument) {
    if (order != 3) std::exit(2);
    order = 4;
    callback = handler;
    callback_argument = argument;
}

int main() {
    if (sizeof stack != 8192) return 1;
    PreparePowerOff__Fv();
    if (order != 4 || sema_words[1] != 1000 || sema_words[2] != 0 || sema_words[5] != 0) return 3;
    if (sema_sid_during_thread != 42 || sid != 42) return 4;
    if (thread_words[1] != static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(&PowerOffThread__FPv))) return 5;
    if (thread_words[2] != static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(stack))) return 6;
    if (thread_words[3] != 8192 || thread_words[5] != 1) return 7;
    if (thread_words[4] != static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(&_gp))) return 8;
    if (tid_during_start != 7 || tid != 7 || started_id != 7 || start_argument != nullptr) return 9;
    if (callback != PowerOffHandler__FPv || callback_argument != 0) return 10;
    std::puts("PreparePowerOff passed");
    return 0;
}

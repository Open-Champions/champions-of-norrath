// Host comparison only. Not an EE build input. Every callee is scripted; the order of calls, the
// thread record words, the stored semaphore and thread ids and the priority restore are checked.
// The semaphore-failure path ends in an endless loop, so the scripted printf long-jumps out.
#include <csetjmp>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#define printf norrath_host_printf
#include "create_reader_thread.cpp"
#undef printf
int GLibData[20];
int norrath_lgkbm_reader_thread_record_view[8];
unsigned char norrath_lgkbm_reader_thread_stack_view[16];
unsigned char _gp[4];
void KbmReaderThread() {}
static char events[64]; static int nev;
static int sema_ret, thread_ret, prio_ret = 17, prio_calls, prio_args[4], prio_ids[4], start_id = -99, start_arg = -99, sema_w1, sema_w2;
static std::jmp_buf stop;
static char printed[128];
extern "C" int norrath_host_printf(const char* f, ...) { events[nev++] = 'p'; std::strcpy(printed, f); std::longjmp(stop, 1); }
extern "C" int GetThreadId() { events[nev++] = 'g'; return 7; }
extern "C" int CreateSema(void* p) { events[nev++] = 's'; sema_w1 = static_cast<int*>(p)[1]; sema_w2 = static_cast<int*>(p)[2]; return sema_ret; }
extern "C" int CreateThread(void*) { events[nev++] = 't'; return thread_ret; }
extern "C" void StartThread(int id, int arg) { events[nev++] = 'r'; start_id = id; start_arg = arg; }
extern "C" int ChangeThreadPriority(int id, int pr) { events[nev++] = 'c'; prio_ids[prio_calls] = id; prio_args[prio_calls++] = pr; return prio_ret; }
static void reset() { nev = 0; events[0] = 0; prio_calls = 0; start_id = -99; std::memset(GLibData, 0, sizeof GLibData); std::memset(norrath_lgkbm_reader_thread_record_view, 0, 32); }
int main() {
    int failures = 0;
    sema_ret = 5; thread_ret = 9; reset();
    CreateReaderThread(12);
    events[nev] = 0;
    if (std::strcmp(events, "gstrcc") != 0) { std::printf("order %s\n", events); ++failures; }
    const int* r = norrath_lgkbm_reader_thread_record_view;
    if (r[1] != static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(&KbmReaderThread)) || r[2] != static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(norrath_lgkbm_reader_thread_stack_view)) ||
        r[3] != 6144 || r[4] != static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(_gp)) || r[5] != 12 || r[0] != 0) { std::printf("record words\n"); ++failures; }
    if (sema_w1 != 1 || sema_w2 != 0 || GLibData[10] != 5 || GLibData[12] != 9) { std::printf("sema/thread words\n"); ++failures; }
    if (start_id != 9 || start_arg != 0) { std::printf("start\n"); ++failures; }
    if (prio_calls != 2 || prio_ids[0] != 7 || prio_ids[1] != 7 || prio_args[0] != 30 || prio_args[1] != 17) { std::printf("priority restore\n"); ++failures; }
    // thread creation failure: no start, id still stored
    thread_ret = -4; reset();
    CreateReaderThread(1);
    events[nev] = 0;
    if (std::strcmp(events, "gstcc") != 0 || GLibData[12] != -4) { std::printf("failed thread: %s\n", events); ++failures; }
    thread_ret = 0; reset(); CreateReaderThread(1); events[nev] = 0;
    if (std::strcmp(events, "gstcc") != 0) { std::printf("zero thread id must not start\n"); ++failures; }
    // semaphore failure: prints once, never creates a thread, id stored
    sema_ret = -1; reset();
    if (setjmp(stop) == 0) { CreateReaderThread(1); std::printf("must not return\n"); ++failures; }
    events[nev] = 0;
    if (std::strcmp(events, "gsp") != 0 || GLibData[10] != -1 || std::strcmp(printed, "lgkbmlib::CreateSema Error creating rpc_sema\n") != 0) { std::printf("sema failure %s\n", events); ++failures; }
    if (failures) return 2;
    std::printf("create_reader_thread cases passed\n");
    return 0;
}

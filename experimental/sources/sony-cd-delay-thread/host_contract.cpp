#include <cstdint>
#include <cstdio>
#include <cstring>
#define NORRATH_HOST_CONTRACT 1
#include "delay_thread.cpp"

static unsigned status_bits = 0x10000u;
static int sema_result = 4;
static int alarm_result = 0;
static int clocks_seen = -1;
static int usec_seen = -1;
static void (*handler_seen)(int, int, int, int) = nullptr;
static int sema_seen = -1;
static int signaled = -1;
static int after = 0;
static unsigned char workspace_init = 0;
static unsigned char workspace_max = 0;
static unsigned workspace_name = 0;
extern "C" unsigned norrath_sony_cd_status() { return status_bits; }
extern "C" void norrath_sony_cd_after_signal() { after += 1; }
extern "C" int CreateSema(void* parameters) {
    auto* bytes = static_cast<unsigned char*>(parameters);
    workspace_init = bytes[4];
    workspace_max = bytes[8];
    workspace_name = bytes[20] | (bytes[21] << 8) | (bytes[22] << 16) | (bytes[23] << 24);
    return sema_result;
}
extern "C" int DeleteSema(int) { return 0; }
extern "C" int norrath_player_wait_semaphore(int semaphore) { return semaphore == 4 ? 0 : 1; }
extern "C" int TimerUSec2BusClock(int unused, int microseconds) {
    if (unused != 0) return -1;
    usec_seen = microseconds;
    return 50;
}
extern "C" int SetTimerAlarm(int clocks, void (*handler)(int, int, int, int), int semaphore) {
    clocks_seen = clocks;
    handler_seen = handler;
    sema_seen = semaphore;
    return alarm_result;
}
extern "C" int iSignalSema(int semaphore) {
    signaled = semaphore;
    return 0;
}
int main() {
    status_bits = 0;
    if (static_cast<unsigned>(DelayThread(10)) != 0x80008008u) return 1;
    status_bits = 0x10000u;
    sema_result = -1;
    if (static_cast<unsigned>(DelayThread(10)) != 0x80008003u) return 2;
    sema_result = 4;
    alarm_result = -6;
    if (DelayThread(25) != -6 || usec_seen != 25 || clocks_seen != 50) return 3;
    const unsigned name_bits = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>("DelayThread"));
    if (workspace_init != 1 || workspace_max != 0 || workspace_name != name_bits) return 4;
    alarm_result = 0;
    if (DelayThread(25) != 0 || sema_seen != 4 || handler_seen == nullptr) return 5;
    handler_seen(0, 0, 0, 4);
    if (signaled != 4 || after != 1) return 6;
    std::puts("delay thread contract passed");
    return 0;
}

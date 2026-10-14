#include "file_delay.cpp"

#include <cstdio>

static int order[8], norder, created_with[3], alarm_time, alarm_common, alarm_handler_ok, waited, deleted;

int CreateSema(VIEESemaParam* p) {
    order[norder++] = 1;
    created_with[0] = p->max_count;
    created_with[1] = p->init_count;
    created_with[2] = static_cast<int>(p->option);
    return 41;
}
int SetAlarm(unsigned short time, unsigned (*handler)(int, unsigned short, void*), void* common) {
    order[norder++] = 2;
    alarm_time = time;
    alarm_common = static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(common));
    alarm_handler_ok = handler == VIEEFileDelayTh__FiUsPv;
    return 1;
}
int WaitSema(int s) { order[norder++] = 3; waited = s; return s; }
int DeleteSema(int s) { order[norder++] = 4; deleted = s; return s; }
unsigned VIEEFileDelayTh__FiUsPv(int, unsigned short, void*) { return 0; }

int main() {
    VIEEFileDelay__Fi(480);
    if (norder != 4 || order[0] != 1 || order[1] != 2 || order[2] != 3 || order[3] != 4) return 1;
    if (created_with[0] != 1 || created_with[1] != 0 || created_with[2] != 0) return 2;
    if (alarm_time != 480 || alarm_common != 41 || !alarm_handler_ok) return 3;
    if (waited != 41 || deleted != 41) return 4;
    norder = 0;
    VIEEFileDelay__Fi(0x12345);
    if (alarm_time != 0x2345) return 5;
    std::puts("VIEEFileDelay contract passed");
    return 0;
}

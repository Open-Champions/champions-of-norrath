#include "sync_iop.cpp"

#include <cstdio>
#include <cstdlib>

static int reg_value;
static int reg_index;
static int resets;
static int notifies;
static int notify_index[4];
static int notify_value[4];

extern "C" int sceSifGetReg(int index) {
    reg_index = index;
    return reg_value;
}
extern "C" void sceResetttyinit() {
    if (resets != 0 || notifies != 0) std::exit(2);
    ++resets;
}
extern "C" void sceSifExecNotifyHandler(int index, int value) {
    notify_index[notifies] = index;
    notify_value[notifies] = value;
    ++notifies;
}

int main() {
    reg_value = 0x3ffff;
    if (sceSifSyncIop() != 0 || reg_index != 4 || resets != 0 || notifies != 0) return 3;
    reg_value = 0x40000;
    if (sceSifSyncIop() != 1 || resets != 1 || notifies != 2) return 4;
    if (notify_index[0] != 1 || notify_value[0] != 1 || notify_index[1] != 0 || notify_value[1] != 1) return 5;
    reg_value = 0x40001;
    resets = 0;
    notifies = 0;
    if (sceSifSyncIop() != 1 || resets != 1 || notifies != 2) return 6;
    std::puts("sceSifSyncIop passed");
    return 0;
}

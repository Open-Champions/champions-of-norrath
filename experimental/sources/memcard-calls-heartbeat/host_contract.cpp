#include "heartbeat.cpp"

#include <cstdio>

extern "C" int disableMC = 0;
extern "C" int asyncMode = -1;
extern "C" int errorCondition = 4;
extern "C" int actionCoolOff = 0;
extern "C" signed char whichPad = 0;

static int process_calls;
static int process_error;
static int configure_calls;
static int save_calls;
static int load_calls;
static int config_calls;
static int silent_calls;
static int change_calls;
static int blank_pad;
static int blank_calls;

extern "C" int MCA_Process__FPi(int* error) {
    ++process_calls;
    process_error = *error;
    *error = 9;
    return 1;
}
extern "C" void asyncConfigure__Fv() { ++configure_calls; }
extern "C" void asyncSaveGame__Fv() {
    ++save_calls;
    actionCoolOff = 4;
}
extern "C" void asyncLoadGame__Fv() { ++load_calls; }
extern "C" void asyncSaveConfig__Fv() { ++config_calls; }
extern "C" void asyncSaveConfigSilent__Fv() { ++silent_calls; }
extern "C" void asyncChangePlayers__Fv() { ++change_calls; }
extern "C" void netBlankController__Fii(int pad, int argument) {
    ++blank_calls;
    blank_pad = pad;
    if (argument != 1) blank_pad = -50;
}

void clear_calls() {
    process_calls = configure_calls = save_calls = load_calls = 0;
    config_calls = silent_calls = change_calls = blank_calls = 0;
}

int main() {
    disableMC = 1;
    asyncMode = 3;
    errorCondition = 4;
    actionCoolOff = 2;
    mcHeartBeat__Fv();
    if (process_calls != 0 || errorCondition != 4 || actionCoolOff != 2) return 1;

    disableMC = 0;
    asyncMode = -1;
    actionCoolOff = 2;
    errorCondition = 4;
    mcHeartBeat__Fv();
    if (process_calls != 1 || process_error != 4 || errorCondition != 0 || actionCoolOff != 1)
        return 2;
    if (configure_calls || save_calls || load_calls || config_calls || silent_calls || change_calls)
        return 3;

    clear_calls();
    disableMC = 1;
    asyncMode = 0;
    actionCoolOff = 0;
    mcHeartBeat__Fv();
    if (process_calls != 1 || configure_calls != 1 || errorCondition != 0) return 4;

    clear_calls();
    disableMC = 0;
    asyncMode = 2;
    whichPad = static_cast<signed char>(0x80);
    actionCoolOff = 0;
    mcHeartBeat__Fv();
    if (save_calls != 1 || blank_calls != 1 || blank_pad != -128 || actionCoolOff != 3) return 5;

    clear_calls();
    asyncMode = 3;
    mcHeartBeat__Fv();
    if (load_calls != 1 || save_calls != 0) return 6;
    clear_calls();
    asyncMode = 4;
    mcHeartBeat__Fv();
    if (config_calls != 1) return 7;
    clear_calls();
    asyncMode = 5;
    mcHeartBeat__Fv();
    if (silent_calls != 1) return 8;
    clear_calls();
    asyncMode = 1;
    actionCoolOff = 0;
    mcHeartBeat__Fv();
    if (process_calls != 1 || configure_calls || save_calls || load_calls || change_calls) return 9;
    clear_calls();
    disableMC = 1;
    asyncMode = 7;
    mcHeartBeat__Fv();
    if (change_calls != 1 || process_calls != 1) return 10;
    std::puts("heartbeat passed");
    return 0;
}

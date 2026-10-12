// Host check for netStart. Not an EE plan input.
#include "net_start.cpp"

extern "C" {
int trueNetFrameNm, sentStatePacketFrameNm, localNetFrameNm, noAdvanceTime, derivedFrameNm;
unsigned syncRandState;
}
static int calls = 0, arg = -1;
static int local_at_call = 0, sync_at_call = 0, adv_at_call = 0;
extern "C" void worldPlotRouteSetEnable__Fi(int enable) {
    ++calls; arg = enable;
    local_at_call = localNetFrameNm; sync_at_call = (int)syncRandState; adv_at_call = noAdvanceTime;
    // the original reloads localNetFrameNm after this call
    localNetFrameNm += 100;
}

int main() {
    int values[] = {0, 1, 37, -5, 123456};
    for (int v : values) {
        trueNetFrameNm = v; sentStatePacketFrameNm = 9; localNetFrameNm = 9;
        noAdvanceTime = 4; derivedFrameNm = 9; syncRandState = 77; calls = 0;
        netStart__Fv();
        if (calls != 1 || arg != 0) return 1;
        if (sentStatePacketFrameNm != v) return 2;
        if (local_at_call != v || sync_at_call != 0 || adv_at_call != 4) return 3;
        if (syncRandState != 0 || noAdvanceTime != 0) return 4;
        if (derivedFrameNm != v + 100 - 10) return 5;  // reads the word after the call
    }
    return 0;
}

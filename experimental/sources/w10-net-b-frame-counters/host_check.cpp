// Host check for the frame counter storage. Not an EE plan input.
#include "frame_counters.cpp"
int main() {
    if (trueNetFrameNm != 1 || packetSendCycleLength != 6) return 1;
    if (packetSendCycleTime != 0 || sentStatePacketFrameNm != 0) return 2;
    if (sizeof trueNetFrameNm != 4 || sizeof packetSendCycleLength != 4 ||
        sizeof packetSendCycleTime != 4 || sizeof sentStatePacketFrameNm != 4) return 3;
    return 0;
}

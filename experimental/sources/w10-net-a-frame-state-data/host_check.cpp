// Host check for the frame state storage. Not an EE plan input.
#include "frame_state_data.cpp"
int main() {
    if (localNetFrameNm != 1 || localNetSentFrameNm != 1) return 1;
    if (localPredictedPlayer != 0 || logChecksum != 0 || broadcastConnectionFlags != 0 || msgBufferSent != 0) return 2;
    if (sizeof netCameraYaw != 8 || sizeof kickState != 8 || sizeof localPlayerPadState != 8) return 3;
    for (unsigned short v : netCameraYaw) if (v) return 4;
    for (unsigned char b : kickState) if (b) return 5;
    for (unsigned char b : localPlayerPadState) if (b) return 6;
    if (sizeof localNetFrameNm != 4 || sizeof logChecksum != 4 || sizeof broadcastConnectionFlags != 4 ||
        sizeof msgBufferSent != 4)
        return 7;
    return 0;
}

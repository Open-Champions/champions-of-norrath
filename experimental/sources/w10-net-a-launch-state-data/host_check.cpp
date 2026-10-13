// Host check for the launch state storage. Not an EE plan input.
#include "launch_state_data.cpp"
int main() {
    if (netBroadCastStart != 1 || netAttemptLaunch != 0 || isModem != 0) return 1;
    if (lastPingTime != 0.0f || compressSize != 0 || lnet_buf != 0) return 2;
    if (hostForcedLaunch || hostLaunchMask || hostStartedWithoutMe) return 3;
    if (sizeof launchPeers != 1008 || sizeof myaddr != 16) return 4;
    for (unsigned char b : launchPeers) if (b) return 5;
    for (unsigned char b : myaddr) if (b) return 6;
    if (sizeof netBroadCastStart != 1 || sizeof netAttemptLaunch != 1 || sizeof isModem != 1) return 7;
    if (sizeof hostForcedLaunch != 1 || sizeof hostLaunchMask != 1 || sizeof hostStartedWithoutMe != 1) return 8;
    if (sizeof lastPingTime != 4 || sizeof compressSize != 4) return 9;
    return 0;
}

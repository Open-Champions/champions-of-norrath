// Host check for the launch state storage. Not an EE plan input.
#include "launch_state.cpp"
int main() {
    if (netBroadCastStart != 1 || netAttemptLaunch != 0 || netTrouble != 0) return 1;
    if (multiHelpMessageOn || nmPlayersAlive || localNmPlayersAlive || nmBatchCommands) return 2;
    if (sizeof multiRelease != 4 || sizeof multiTitle != 16 || sizeof multiBody != 16 || sizeof multiSide != 16) return 3;
    for (unsigned char b : multiTitle) if (b) return 4;
    for (unsigned char b : multiBody) if (b) return 5;
    for (unsigned char b : multiSide) if (b) return 6;
    for (unsigned char b : multiRelease) if (b) return 7;
    if (sizeof netBroadCastStart != 1 || sizeof netAttemptLaunch != 1 || sizeof netTrouble != 4) return 8;
    return 0;
}

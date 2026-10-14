// Host check for the voice-chat state storage. Not an EE plan input.
#include "comm_state_data.cpp"
int main() {
    if (commCVol != 0.0f || commNameMask != 0 || commRVInternalLast != 0) return 1;
    if (sizeof commCVol != 4 || sizeof commNameMask != 4 || sizeof commRVInternalLast != 4) return 2;
    return 0;
}

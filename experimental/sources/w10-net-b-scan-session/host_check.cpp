// Host check for the scan mask and session mark storage. Not an EE plan input.
#include "scan_session.cpp"
int main() {
    if (scanMask || sessionMark) return 1;
    if (sizeof scanMask != 4 || sizeof sessionMark != 4) return 2;
    return 0;
}

// Host check only; not a build input.
static int calls, lastMode;
extern "C" int sceCdDiskReady(int mode) { ++calls; lastMode = mode; return 6; }
int norrath_file_iop_stream __asm__("_6VIFile$sIopStream");
#include "cd_disk_ready.cpp"
int main() {
    norrath_file_iop_stream = 0;
    if (CDDiskReady__6VIFilei(0, 1) != 6 || calls != 1 || lastMode != 1) return 1;
    norrath_file_iop_stream = 1;
    if (CDDiskReady__6VIFilei(0, 0) != 2 || calls != 1) return 2;
    return 0;
}

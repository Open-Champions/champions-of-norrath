// Observed: CDDiskReady__6VIFilei at 0x010cb290, 56 bytes. Arguments: the file (unused) and a mode. When the static word
// _6VIFile$sIopStream (0x01192c84) is zero it tail-calls sceCdDiskReady(mode) and returns its result. Otherwise it returns 2
// without calling anything.
// Inferred: sIopStream is the flag EnableIopStreaming__6VIFilei sets; with IOP streaming on the disc status is not polled from the
// EE and "ready" is reported directly. 2 is the value the retry loop in CDSearchFile__6VIFileP10sceCdlFILEPCc waits for.
// Unknown: the meaning of the mode argument beyond being passed on.
extern int norrath_file_iop_stream __asm__("_6VIFile$sIopStream");
extern "C" {
int sceCdDiskReady(int mode);

int CDDiskReady__6VIFilei(unsigned char*, int mode) {
    if (norrath_file_iop_stream != 0) return 2;
    return sceCdDiskReady(mode);
}
}

// Observed: CDSync__6VIFilei at 0x010cac38 is 412 bytes and returns an int. The second
// argument is passed straight to the wait call. Two paths, chosen by the word
// _6VIFile$sIopStream:
// Zero: result = sceCdSync(mode); a nonzero result is returned at once. Otherwise,
// while sceCdGetError() is nonzero and the word at +112 is nonzero: print
// "CDSync: Error %d, restarting previous read\n" with a second sceCdGetError() value;
// while sceCdDiskReady(0) != 2 print "CDSync: Waiting for disk ready\n" and call
// VIEEFileDelay__Fi(480); then sceCdSync(0); sceCdRead with the words at +116,
// +120, +124 and +128; result = sceCdSync(0).
// Nonzero: the same shape with VICdStreamSync(mode) / VICdStreamStatus() in place of
// the sceCdSync / sceCdGetError tests; the wait loop runs while VICdStreamDiskReady()
// is negative and prints "CDSync: Waiting for disc ready\n" (disc, not disk); the
// restart is VICdStreamSync(0), VICdStreamRead(+116, +120, +124, 2),
// result = VICdStreamSync(0).
// Both paths end by storing zero at +112 and returning the last result.
// Inferred: +112 is a retry permission that is dropped when the wait ends;
// +116 to +128 hold the arguments of the last read (sector, count, buffer, mode).
// Unknown: field names, and whether the status value printed is the same error
// the loop condition read (it is read again, so it may differ).
extern "C" {
extern int norrath_file_iop_stream __asm__("_6VIFile$sIopStream");
int sceCdSync(int mode);
int sceCdGetError();
int sceCdDiskReady(int mode);
unsigned sceCdRead(unsigned lsn, unsigned sectors, unsigned buffer, unsigned mode);
int VICdStreamSync__Fi(int mode);
int VICdStreamStatus__Fv();
int VICdStreamDiskReady__Fv();
int VICdStreamRead__FUiUiPvi(unsigned lsn, unsigned sectors, unsigned buffer, int mode);
void VIEEFileDelay__Fi(int ticks);
int printf(const char* format, ...);

int CDSync__6VIFilei(unsigned char* self, int mode) {
    int* retry = reinterpret_cast<int*>(self + 112);
    unsigned* read = reinterpret_cast<unsigned*>(self + 116);
    int result;
    if (norrath_file_iop_stream == 0) {
        result = sceCdSync(mode);
        if (result != 0) return result;
        while (sceCdGetError() != 0 && *retry != 0) {
            printf("CDSync: Error %d, restarting previous read\n", sceCdGetError());
            while (sceCdDiskReady(0) != 2) {
                printf("CDSync: Waiting for disk ready\n");
                VIEEFileDelay__Fi(480);
            }
            sceCdSync(0);
            sceCdRead(read[0], read[1], read[2], read[3]);
            result = sceCdSync(0);
        }
    } else {
        result = VICdStreamSync__Fi(mode);
        if (result != 0) return result;
        while (VICdStreamStatus__Fv() != 0 && *retry != 0) {
            printf("CDSync: Error %d, restarting previous read\n", VICdStreamStatus__Fv());
            while (VICdStreamDiskReady__Fv() < 0) {
                printf("CDSync: Waiting for disc ready\n");
                VIEEFileDelay__Fi(480);
            }
            VICdStreamSync__Fi(0);
            VICdStreamRead__FUiUiPvi(read[0], read[1], read[2], 2);
            result = VICdStreamSync__Fi(0);
        }
    }
    *retry = 0;
    return result;
}
}

// Observed: CDSearchFile__6VIFileP10sceCdlFILEPCc at 0x010cb2c8, 252 bytes. Arguments: the file (unused), an sceCdlFILE record (F)
// and a path. It reads the static word _6VIFile$sIopStream (0x01192c84).
//  - Nonzero: calls VICdSearchFile__FPCcPUiT1(path, &a, &b) (two words on the stack). If the result is negative it returns 0.
//    Otherwise it stores a in F+0 and b in F+4 and returns 1.
//  - Zero: loops: R = sceCdSearchFile(F, path); a nonzero R is returned. On zero, if sceCdGetError() is zero the function returns
//    0. Else it calls sceCdGetError() a second time and prints the text at 0x011d5560 with that result, then polls
//    sceCdDiskReady(0) until it returns 2, printing the text at 0x011d5578 and calling VIEEFileDelay__Fi(480) after each poll that
//    returned anything else, and repeats the search.
// The two texts are the debug lines "CDSearchFile: Error %d" and "CDSearchFile: Waiting for disk ready", each ending in a newline.
// Inferred: a failed lookup with a pending drive error is retried once the disc reports ready. F+0 and F+4 are the first two
// fields of the record (start sector and size).
// Unknown: the unit of the 480 delay, and the contract of VICdSearchFile.
extern int norrath_file_iop_stream __asm__("_6VIFile$sIopStream");
extern "C" {
int printf(const char* format, ...);
int sceCdSearchFile(void* record, const char* path);
int sceCdGetError();
int sceCdDiskReady(int mode);
int VICdSearchFile__FPCcPUiT1(const char* path, unsigned* first, unsigned* second);
void VIEEFileDelay__Fi(int ticks);

int CDSearchFile__6VIFileP10sceCdlFILEPCc(unsigned char*, unsigned* record, const char* path) {
    if (norrath_file_iop_stream != 0) {
        unsigned found[2];
        if (VICdSearchFile__FPCcPUiT1(path, &found[0], &found[1]) < 0) return 0;
        unsigned first = found[0];
        record[0] = first;
        record[1] = found[1];
        return 1;
    }
    for (;;) {
        int result = sceCdSearchFile(record, path);
        if (result != 0) return result;
        if (sceCdGetError() == 0) return 0;
        printf("CDSearchFile: Error %d\n", sceCdGetError());
        while (sceCdDiskReady(0) != 2) {
            printf("CDSearchFile: Waiting for disk ready\n");
            VIEEFileDelay__Fi(480);
        }
    }
}
}

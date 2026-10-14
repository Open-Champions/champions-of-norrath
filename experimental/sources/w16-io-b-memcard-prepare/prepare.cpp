// Observed: VIMemCardPrepare__Fi at 0x010d2e58 is 912 bytes and returns an int.
// The argument is a pad count (pad). Steps, each card command followed by a
// blocking sceMcSync(0, &command, &result) whose result word is read after:
//  1. VIMemCardStatus__FPi(0) nonzero returns -1. "/%s" is formatted with
//     sProductId into a 256-byte stack buffer before that test.
//  2. VIMemCardIsPrepared__Fv(): negative returns -1; 1 returns 0. Its value is
//     also stored in the result word.
//  3. sceMcMkdir(0, 0, path). A result below -9 returns -1. Otherwise
//     printf("prepare result: %d\n", result). -3 returns -2; -2 and -5 return -1;
//     other results (including success) continue.
//  4. Create "/%s/icon.sys" (sceMcOpen mode 514), write 964 bytes from
//     sIconDefinition, sceMcFlush, sceMcSync, sceMcClose, sceMcSync.
//  5. Create "/%s/%s" with the text at sIconDefinition + 260, write the buffer
//     whose pointer is the word at sIconDefinition + 964 and whose length is the
//     word at sIconDefinition + 968, flush, sync, close, sync.
//  6. When pad > 0 call VIMemCardPad__Fi(pad); a negative value is returned.
//     A negative pad returns 0 here.
//  7. Create "/%s/%s" with sProductId as both parts, write the eight bytes
//     "12345678" (copied into the path buffer), flush, sync, close, sync.
//  The result of every open or flush sync: -3 returns -2, another negative returns
//  -1. A negative result after a close sync returns -1. The final return is -1 if
//  the last close result is negative and 0 otherwise.
// Inferred: this lays out a new save directory on the card with an icon
// definition, an icon, optional padding and a marker file. VIMemCardIsPrepared
// always returns 1 in this image, so a call returns 0 after the status check.
// Unknown: the layout of sIconDefinition past the two fields used, the meaning
// of the -2/-3/-5 results, and the marker file's purpose.
extern "C" {
extern char sProductId[];
extern unsigned char sIconDefinition[];
int VIMemCardStatus__FPi(int* freeOut);
int VIMemCardIsPrepared__Fv();
int VIMemCardPad__Fi(int count);
int sprintf(char* buffer, const char* format, ...);
int printf(const char* format, ...);
int sceMcMkdir(int port, int slot, const char* name);
int sceMcOpen(int port, int slot, const char* path, int mode);
int sceMcWrite(int fd, const void* buffer, int size);
int sceMcFlush(int fd);
int sceMcClose(int fd);
int sceMcSync(int mode, int* command, int* result);

int VIMemCardPrepare__Fi(int pad) {
    char path[256];
    int words[2];  // command, result
    int status = VIMemCardStatus__FPi(0);
    sprintf(path, "/%s", sProductId);
    if (status != 0) return -1;
    int prepared = VIMemCardIsPrepared__Fv();
    words[1] = prepared;
    if (prepared < 0) return -1;
    if (prepared == 1) return 0;
    sceMcMkdir(0, 0, path);
    sceMcSync(0, &words[0], &words[1]);
    int result = words[1];
    if (result < -9) return -1;
    printf("prepare result: %d\n", result);
    result = words[1];
    if (result == -3) return -2;
    if (result == -2 || result == -5) return -1;

    sprintf(path, "/%s/icon.sys", sProductId);
    sceMcOpen(0, 0, path, 514);
    sceMcSync(0, &words[0], &words[1]);
    int fd = words[1];
    if (fd == -3) return -2;
    if (fd < 0) return -1;
    sceMcWrite(fd, sIconDefinition, 964);
    sceMcFlush(fd);
    sceMcSync(0, &words[0], &words[1]);
    result = words[1];
    if (result == -3) return -2;
    if (result < 0) return -1;
    sceMcClose(fd);
    sceMcSync(0, &words[0], &words[1]);
    result = words[1];
    if (result < 0) return -1;

    sprintf(path, "/%s/%s", sProductId, reinterpret_cast<char*>(sIconDefinition) + 260);
    sceMcOpen(0, 0, path, 514);
    sceMcSync(0, &words[0], &words[1]);
    fd = words[1];
    if (fd == -3) return -2;
    if (fd < 0) return -1;
    const unsigned iconAddress = *reinterpret_cast<unsigned*>(sIconDefinition + 964);
    sceMcWrite(fd, reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(iconAddress)),
               *reinterpret_cast<int*>(sIconDefinition + 968));
    sceMcFlush(fd);
    sceMcSync(0, &words[0], &words[1]);
    result = words[1];
    if (result == -3) return -2;
    if (result < 0) return -1;
    sceMcClose(fd);
    sceMcSync(0, &words[0], &words[1]);
    result = words[1];
    if (result < 0) return -1;

    if (pad > 0) {
        int padded = VIMemCardPad__Fi(pad);
        words[1] = padded;
        if (padded < 0) return padded;
    }
    if (pad < 0) return 0;

    sprintf(path, "/%s/%s", sProductId, sProductId);
    sceMcOpen(0, 0, path, 514);
    sceMcSync(0, &words[0], &words[1]);
    fd = words[1];
    if (fd == -3) return -2;
    if (fd < 0) return -1;
    __builtin_strcpy(path, "12345678");
    sceMcWrite(fd, path, 8);
    sceMcFlush(fd);
    sceMcSync(0, &words[0], &words[1]);
    result = words[1];
    if (result == -3) return -2;
    if (result < 0) return -1;
    sceMcClose(fd);
    sceMcSync(0, &words[0], &words[1]);
    return words[1] < 0 ? -1 : 0;
}
}

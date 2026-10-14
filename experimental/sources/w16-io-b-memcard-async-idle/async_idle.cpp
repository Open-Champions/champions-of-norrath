// Observed: VIMemCardAsyncIdle__Fv at 0x010d3618 is 272 bytes and returns an int.
// It polls sceMcSync(1, &command, &result). A poll return of -1 returns 1 and a
// return of 0 returns 0 (still running). Otherwise, with result the word the
// sync stored: a negative result sets sAsyncFd = -1 and returns -1; if
// sAsyncFd is already -1 it returns 1. The pending return value starts at 1.
// When sAsyncMode is nonzero it calls sceMcFlush(sAsyncFd) and a blocking
// sceMcSync(0, ...) and re-reads result; when sAsyncMode is zero the pending
// value becomes result, or -1 when result is zero. Then a negative result sets
// sAsyncFd = -1 and returns -1; otherwise sceMcClose(sAsyncFd), a blocking
// sceMcSync(0, ...), and a negative second result sets sAsyncFd = -1 and
// returns -1, else sAsyncFd = -1 and the pending value is returned.
// Inferred: this finishes an asynchronous memory-card file operation and
// closes its descriptor; the zero-result special case reads like a size.
// Unknown: what callers pass as the mode, and the meaning of sAsyncMode.
extern "C" {
extern int sAsyncMode;
extern int sAsyncFd;
int sceMcSync(int mode, int* command, int* result);
int sceMcFlush(int fd);
int sceMcClose(int fd);

int VIMemCardAsyncIdle__Fv() {
    int words[2];  // command, result
    int pending = 1;
    int polled = sceMcSync(1, &words[0], &words[1]);
    if (polled == -1) return 1;
    if (polled == 0) return 0;
    int result = words[1];
    if (result < 0) {
        sAsyncFd = -1;
        return -1;
    }
    if (sAsyncFd == -1) return 1;
    if (sAsyncMode != 0) {
        sceMcFlush(sAsyncFd);
        sceMcSync(0, &words[0], &words[1]);
        result = words[1];
    } else {
        pending = -1;
        if (result != 0) pending = result;
    }
    if (result < 0) {
        sAsyncFd = -1;
        return -1;
    }
    sceMcClose(sAsyncFd);
    sceMcSync(0, &words[0], &words[1]);
    if (words[1] < 0) {
        sAsyncFd = -1;
        return -1;
    }
    sAsyncFd = -1;
    return pending;
}
}

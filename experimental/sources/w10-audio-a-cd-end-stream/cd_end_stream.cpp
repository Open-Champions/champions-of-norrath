// Observed: cdEndStream__Fv at 0x001669c8 is 64 bytes. When the byte streamPaused
// (0x00484010) is non-zero it first calls sceCdStResume, stores 1 in the byte streaming
// (0x00483ffd) and clears streamPaused. In every case it then calls sceCdStStop, clears
// streaming and zeroes the two words streamOutput and streamSizeLeft (0x00484008, 0x0048400c).
// The results of both library calls are dropped.
// Inferred: nothing.
// Unknown: argument lists of the two library calls (none are set up here).

extern "C" {
extern signed char streamPaused;
extern signed char streaming;
extern char* streamOutput;
extern int streamSizeLeft;
int sceCdStResume();
int sceCdStStop();
}

extern "C" void cdEndStream__Fv() {
    if (streamPaused != 0) {
        sceCdStResume();
        streaming = 1;
        streamPaused = 0;
    }
    sceCdStStop();
    streaming = 0;
    streamOutput = 0;
    streamSizeLeft = 0;
}

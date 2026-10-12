// Observed cdClear__Fv at 0x00166cd8, 68 bytes. It runs cdWait__Fv, calls
// sceCdStStop when either the streaming byte or the streamPaused byte is
// nonzero, then clears streamPaused, cdBusy and streaming.
extern "C" {
extern unsigned char cdBusy;
extern unsigned char streaming;
extern unsigned char streamPaused;

void cdWait__Fv();
int sceCdStStop();

void cdClear__Fv() {
    cdWait__Fv();
    if (streaming || streamPaused) sceCdStStop();
    streamPaused = 0;
    cdBusy = 0;
    streaming = 0;
}
}

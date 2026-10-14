// Observed cdWait__Fv at 0x00166d50, 112 bytes. It pushes the border colour
// 0x00ffffff, then while the cdBusy byte is nonzero it runs cdProcess__Fv and
// engineSyncVBlank__Fv. It pops the border colour. A nonzero streaming byte then
// calls sceCdStPause, clears streaming and sets streamPaused to 1.
extern "C" {
extern unsigned char cdBusy;
extern unsigned char streaming;
extern unsigned char streamPaused;

void enginePushBorderColor__FUi(unsigned colour);
void enginePopBorderColor__Fv();
void cdProcess__Fv();
void engineSyncVBlank__Fv();
int sceCdStPause();

void cdWait__Fv() {
    enginePushBorderColor__FUi(0x00ffffffu);
    while (cdBusy) {
        cdProcess__Fv();
        engineSyncVBlank__Fv();
    }
    enginePopBorderColor__Fv();
    if (streaming) {
        sceCdStPause();
        streaming = 0;
        streamPaused = 1;
    }
}
}

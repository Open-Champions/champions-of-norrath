// Observed: cdStreamLoadAsync__FPcT0i at 0x001668b8 is 108 bytes. Arguments are (char* output,
// char* done, int size). When the byte streamPaused is non-zero it calls sceCdStResume, stores
// 1 in streaming and clears streamPaused. It then stores done in cdResult, zero through done,
// output in streamOutput, size in streamSizeLeft, and 1 in the byte cdBusy. It issues no
// disc command itself.
// Inferred: nothing.
// Unknown: the consumer of streamOutput and streamSizeLeft.

extern "C" {
extern signed char streamPaused;
extern signed char streaming;
extern signed char cdBusy;
extern unsigned char* cdResult;
extern char* streamOutput;
extern int streamSizeLeft;
int sceCdStResume();
}

extern "C" void cdStreamLoadAsync__FPcT0i(char* output, char* done, int size) {
    if (streamPaused != 0) {
        sceCdStResume();
        streaming = 1;
        streamPaused = 0;
    }
    cdResult = reinterpret_cast<unsigned char*>(done);
    *done = 0;
    streamOutput = output;
    streamSizeLeft = size;
    cdBusy = 1;
}

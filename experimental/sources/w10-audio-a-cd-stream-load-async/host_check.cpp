#include "cd_stream_load_async.cpp"
#include <cstdio>
signed char streamPaused, streaming, cdBusy;
unsigned char* cdResult;
char* streamOutput;
int streamSizeLeft;
static int resumes;
int sceCdStResume() { ++resumes; return 0; }
int main() {
    char output[8];
    char done = 0x5a;
    for (int paused = 0; paused < 2; ++paused) {
        resumes = 0;
        streamPaused = static_cast<signed char>(paused * 3);
        streaming = 0;
        cdBusy = 0;
        cdStreamLoadAsync__FPcT0i(output, &done, 1234 + paused);
        if (resumes != paused || streaming != paused || streamPaused != 0) return 1;
        if (cdBusy != 1 || done != 0) return 2;
        if (cdResult != reinterpret_cast<unsigned char*>(&done) || streamOutput != output) return 3;
        if (streamSizeLeft != 1234 + paused) return 4;
        done = 0x5a;
    }
    std::puts("cd_stream_load_async passed");
    return 0;
}

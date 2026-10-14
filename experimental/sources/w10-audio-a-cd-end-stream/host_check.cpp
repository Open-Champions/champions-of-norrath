#include "cd_end_stream.cpp"
#include <cstdio>
signed char streamPaused, streaming;
char* streamOutput;
int streamSizeLeft;
static int resumes, stops;
static signed char paused_at_stop, streaming_at_stop;
int sceCdStResume() { ++resumes; return 0; }
int sceCdStStop() { ++stops; paused_at_stop = streamPaused; streaming_at_stop = streaming; return 0; }
int main() {
    char buffer[4];
    for (int paused = 0; paused < 2; ++paused) {
        resumes = stops = 0;
        streamPaused = static_cast<signed char>(paused * 5);
        streaming = 0;
        streamOutput = buffer;
        streamSizeLeft = 77;
        cdEndStream__Fv();
        if (resumes != paused || stops != 1) return 1;
        if (paused_at_stop != 0 || streaming_at_stop != paused) return 2;
        if (streaming != 0 || streamOutput != nullptr || streamSizeLeft != 0) return 3;
    }
    std::puts("cd_end_stream passed");
    return 0;
}

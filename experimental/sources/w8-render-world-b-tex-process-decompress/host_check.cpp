extern "C" {
unsigned* texDecodeStarted;
int drawFrameNm;
int decompressThreadid;
int texLastDecompressionTime;
void texProcessDecompress__Fv();
}
volatile unsigned norrath_host_timer;
static int wake_calls, wake_arg, started_seen;
static unsigned record[10];
extern "C" int WakeupThread(int thread) {
    ++wake_calls;
    wake_arg = thread;
    // The timer is sampled before the call; advance it so the delta is visible.
    started_seen = (texDecodeStarted != nullptr) ? int(texDecodeStarted[7]) : -1;
    norrath_host_timer = norrath_host_timer + 100u;
    return 0;
}
int main() {
    drawFrameNm = 77;
    decompressThreadid = 12;
    norrath_host_timer = 1000;
    texDecodeStarted = nullptr;
    texLastDecompressionTime = -1;
    texProcessDecompress__Fv();
    if (wake_calls != 1 || wake_arg != 12) return 1;
    if (started_seen != -1) return 2;
    if (texLastDecompressionTime != 100) return 3;

    for (auto& w : record) w = 0xabababab;
    texDecodeStarted = record;
    norrath_host_timer = 0xffffffd0u;  // wraps across the call
    texProcessDecompress__Fv();
    if (record[7] != 77u) return 4;
    if (started_seen != 77) return 5;
    for (int i = 0; i < 10; ++i) if (i != 7 && record[i] != 0xabababab) return 6;
    if (texLastDecompressionTime != 100) return 7;
    if (wake_calls != 2) return 8;
    return 0;
}

#include "raster_end_scene.cpp"

#include <cstring>
#include <string>

alignas(4) static unsigned char object[20000];
static std::string trace;
static int flush_sets[2];  // value written to the state word by the first and second Flush
static int flush_calls;
static int flip_sets;
static int release_result;

static int& word(int offset) { return *reinterpret_cast<int*>(object + offset); }

extern "C" int Flush__8VIRaster(VIRaster*) {
    trace += "F;";
    word(19504) = flush_sets[flush_calls++ & 1];
    return 0;
}
extern "C" void VIEEDMASync__Fi(int mode) { trace += "S" + std::to_string(mode) + ";"; }
extern "C" void Flip__8VIRaster(VIRaster*) {
    trace += "P;";
    word(19504) = flip_sets;
}
extern "C" int ReleaseDeferred__8VIRaster(VIRaster*) {
    trace += "R;";
    return release_result;
}

static int run(int state, int flush0, int flush1, int flip, int release, int expect_ret,
               const char* expect_trace, int expect_state, int expect_counter) {
    std::memset(object, 0x33, sizeof object);
    word(19504) = state;
    word(17880) = 40;
    flush_sets[0] = flush0;
    flush_sets[1] = flush1;
    flush_calls = 0;
    flip_sets = flip;
    release_result = release;
    trace.clear();
    int ret = EndScene__8VIRaster(reinterpret_cast<VIRaster*>(object));
    if (ret != expect_ret) return 1;
    if (trace != expect_trace) return 2;
    if (word(19504) != expect_state) return 3;
    if (word(17880) != expect_counter) return 4;
    if (word(17856) != 0 || word(17852) != -1) return 5;
    return 0;
}

int main() {
    // S=2: cleared, counter incremented, no Flip; then S==0 -> Flush.
    if (int e = run(2, 0, 0, 9, 0, 0, "S1;F;R;", 0, 41)) return e;
    if (int e = run(2, 1, 0, 9, 0, 0, "S1;F;R;", 2, 41)) return 10 + e;
    // S=0: Flip then (Flip leaves 0) Flush.
    if (int e = run(0, 1, 0, 0, 0, 0, "S1;P;F;R;", 2, 40)) return 20 + e;
    // S=0, Flip sets 1: no second Flush; S becomes 2.
    if (int e = run(0, 5, 0, 1, 0, 0, "S1;P;R;", 2, 40)) return 30 + e;
    // S=1: Flush + sync first (Flush leaves 1), then Flip leaves 1 -> 2.
    if (int e = run(1, 1, 0, 1, 0, 0, "F;S1;P;R;", 2, 40)) return 40 + e;
    // S=3: first branch runs; Flip sets 3 -> not 1 -> cleared.
    if (int e = run(3, 3, 0, 3, 0, 0, "F;S1;P;R;", 0, 40)) return 50 + e;
    // Flush in step 1 sets 2: that counts as the S==2 case.
    if (int e = run(1, 2, 0, 9, 0, 0, "F;S1;F;R;", 0, 41)) return 60 + e;
    // Release result sign.
    if (int e = run(2, 0, 0, 0, -3, -1, "S1;F;R;", 0, 41)) return 70 + e;
    if (int e = run(2, 0, 0, 0, 12, 0, "S1;F;R;", 0, 41)) return 80 + e;
    return 0;
}

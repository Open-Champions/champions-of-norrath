// Host contract for IsT10K. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// GetRomName is scripted: it counts calls and fills the object the way a successful read would.
extern "C" {
char gRomName[15];
static int calls;
static char scripted[15];
void* GetRomName() { ++calls; for (int i = 0; i < 15; ++i) gRomName[i] = scripted[i]; return gRomName; }
}
#include "is_t10k.cpp"
#include <cstdio>
#include <cstring>
static int bad;
static void expect(const char* what, int got, int want) { if (got != want) { std::printf("%s: got %d want %d\n", what, got, want); ++bad; } }
int main() {
    // already filled: no call, byte 4 decides
    std::memcpy(gRomName, "0160T0003", 10); calls = 0;
    expect("T result", IsT10K(), 1); expect("T calls", calls, 0);
    std::memcpy(gRomName, "0160X0003", 10);
    expect("non-T result", IsT10K(), 0); expect("non-T calls", calls, 0);
    std::memcpy(gRomName, "0160t0003", 10);
    expect("lower-case t is not T", IsT10K(), 0);
    // byte 4 only: a T elsewhere does not count
    std::memcpy(gRomName, "T1600X003", 10);
    expect("T at offset 0 only", IsT10K(), 0);
    // empty: GetRomName runs once and its result is used
    std::memset(gRomName, 0, 15); std::memcpy(scripted, "0160T0003", 10); calls = 0;
    expect("empty->T result", IsT10K(), 1); expect("empty->T calls", calls, 1);
    expect("second call", IsT10K(), 1); expect("second call count", calls, 1);
    std::memset(gRomName, 0, 15); std::memcpy(scripted, "0160X0003", 10); calls = 0;
    expect("empty->X result", IsT10K(), 0); expect("empty->X calls", calls, 1);
    // failed read leaves the object empty: the byte at 4 is zero
    std::memset(gRomName, 0, 15); std::memset(scripted, 0, 15); calls = 0;
    expect("failed read", IsT10K(), 0); expect("failed read calls", calls, 1);
    return bad;
}

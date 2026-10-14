// Host contract for uncompress. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// inflateInit_, inflate and inflateEnd are scripted; the contract records the argument values and the stream
// fields the routine fills, and the order of the calls.
#include <cstdio>
#include <cstring>
#include <string>
static std::string calls;
static int initResult, inflateResult, endResult;
static const void* gotIn; static unsigned gotInLen; static void* gotOut; static unsigned gotOutCap;
static void* gotAlloc; static void* gotFree; static char gotVersion[16]; static int gotSize, gotFlush;
struct NorrathZStream;
#include "uncompress.cpp"
extern "C" int inflateInit_(NorrathZStream* s, const char* v, int size) {
    calls += "init;"; std::strncpy(gotVersion, v, 15); gotSize = size;
    gotIn = s->next_in; gotInLen = s->avail_in; gotOut = s->next_out; gotOutCap = s->avail_out; gotAlloc = s->allocate; gotFree = s->release;
    return initResult;
}
extern "C" int inflate(NorrathZStream* s, int flush) { calls += "inflate;"; gotFlush = flush; s->total_out = 1234; return inflateResult; }
extern "C" int inflateEnd(NorrathZStream*) { calls += "end;"; return endResult; }
int main() {
    int bad = 0;
    unsigned char in[8], out[8];
    {
        calls.clear(); initResult = 0; inflateResult = 1; endResult = 0; unsigned cap = 77;
        int r = uncompress(out, &cap, in, 5);
        if (r != 0 || cap != 1234) { std::puts("success result"); ++bad; }
        if (calls != "init;inflate;end;") { std::printf("order %s\n", calls.c_str()); ++bad; }
        if (gotIn != in || gotInLen != 5 || gotOut != out || gotOutCap != 77 || gotAlloc || gotFree) { std::puts("stream fields"); ++bad; }
        if (std::strcmp(gotVersion, "1.1.4") != 0 || gotSize != 56 || gotFlush != 4) { std::puts("init/inflate arguments"); ++bad; }
    }
    {
        calls.clear(); initResult = 0; inflateResult = 1; endResult = -3; unsigned cap = 77;
        if (uncompress(out, &cap, in, 5) != -3 || cap != 1234) { std::puts("end result passes through"); ++bad; }
    }
    {
        calls.clear(); initResult = -2; unsigned cap = 77;
        if (uncompress(out, &cap, in, 5) != -2 || calls != "init;" || cap != 77) { std::puts("init failure"); ++bad; }
    }
    for (int code : {0, 2, -3, -4, -5}) {
        calls.clear(); initResult = 0; inflateResult = code; endResult = 9; unsigned cap = 77;
        int want = code == 0 ? -5 : code;
        if (uncompress(out, &cap, in, 5) != want || calls != "init;inflate;end;" || cap != 77) { std::printf("inflate result %d\n", code); ++bad; }
    }
    return bad;
}

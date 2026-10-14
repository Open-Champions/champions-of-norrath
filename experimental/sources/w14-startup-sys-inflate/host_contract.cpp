// Host contract for inflate. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// inflate_blocks and inflate_blocks_reset are scripted (the block decoder returns stream end at once and the
// reset stores a scripted running check). The contract walks the wrapper protocol: argument checks, the
// zlib header bytes (accepted and rejected forms, one byte at a time to exercise the resume path), the
// preset-dictionary request, the four trailer bytes (match and mismatch), the no-wrapper path, error
// states and counts. It does not cover the block decoder.
#include <cstdio>
#include <cstring>
#include <string>
#include "inflate.cpp"

static int blocks_ret = 1, blocks_calls, reset_calls;
static unsigned was_value;
extern "C" int inflate_blocks(void*, NorrathZStream*, int r) { ++blocks_calls; return blocks_ret; }
extern "C" void inflate_blocks_reset(void*, NorrathZStream*, unsigned* check) { ++reset_calls; *check = was_value; }

static int fails;
#define CHECK(c, what) do { if (!(c)) { std::printf("fail: %s\n", what); ++fails; } } while (0)

int main() {
    NorrathZState st; NorrathZStream z;
    auto fresh = [&](unsigned mode, int nowrap = 0, unsigned wbits = 15) {
        std::memset(&z, 0, sizeof z); std::memset(&st, 0, sizeof st);
        st.mode = mode; st.nowrap = nowrap; st.wbits = wbits; z.state = &st;
        blocks_calls = reset_calls = 0; blocks_ret = 1; was_value = 0;
    };
    unsigned char in[16] = {0x78, 0x9c, 0, 0, 0, 0};
    fresh(kMethod);
    CHECK(inflate(nullptr, 0) == -2, "null stream");
    z.next_in = in; z.state = nullptr;
    CHECK(inflate(&z, 0) == -2, "null state");
    z.state = &st; z.next_in = nullptr;
    CHECK(inflate(&z, 0) == -2, "null input");
    // no input at all: buffer error for finish, 0 otherwise
    fresh(kMethod); z.next_in = in; z.avail_in = 0;
    CHECK(inflate(&z, 4) == -5 && inflate(&z, 0) == -5, "empty input returns the initial -5");
    // a valid header and a good trailer with a block decoder that ends at once, byte by byte
    fresh(kMethod); z.next_in = in; was_value = 0x01020304u;
    unsigned char all[8] = {0x78, 0x9c, 1, 2, 3, 4};
    z.next_in = all;
    int rc = 0;
    for (int i = 0; i < 6; ++i) {
        z.avail_in = 1;
        rc = inflate(&z, 0);
        if (i < 5) CHECK(rc == 0, "partial progress returns 0");
    }
    CHECK(rc == 1 && st.mode == kDone && z.total_in == 6 && blocks_calls == 1 && reset_calls == 1, "full stream");
    CHECK(inflate(&z, 0) == 1, "done stays done");
    // finish request with input exhausted mid-header returns -5
    fresh(kMethod); z.next_in = all; z.avail_in = 1;
    CHECK(inflate(&z, 4) == -5 && st.mode == kFlag, "finish with partial header");
    // bad method, window and header check
    fresh(kMethod); in[0] = 0x79; z.next_in = in; z.avail_in = 2;
    CHECK(inflate(&z, 0) == -3 && std::strcmp(z.message, "unknown compression method") == 0 && st.sub[0] == 5, "method");
    CHECK(inflate(&z, 0) == -3, "bad stays bad");
    fresh(kMethod, 0, 9); in[0] = 0x78; z.next_in = in; z.avail_in = 2;
    CHECK(inflate(&z, 0) == -3 && std::strcmp(z.message, "invalid window size") == 0 && st.sub[0] == 5, "window");
    fresh(kMethod); in[0] = 0x78; in[1] = 0x9d; z.next_in = in; z.avail_in = 2;
    CHECK(inflate(&z, 0) == -3 && std::strcmp(z.message, "incorrect header check") == 0 && st.sub[0] == 5, "header check");
    // preset dictionary: 78 bb then four id bytes
    unsigned char dict[8] = {0x78, 0xbb, 0x11, 0x22, 0x33, 0x44};
    fresh(kMethod); z.next_in = dict; z.avail_in = 6;
    CHECK(inflate(&z, 0) == 2 && z.adler == 0x11223344u && st.mode == kDict0 && z.avail_in == 0, "need dictionary");
    CHECK(inflate(&z, 0) == -2 && std::strcmp(z.message, "need dictionary") == 0 && st.mode == kBad && st.sub[0] == 0, "dictionary not set");
    // block decoder results
    fresh(kBlocks); z.next_in = in; z.avail_in = 1; blocks_ret = 0;
    CHECK(inflate(&z, 0) == 0 && inflate(&z, 4) == -5 && blocks_calls == 2, "blocks need more");
    fresh(kBlocks); z.next_in = in; z.avail_in = 1; blocks_ret = -3;
    CHECK(inflate(&z, 0) == -3 && st.mode == kBad && st.sub[0] == 0, "blocks data error");
    fresh(kBlocks); z.next_in = in; z.avail_in = 1; blocks_ret = -5;
    CHECK(inflate(&z, 0) == -5, "blocks buffer error is returned");
    // no wrapper: done right after the blocks end, no trailer read
    fresh(kBlocks, 1); z.next_in = in; z.avail_in = 0; was_value = 7;
    CHECK(inflate(&z, 0) == 1 && st.mode == kDone && reset_calls == 1 && st.sub[0] == 7, "nowrap");
    // trailer mismatch
    fresh(kBlocks); was_value = 0x01020305u; z.next_in = all + 2; z.avail_in = 4;
    CHECK(inflate(&z, 0) == -3 && std::strcmp(z.message, "incorrect data check") == 0 && st.sub[0] == 5 && z.avail_in == 0, "trailer mismatch");
    // unknown state value
    fresh(99); z.next_in = in;
    CHECK(inflate(&z, 0) == -2, "unknown state");
    return fails;
}

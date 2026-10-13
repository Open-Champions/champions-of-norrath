// Host contract for _sceSDC. Not a build-plan input.
//   c++ -std=c++17 -DNORRATH_HOST_CONTRACT -o check host_contract.cpp && ./check
// Sync, cache operations and TagLo reads are scripted and logged in order. The
// expected trace is built here from the observed control flow, separately from
// the implementation. Scripted TagLo values exercise lines inside, on, and
// outside each range boundary, on both ways.
#include <cstdio>
#include <string>

namespace {
std::string trace;
unsigned last_op_offset;
unsigned last_op_index;

unsigned scripted_taglo(unsigned offset, unsigned index) {
    // Tags that land below, inside, on the edges of, and above the test ranges.
    const unsigned page = (index >> 6) * 0x1000u;
    const unsigned way = offset ? 0x00400000u : 0x00100000u;
    if (index == 0) return 0x80000000u + way;              // sign bit set: masked, not below
    if (index == 64) return way + 0x00001000u;
    if (index == 128) return way + 0x00002000u;
    return way + page;
}
}  // namespace

extern "C" void norrath_host_sync() { trace += 'S'; }
extern "C" void norrath_host_cache_op(unsigned op, unsigned offset, unsigned index) {
    char buffer[32];
    std::snprintf(buffer, sizeof buffer, "C%x:%u:%u;", op, offset, index);
    trace += buffer;
    last_op_offset = offset;
    last_op_index = index;
}
extern "C" unsigned norrath_host_taglo() {
    trace += 'T';
    return scripted_taglo(last_op_offset, last_op_index);
}

extern "C" void _sceSDC(unsigned begin, unsigned end);

namespace {
// Expected trace, written from the observed sequence. The inclusive range test
// is applied to the masked tag plus index, which is what the original compares.
std::string expected(unsigned begin, unsigned end) {
    std::string out;
    auto in_range = [&](unsigned offset, unsigned index) {
        unsigned tag = scripted_taglo(offset, index) & 0xfffff000u;
        unsigned address = tag + index;
        return address >= begin && address <= end;
    };
    char buffer[32];
    for (unsigned index = 0; index < 4096; index += 64) {
        out += 'S';
        std::snprintf(buffer, sizeof buffer, "C10:0:%u;", index);
        out += buffer;
        out += 'S';
        out += 'T';
        if (in_range(0, index)) {
            out += 'S';
            std::snprintf(buffer, sizeof buffer, "C14:0:%u;", index);
            out += buffer;
            out += 'S';
        }
        out += 'S';
        std::snprintf(buffer, sizeof buffer, "C10:1:%u;", index);
        out += buffer;
        out += 'S';
        out += 'T';
        if (in_range(1, index)) {
            out += 'S';
            std::snprintf(buffer, sizeof buffer, "C14:1:%u;", index);
            out += buffer;
            out += 'S';
        }
        out += 'S';
    }
    return out;
}
}  // namespace

#include "sce_dcache.cpp"

int main() {
    struct Case { unsigned begin, end; };
    const Case cases[] = {
        {0x00102000u, 0x00103fffu},   // several way-0 lines inside the range
        {0x00101040u, 0x00101040u},   // single address: masked tag 0x00101000 plus index 64
        {0x00101041u, 0x00101fffu},   // begin just above that line, so it is excluded
        {0x00000000u, 0x7fffffffu},   // everything positive
        {0x00400000u, 0x00400fffu},   // way-1 tags, none inside the range
        {0x80000000u, 0x80001000u},   // high addresses: sign-extended compare
    };
    int bad = 0;
    for (const Case& c : cases) {
        trace.clear();
        _sceSDC(c.begin, c.end);
        const std::string want = expected(c.begin, c.end);
        if (trace != want) {
            std::printf("trace mismatch begin=%x end=%x\n", c.begin, c.end);
            std::printf("got  %s\nwant %s\n", trace.c_str(), want.c_str());
            ++bad;
        }
    }
    // The single-address case must invalidate exactly one line, on way 0.
    trace.clear();
    _sceSDC(0x00101040u, 0x00101040u);
    const std::string invalidates = "C14:0:64;";
    if (trace.find(invalidates) == std::string::npos || trace.find("C14:") != trace.rfind("C14:")) {
        std::puts("single-address case must invalidate exactly index 64 on way 0");
        ++bad;
    }
    if (bad == 0) std::puts("_sceSDC host contract passed");
    return bad;
}

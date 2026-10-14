// Host contract for __frame_state_for. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// The four table helpers are scripted. Code addresses are small integers (they are only compared); the
// descriptor and common-information entry sit in one host buffer. The contract checks: null results of
// find_fde and extract_cie_info give null without further calls; the work record is cleared, receives
// info's word 4 at offset 4 and its halfword at offset 1018; the common-information instructions run up to
// the entry's end with a null pc pointer; the 'z' augmentation skips decode_uleb128's length; the
// descriptor instructions run only while the address is not below the running pc (first test before the
// loop) with the descriptor's start address as the first pc; and exactly 1144 bytes are copied out.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include "frame_state_for.cpp"

struct Call { const unsigned char* insn; void* state; NorrathCieInfo* info; unsigned char** pcp; unsigned long pc; unsigned word4; unsigned short ra; };
static std::vector<Call> calls;
static std::string trace;
static NorrathFde* the_fde; static unsigned char* first_insn; static const char* aug;
static unsigned info_word4, uleb_len; static int uleb_value;
static std::vector<unsigned> pc_steps; static size_t step_index;
extern "C" {
void* memset(void* d, int v, unsigned long n) { unsigned char* p = (unsigned char*)d; for (unsigned long i = 0; i < n; ++i) p[i] = (unsigned char)v; trace += "M;"; return d; }
NorrathFde* find_fde(void*) { trace += "F;"; return the_fde; }
unsigned char* extract_cie_info(NorrathFde*, NorrathCieInfo* info) {
    trace += "E;"; info->augmentation = aug; info->word4 = info_word4; info->ra_column = 0x1234; return first_insn;
}
unsigned char* execute_cfa_insn(unsigned char* insn, void* state, NorrathCieInfo* info, unsigned char** pcp) {
    trace += pcp ? "P;" : "C;";
    calls.push_back({insn, state, info, pcp, pcp ? (unsigned long)*pcp : 0, *(unsigned*)((unsigned char*)state + 4), *(unsigned short*)((unsigned char*)state + 1018)});
    unsigned char* st = (unsigned char*)state; st[1140] = (unsigned char)(calls.size() + 1);
    if (pcp) *pcp += pc_steps[step_index++ % pc_steps.size()];
    return insn + 4;
}
unsigned char* decode_uleb128(unsigned char* insn, int* value) { trace += "U;"; *value = uleb_value; return insn + uleb_len; }
}

static int fails;
#define CHECK(c, what) do { if (!(c)) { std::printf("fail: %s\n", what); ++fails; } } while (0)

int main() {
    alignas(8) static unsigned char buf[512];
    NorrathFrameState out;
    // layout: common entry at 0 (length 12: ends at 16), descriptor at 64 (length 28: ends at 96)
    auto setup = [&](const char* augmentation, unsigned fde_length) {
        std::memset(buf, 0, sizeof buf);
        *(unsigned*)(buf) = 12;
        the_fde = (NorrathFde*)(buf + 64);
        the_fde->length = fde_length; the_fde->cie_delta = 64 + 4; the_fde->pc_begin = 0x1000; the_fde->pc_range = 0x100;
        first_insn = buf + 8; aug = augmentation; info_word4 = 0xcafe0001u; uleb_len = 1; uleb_value = 0;
        calls.clear(); trace.clear(); step_index = 0; pc_steps = {0x10};
        std::memset(&out, 0xee, sizeof out);
        trace.clear();
    };
    // not found
    setup("", 28); the_fde = nullptr;
    CHECK(__frame_state_for((void*)0x1000, &out) == nullptr && trace == "F;", "no descriptor");
    setup("", 28); first_insn = nullptr;
    CHECK(__frame_state_for((void*)0x1000, &out) == nullptr && trace == "F;E;", "no common entry");
    // plain augmentation: CIE insns at 8 and 12 (end 16), FDE insns start at 80 (descriptor + 16), end 96
    setup("", 28);
    CHECK(__frame_state_for((void*)0x1020, &out) == &out, "returns the caller's record");
    CHECK(trace == "F;E;M;C;C;P;P;P;", "call sequence (the third step passes the address)");
    CHECK(calls.size() >= 4 && calls[0].insn == buf + 8 && calls[1].insn == buf + 12 && calls[0].pcp == nullptr, "common entry walk");
    CHECK(calls[2].insn == buf + 80 && calls[2].pcp != nullptr && calls[2].pc == 0x1000, "descriptor walk starts at descriptor + 16 with the start address");
    CHECK(calls[0].word4 == 0xcafe0001u && calls[0].ra == 0x1234, "work record fields");
    CHECK(out.bytes[1140] == calls.size() + 1, "last executed write is copied out");
    CHECK(out.bytes[1143] == 0 && std::memcmp(out.bytes + 1018, "\x34\x12", 2) == 0, "copy covers 1144 bytes");
    // address below the start: the descriptor walk is skipped
    setup("", 28);
    __frame_state_for((void*)0x0fff, &out);
    CHECK(trace == "F;E;M;C;C;", "address below pc_begin");
    // address inside the first step: one instruction run only
    setup("", 28); pc_steps = {0x1000};
    __frame_state_for((void*)0x1008, &out);
    CHECK(trace == "F;E;M;C;C;P;", "stops after the pc passes the address");
    // 'z' augmentation: skip the uleb128 length (here 1 byte + value 4)
    setup("z", 28); uleb_len = 1; uleb_value = 4;
    __frame_state_for((void*)0x1020, &out);
    CHECK(trace.find("U;") != std::string::npos && calls[2].insn == buf + 80 + 1 + 4, "z skips the augmentation data");
    // other augmentation characters do not
    setup("R", 28);
    __frame_state_for((void*)0x1020, &out);
    CHECK(trace.find("U;") == std::string::npos && calls[2].insn == buf + 80, "only 'z' decodes a length");
    // the walk ends at the descriptor's end: length 12 gives end 80
    setup("", 12);
    __frame_state_for((void*)0x1020, &out);
    CHECK(trace == "F;E;M;C;C;", "empty descriptor body");
    return fails;
}

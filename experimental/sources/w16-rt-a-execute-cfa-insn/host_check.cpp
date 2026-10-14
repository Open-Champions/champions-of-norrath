// Host check (not a plan input). Build:
//   c++ -O1 -std=c++17 -c -DW16_HOST_CHECK -Dexecute_cfa_insn=w16_cfa -Ddecode_uleb128=w16_uleb -Dabort=w16_abort -o /tmp/w16_cfa_src.o execute_cfa_insn.cpp
//   c++ -O1 -std=c++17 -o /tmp/w16_cfa host_check.cpp /tmp/w16_cfa_src.o && /tmp/w16_cfa
// Address words are offsets into a host arena. Every opcode is checked against an independent
// model of what it does to the 1152-byte record, the location and the returned cursor.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

unsigned char* w16_host_arena;
struct CieInfo { const char* text; unsigned eh_pointer; unsigned code_align; int data_align; unsigned return_column; };
struct State {
    unsigned base, word4;
    long long offset_from_base, indirect_offset, extra_value, reg_or_offset[123];
    unsigned short base_reg, return_column;
    signed char saved[123], indirect;
    unsigned saved_state;
};
static_assert(sizeof(State) == 1152, "record size");
extern "C" const unsigned char* w16_cfa(const unsigned char* p, State* state, CieInfo* info, unsigned* pc);

static int aborts, frees, stack_ops;
static std::vector<unsigned> mallocs;
static unsigned arena_next = 0x10000;
static void* last_free;
extern "C" const unsigned char* w16_uleb(const unsigned char* p, unsigned* v) {
    unsigned r = 0, s = 0; unsigned char b;
    do { b = *p++; r |= static_cast<unsigned>(b & 0x7f) << s; s += 7; } while (b & 0x80);
    *v = r; return p;
}
extern "C" const unsigned char* decode_stack_op(const unsigned char* p, State*) { ++stack_ops; return p + 1 + (*p & 3); }
extern "C" void* __wrap_malloc(unsigned n) { mallocs.push_back(n); unsigned a = arena_next; arena_next += 0x800; return w16_host_arena + a; }
extern "C" void __wrap_free(void* p) { ++frees; last_free = p; }
extern "C" void w16_abort() { ++aborts; }

static std::vector<unsigned char> uleb(unsigned v) {
    std::vector<unsigned char> out;
    do { unsigned char b = v & 0x7f; v >>= 7; if (v) b |= 0x80; out.push_back(b); } while (v);
    return out;
}
static void fill(State& s, unsigned seed) {
    unsigned char* b = reinterpret_cast<unsigned char*>(&s);
    unsigned x = seed * 2654435761u + 1;
    for (unsigned i = 0; i < sizeof s; ++i) { x = x * 1664525u + 1013904223u; b[i] = static_cast<unsigned char>(x >> 24); }
    for (int i = 0; i < 123; ++i) s.saved[i] = static_cast<signed char>(i % 3);
}

struct Run { State state; unsigned pc; const unsigned char* result; std::vector<unsigned char> code; };
static Run run(const std::vector<unsigned char>& bytes, unsigned base_reg, unsigned code_align = 4, int data_align = -4) {
    Run r;
    r.code = bytes; r.code.resize(bytes.size() + 8, 0);
    fill(r.state, 5);
    r.state.base_reg = static_cast<unsigned short>(base_reg);
    CieInfo info = {nullptr, 0, code_align, data_align, 0};
    r.pc = 0x1000;
    aborts = frees = stack_ops = 0; mallocs.clear();
    r.result = w16_cfa(r.code.data(), &r.state, &info, &r.pc);
    return r;
}
#define CHECK(cond) do { if (!(cond)) { std::printf("fail line %d\n", __LINE__); return 1; } } while (0)

int main() {
    w16_host_arena = new unsigned char[1 << 20];
    unsigned cases = 0;
    // 0x40..0xff: advance by low six bits * code alignment.
    for (unsigned op : {0x40u, 0x41u, 0x7fu, 0xc0u, 0xc5u, 0xffu}) {
        Run r = run({static_cast<unsigned char>(op)}, 3, 4);
        CHECK(r.pc == 0x1000 + (op & 0x3f) * 4 && r.result == r.code.data() + 1); ++cases;
    }
    // 0x80..0xbf: register offset (operand * data alignment) unless it is the base register.
    {
        auto code = std::vector<unsigned char>{0x85}; for (auto b : uleb(300)) code.push_back(b);
        Run r = run(code, 3, 4, -4);
        CHECK(r.state.saved[5] == 1 && r.state.reg_or_offset[5] == -1200 && r.result == r.code.data() + 3); ++cases;
        Run same = run(code, 5, 4, -4);
        CHECK(same.state.saved[5] == 5 % 3 && same.state.reg_or_offset[5] != -1200); ++cases;
    }
    // Opcode 1: location from four unaligned bytes; 2, 3, 4: unscaled additions.
    { Run r = run({1, 0x78, 0x56, 0x34, 0x12}, 3); CHECK(r.pc == 0x12345678 && r.result == r.code.data() + 5); ++cases; }
    { Run r = run({2, 0xf0}, 3); CHECK(r.pc == 0x1000 + 0xf0 && r.result == r.code.data() + 2); ++cases; }
    { Run r = run({3, 0x34, 0x92}, 3); CHECK(r.pc == 0x1000 + 0x9234 && r.result == r.code.data() + 3); ++cases; }
    { Run r = run({4, 0x01, 0x02, 0x03, 0x84}, 3); CHECK(r.pc == 0x1000 + 0x84030201u && r.result == r.code.data() + 5); ++cases; }
    for (unsigned op : {0u, 7u, 8u}) { Run r = run({static_cast<unsigned char>(op)}, 3); CHECK(r.pc == 0x1000 && r.result == r.code.data() + 1 && aborts == 0); ++cases; }
    // 5: register, offset.   6: not saved.   9: saved in register.
    { Run r = run({5, 9, 3}, 3, 4, -8); CHECK(r.state.saved[9] == 1 && r.state.reg_or_offset[9] == -24 && r.result == r.code.data() + 3); ++cases; }
    { Run r = run({5, 3, 3}, 3, 4, -8); CHECK(r.state.reg_or_offset[3] != -24); ++cases; }
    { Run r = run({6, 20}, 3); CHECK(r.state.saved[20] == 0 && r.result == r.code.data() + 2); ++cases; }
    { Run r = run({9, 22, 77}, 3); CHECK(r.state.saved[22] == 2 && r.state.reg_or_offset[22] == 77 && r.result == r.code.data() + 3); ++cases; }
    // 12, 13, 14: base register and offset.
    { Run r = run({12, 31, 0x90, 0x01}, 3); CHECK(r.state.base_reg == 31 && r.state.offset_from_base == 144 && r.result == r.code.data() + 4); ++cases; }
    { Run r = run({13, 40}, 3); CHECK(r.state.base_reg == 40 && r.result == r.code.data() + 2); ++cases; }
    { Run r = run({14, 0xff, 0xff, 0xff, 0xff, 0x0f}, 3); CHECK(r.state.offset_from_base == -1); ++cases; }
    // 10 then 11: remember state, change, restore. The copy is 1152 bytes taken from the record before the change.
    {
        State s; fill(s, 9); s.saved_state = 0;
        CieInfo info = {nullptr, 0, 4, -4, 0};
        unsigned pc = 0x2000; aborts = frees = 0; mallocs.clear();
        unsigned char c10[] = {10, 0};
        w16_cfa(c10, &s, &info, &pc);
        CHECK(mallocs.size() == 1 && mallocs[0] == 1152);
        unsigned char* copy = w16_host_arena + s.saved_state;
        CHECK(copy == w16_host_arena + 0x10000);
        State before; std::memcpy(&before, copy, sizeof before);
        State kept = s;  // the copy equals the record at the time of the call (apart from the chain word set after)
        CHECK(std::memcmp(&before, &kept, 1144) == 0);
        s.base_reg = 99; s.saved[7] = 1; s.reg_or_offset[7] = 12345; s.offset_from_base = 4242;
        unsigned char c11[] = {11, 0};
        w16_cfa(c11, &s, &info, &pc);
        CHECK(frees == 1 && last_free == copy);
        CHECK(std::memcmp(&s, copy, 1152) == 0);
        CHECK(s.base_reg != 99); ++cases;
    }
    // 15: clears base, offsets and the indirect flag, then runs decode_stack_op over the operand-length bytes.
    { Run r = run({15, 4, 1, 0, 0, 0, 9}, 3); CHECK(r.state.base_reg == 0 && r.state.offset_from_base == 0 && r.state.indirect_offset == 0 && r.state.indirect == 0);
      CHECK(stack_ops == 3 && r.result == r.code.data() + 6); ++cases; }
    // 45: registers 16..31 saved at offset 0, 4, ..., 60.   46: extra value.   47: negated offset.
    { Run r = run({45}, 3); for (int i = 16; i < 32; ++i) CHECK(r.state.saved[i] == 1 && r.state.reg_or_offset[i] == (i - 16) * 4);
      CHECK(r.state.saved[15] == 15 % 3 && r.state.saved[32] == 32 % 3); ++cases; }
    { Run r = run({46, 0x80, 0x01}, 3); CHECK(r.state.extra_value == 128 && r.result == r.code.data() + 3); ++cases; }
    { Run r = run({47, 12, 2}, 3, 4, 8); CHECK(r.state.saved[12] == 1 && r.state.reg_or_offset[12] == -16); ++cases; }
    // Unknown opcodes call abort and return the cursor just past the opcode.
    for (unsigned op : {16u, 44u, 48u, 0x3fu}) { Run r = run({static_cast<unsigned char>(op)}, 3); CHECK(aborts == 1 && r.result == r.code.data() + 1); ++cases; }
    std::printf("%u execute_cfa_insn cases passed\n", cases);
    return 0;
}

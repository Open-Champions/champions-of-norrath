// Host contract for throw_helper. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// The six table/handler helpers and __terminate / __unwinding_cleanup are scripted. The host record has
// pointer-sized fields, so only the logic is checked (the 32-bit layout is covered by the private paired run
// in the notes): handler found in the throw context (no unwinding, offset = last cfa difference + args size),
// handler found in an outer frame (the unwinding replay copies the saved registers other than the return
// column, in order), a cleanup that is kept while the search goes on (and, as observed, replaces a later
// real handler), cleanup-only (the debug hook runs and
// the saved frame is used), no handler (terminate, which is expected not to return, handler slot untouched), the re-throw first lookup (table
// index passed, then cleared) and the old-model lookup.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include "throw_helper.cpp"

struct Script { char* cfa; void* table; long long args; unsigned short column; std::vector<int> saved; long long ra = 0; };
static std::vector<Script> frames; static size_t next_index;
static std::vector<std::pair<void*, int>> finds; static size_t find_index;
static std::vector<void*> olds; static size_t old_index;
static std::string trace;
static void* last_table_index_arg; static int last_rethrow_arg;
static bool terminate_called, cleanup_called;
static std::vector<int> copied;
static std::vector<void*> next_pcs;
static long long ra_word;
extern "C" {
NorrathThrowFrame* next_stack_level(void* pc, NorrathThrowFrame*, NorrathThrowFrame* out) {
    next_pcs.push_back(pc); trace += "N;";
    if (pc == (void*)0x400) next_index = 0;  // a replay restarts at the entry pc
    if (next_index >= frames.size()) return nullptr;
    const Script& s = frames[next_index++];
    out->cfa = s.cfa; out->table = s.table; out->args_size = s.args; out->return_column = s.column;
    std::memset(out->saved, 0, sizeof out->saved);
    for (int i : s.saved) out->saved[i] = 1;
    return out;
}
void* find_exception_handler(void*, void* table, void*, int rethrow, int* cleanup) {
    trace += rethrow ? "R;" : "F;"; last_rethrow_arg = rethrow;
    auto h = finds[find_index++ % finds.size()]; *cleanup = h.second; (void)table; return h.first;
}
void* old_find_exception_handler(void*, void*) { trace += "O;"; return olds[old_index++ % olds.size()]; }
long long* get_reg_addr(unsigned, NorrathThrowFrame* udata, NorrathThrowFrame*) {
    ra_word = 0x77;
    for (const Script& f : frames) if (f.cfa == udata->cfa) ra_word = f.ra;
    return &ra_word;
}
void copy_reg(unsigned reg, NorrathThrowFrame*, NorrathThrowFrame*) { copied.push_back((int)reg); trace += "C;"; }
void __unwinding_cleanup() { cleanup_called = true; trace += "U;"; }
void __terminate() { terminate_called = true; trace += "T;"; throw 0; }  // the real routine does not return
}

static int fails;
#define CHECK(c, what) do { if (!(c)) { std::printf("fail: %s\n", what); ++fails; } } while (0)
static char cfa_base[4096];
static int table_new = -2, table_old = 7;

int main() {
    NorrathThrowContext eh; static NorrathThrowFrame mine; long long offset;
    auto reset = [&]() {
        std::memset(&eh, 0, sizeof eh); eh.info = (void*)0x5000;
        std::memset(&mine, 0, sizeof mine); mine.cfa = cfa_base + 100; mine.return_column = 31;
        frames.clear(); finds.clear(); olds.clear(); next_index = find_index = old_index = 0;
        trace.clear(); copied.clear(); next_pcs.clear(); terminate_called = cleanup_called = false; offset = 0x55;
    };
    void* const entry = (void*)0x400;
    void* h1 = (void*)0x9001; void* h2 = (void*)0x9002;
    // handler found in the throw context's own caller frame: pc unchanged, no unwinding
    reset(); frames = {{cfa_base + 160, &table_new, 12, 31, {}}}; finds = {{h1, 0}};
    void* r = throw_helper(&eh, entry, &mine, &offset);
    CHECK(r == h1 && eh.handler_label == h1 && trace == "N;F;" && copied.empty() && !terminate_called, "context handler");
    CHECK(offset == 12, "handler in the throw context: udata is my_udata, offset = args size");
    CHECK(next_pcs.size() == 1 && next_pcs[0] == entry, "first lookup uses the entry pc");
    // old-model lookup
    reset(); frames = {{cfa_base + 100, &table_old, 0, 31, {}}}; olds = {h2};
    r = throw_helper(&eh, entry, &mine, &offset);
    CHECK(r == h2 && trace == "N;O;" && offset == 0, "old model");
    reset(); frames = {{cfa_base + 100, nullptr, 0, 31, {}}}; olds = {h2};
    r = throw_helper(&eh, entry, &mine, &offset);
    CHECK(r == h2 && trace == "N;O;", "null table is the old model");
    // handler in the second frame: replay copies saved registers except the return column
    reset(); frames = {{cfa_base + 160, &table_new, 8, 31, {1, 4, 31}, 0x1000}, {cfa_base + 220, &table_new, 24, 31, {2, 31, 120}, 0x2000}};
    finds = {{nullptr, 0}, {h1, 0}};
    r = throw_helper(&eh, entry, &mine, &offset);
    CHECK(r == h1 && next_index == 1, "second frame handler (the replay restarts the script)");
    CHECK(trace == "N;F;N;F;N;C;C;", "replay: one frame, saved registers except the return column");
    CHECK(copied.size() == 2 && copied[0] == 1 && copied[1] == 4, "copied registers in index order");
    CHECK(offset == 60 + 24, "offset = replayed frame's cfa difference + the found frame's args");
    // cleanup kept while searching; the saved state wins over a later real handler
    reset(); frames = {{cfa_base + 160, &table_new, 8, 31, {}, 0x1000}, {cfa_base + 220, &table_new, 16, 31, {}, 0x2000}};
    finds = {{h1, 1}, {h2, 0}};
    r = throw_helper(&eh, entry, &mine, &offset);
    CHECK(r == h1 && !cleanup_called && trace == "N;F;N;F;", "a saved cleanup state replaces a later real handler (as observed), no debug hook then");
    // cleanup only: the hook runs and the saved cleanup is returned
    reset(); frames = {{cfa_base + 160, &table_new, 8, 31, {}}}; finds = {{h1, 1}};
    r = throw_helper(&eh, entry, &mine, &offset);
    CHECK(r == h1 && cleanup_called && eh.handler_label == h1, "cleanup only");
    // no handler at all
    reset(); frames = {{cfa_base + 160, &table_new, 8, 31, {}}}; finds = {{nullptr, 0}};
    r = nullptr;
    try { r = throw_helper(&eh, entry, &mine, &offset); } catch (int) {}
    CHECK(terminate_called && r == nullptr && eh.handler_label == nullptr && offset == 0x55, "no handler terminates");
    // re-throw: first lookup with the saved table index and rethrow flag, index cleared
    reset(); eh.table_index = (void*)0x9100; frames = {{cfa_base + 160, &table_new, 8, 31, {}}}; finds = {{h1, 0}};
    r = throw_helper(&eh, entry, &mine, &offset);
    CHECK(r == h1 && trace == "N;R;" && last_rethrow_arg == 1 && eh.table_index == nullptr, "re-throw");
    return fails;
}

// Host check only; not an EE build input. Build the source with -DW15_EH_HOST -Dfind_exception_handler=w15_find_eh;
// this file is compiled with -DW15_EH_HOST only. Address words are offsets into one arena; the matcher call goes
// through w15_eh_host_call_matcher, which records the call and answers from a script.
#include <cstdio>
#include <cstring>
#include "../w15-libs-eh-a-common/eh.hpp"
using w15eh::HandlerEntry;
using w15eh::HandlerTable;
static unsigned char arena[0x2000];
unsigned char* w15_eh_host_base = arena;
static int matcher_calls; static unsigned matcher_fn, matcher_match; static unsigned char* matcher_info; static unsigned char* matcher_table;
static int answers[8]; 
unsigned w15_eh_host_call_matcher(unsigned fn, unsigned char* info, unsigned match, unsigned char* table) {
    matcher_fn = fn; matcher_match = match; matcher_info = info; matcher_table = table;
    return static_cast<unsigned>(answers[matcher_calls++ & 7]);
}
extern "C" unsigned w15_find_eh(unsigned pc, HandlerTable* table, unsigned char* info, int rethrow, int* flag);
static void set_word(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static HandlerTable* make(const HandlerEntry* entries, int n) {
    HandlerTable* t = reinterpret_cast<HandlerTable*>(arena + 0x100);
    std::memset(arena, 0, sizeof arena);
    for (int i = 0; i < n; ++i) t->entries[i] = entries[i];
    t->entries[n].start = 0xffffffffu;
    matcher_calls = 0; std::memset(answers, 0, sizeof answers);
    return t;
}
int main() {
    int flag = 7;
    unsigned char* const info = arena + 0x800;
    const unsigned info_word = 0x800;
    if (w15_find_eh(0x104, nullptr, info, 0, &flag) != 0 || flag != 1) return 1;     // flag is set before the null test
    {   // cleanup entries only: the first applying entry's handler, flag stays 1
        const HandlerEntry e[] = {{0x100, 0x110, 0xa1, 0}, {0x100, 0x120, 0xa2, 0}};
        HandlerTable* t = make(e, 2);
        if (w15_find_eh(0x105, t, info, 0, &flag) != 0xa1 || flag != 1) return 2;   // address - 1 is searched: 0x104
        if (w15_find_eh(0x101, t, info, 0, &flag) != 0xa1) return 3;                 // 0x100 is inside (start inclusive)
        if (w15_find_eh(0x100, t, info, 0, &flag) != 0) return 4;                    // 0xff is below
        if (w15_find_eh(0x111, t, info, 0, &flag) != 0xa2) return 5;                 // 0x110 is outside the first (end exclusive)
        if (matcher_calls != 0) return 6;
    }
    {   // a matcher answers: the handler of the matching entry is returned unless a cleanup was held earlier
        const HandlerEntry e[] = {{0x100, 0x110, 0xb1, 0}, {0x100, 0x110, 0xb2, info_word + 16}, {0x100, 0x110, 0xb3, info_word + 32}};
        HandlerTable* t = make(e, 3);
        set_word(info, 0x4000u);
        answers[0] = 1;
        if (w15_find_eh(0x105, t, info, 0, &flag) != 0xb1 || flag != 0) return 7;   // held cleanup stays the result
        if (matcher_calls != 1 || matcher_fn != 0x4000u || matcher_match != info_word + 16 || matcher_info != info || matcher_table != reinterpret_cast<unsigned char*>(t)) return 8;
        t = make(e, 3);
        set_word(info, 0x4000u);
        answers[0] = 0; answers[1] = 1;
        if (w15_find_eh(0x105, t, info, 0, &flag) != 0xb1 || flag != 0 || matcher_calls != 2) return 9;
        t = make(e, 3);
        set_word(info, 0x4000u);
        if (w15_find_eh(0x105, t, info, 0, &flag) != 0xb1 || flag != 1 || matcher_calls != 2) return 10;   // no match: flag stays 1
        t = make(e + 1, 2);                                                           // no cleanup entry before the match
        set_word(info, 0x4000u);
        answers[0] = 1;
        if (w15_find_eh(0x105, t, info, 0, &flag) != 0xb2 || flag != 0) return 11;
        t = make(e + 1, 2);                                                           // a zero matcher word skips the entry
        std::memset(info, 0, 4);
        if (w15_find_eh(0x105, t, info, 0, &flag) != 0 || flag != 1 || matcher_calls != 0) return 12;
    }
    {   // rethrow: the first argument is an entry word; the search resumes after it and below its end
        const HandlerEntry e[] = {{0x100, 0x110, 0xc1, 0}, {0x100, 0x108, 0xc2, 0}, {0x0f0, 0x108, 0xc3, 0}, {0x104, 0x300, 0xc4, 0}};
        HandlerTable* t = make(e, 4);
        const unsigned first = 0x100 + 8;
        if (w15_find_eh(first + 16, t, info, 1, &flag) != 0xc3 || flag != 1) return 13;   // entry 1's end - 1 = 0x107, from entry 2
        if (w15_find_eh(first + 48, t, info, 1, &flag) != 0) return 14;                   // the terminator entry
        if (w15_find_eh(first + 0, t, info, 2, &flag) != 0xc4) return 15;                 // any nonzero flag is a rethrow
    }
    std::puts("find_exception_handler ok");
    return 0;
}

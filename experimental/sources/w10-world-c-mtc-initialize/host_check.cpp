// Host contract for mtcInitialize__Fv. Not an EE plan input. The record layout is repeated
// from the source; on the host the pointer union is wider, which the source's indexing allows.
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct MtcRecord {
    signed char kind;
    unsigned char pad[3];
    union { const char* name; struct { short modulus; short remainder; } wrap; } arg;
    int start;
    int exit;
};
struct MtcBlock { unsigned char before_records[784]; MtcRecord records[8]; };

extern "C" {
void* mgrg;
char levelName[322];
int mtcStart, mtcExit;
unsigned char _ctype_[257];
void mtcInitialize__Fv();
}

static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }

static MtcBlock block;
static void reset(const char* level) {
    std::memset(&block, 0, sizeof block);
    mgrg = &block; mtcStart = -1; mtcExit = -1;
    std::memset(levelName, 0, sizeof levelName);
    std::strcpy(levelName + 161, level);
}
static void rec(int i, int kind, int start, int exit) { block.records[i].kind = (signed char)kind; block.records[i].start = start; block.records[i].exit = exit; }

int main() {
    for (int c = '0'; c <= '9'; ++c) _ctype_[1 + c] = 4;
    for (int c = 'a'; c <= 'z'; ++c) _ctype_[1 + c] = 0x13;
    reset("hate1");
    mtcInitialize__Fv();
    expect(mtcStart == -1 && mtcExit == -1, "empty first record: nothing stored");

    reset("hate1");
    rec(0, 1, 10, 11); block.records[0].arg.name = "intro";
    rec(1, 1, 20, 21); block.records[1].arg.name = "hate1";
    rec(2, 1, 30, 31); block.records[2].arg.name = "hate1";
    mtcInitialize__Fv();
    expect(mtcStart == 20 && mtcExit == 21, "kind 1: first string match wins, earlier mismatch skipped");

    reset("zzz");
    rec(0, 1, 10, 11); block.records[0].arg.name = "intro";
    mtcInitialize__Fv();
    expect(mtcStart == -1 && mtcExit == -1, "kind 1 with no match stores nothing");

    reset("12");
    rec(0, 3, 1, 2); block.records[0].arg.wrap.modulus = 5; block.records[0].arg.wrap.remainder = 3;
    rec(1, 3, 7, 8); block.records[1].arg.wrap.modulus = 5; block.records[1].arg.wrap.remainder = 2;
    mtcInitialize__Fv();
    expect(mtcStart == 7 && mtcExit == 8, "kind 3: 12 % 5 == 2 picks the second record");

    reset("a12");
    rec(0, 3, 1, 2); block.records[0].arg.wrap.modulus = 5; block.records[0].arg.wrap.remainder = 2;
    rec(1, 2, 40, 41);
    mtcInitialize__Fv();
    expect(mtcStart == 40 && mtcExit == 41, "kind 3 with a non-digit first byte is skipped; kind 2 is chosen");

    reset("-5");
    _ctype_[1 + '-'] = 4;
    rec(0, 3, 1, 2); block.records[0].arg.wrap.modulus = 3; block.records[0].arg.wrap.remainder = -2;
    mtcInitialize__Fv();
    expect(mtcStart == 1 && mtcExit == 2, "signed remainder: -5 % 3 == -2");
    _ctype_[1 + '-'] = 0;

    const int unconditional[] = {4, 7, -1, 2};
    for (int kind : unconditional) {
        reset("x"); rec(0, kind, 100 + kind, 200 + kind);
        mtcInitialize__Fv();
        expect(mtcStart == 100 + kind && mtcExit == 200 + kind, "other kinds are chosen outright");
    }

    reset("q");
    for (int i = 0; i < 8; ++i) { rec(i, 1, i, i + 100); block.records[i].arg.name = "never"; }
    mtcInitialize__Fv();
    expect(mtcStart == -1 && mtcExit == -1, "eight unmatched records, no ninth read");

    reset("q");
    for (int i = 0; i < 7; ++i) { rec(i, 1, i, i + 100); block.records[i].arg.name = "never"; }
    rec(7, 2, 70, 71);
    mtcInitialize__Fv();
    expect(mtcStart == 70 && mtcExit == 71, "the eighth record is reached");

    reset("q");
    rec(0, 1, 1, 2); block.records[0].arg.name = "never";
    rec(2, 2, 70, 71);
    mtcInitialize__Fv();
    expect(mtcStart == -1 && mtcExit == -1, "a zero kind ends the run");
    if (failures) return 1;
    std::printf("mtcInitialize contract passed\n");
    return 0;
}

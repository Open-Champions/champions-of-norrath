#define NORRATH_W8_HOST 1
#include <cstdio>
#include <cstring>
static unsigned sentinel;
extern void* norrath_w8_word_to_pointer(unsigned word);
void* norrath_w8_word_to_pointer(unsigned word) { return word == 0 ? nullptr : &sentinel + word; }
#include "lump_query.cpp"
unsigned lumps[128 * 10];
int nmLumps;
static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
static void record(int i, const char* name, unsigned data, unsigned flags) {
    std::memset(&lumps[i * 10], 0, 40);
    std::strcpy(reinterpret_cast<char*>(&lumps[i * 10]), name);
    lumps[i * 10 + 4] = data;
    lumps[i * 10 + 5] = flags;
}
int main() {
    char font[] = "font.lmp";
    char env[] = "env.lmp";
    char absent[] = "nope.lmp";
    record(0, "env.lmp", 100, 1);
    record(1, "font.lmp", 200, 0);
    record(2, "env.lmp", 300, 2);
    record(3, "env.lmp", 400, 0);
    nmLumps = 4;
    expect(lumpQuery__FPc(font) == &sentinel + 200, "font record");
    expect(lumpQuery__FPc(env) == &sentinel + 300, "skips the env record whose bit 0 is set, takes the first with it clear");
    expect(lumpQuery__FPc(absent) == nullptr, "unknown name");
    nmLumps = 2;
    expect(lumpQuery__FPc(env) == nullptr, "records past nmLumps are not searched");
    nmLumps = 0;
    expect(lumpQuery__FPc(font) == nullptr, "no lumps");
    nmLumps = 4;
    lumps[1 * 10 + 5] = 1;
    expect(lumpQuery__FPc(font) == nullptr, "the only record is flagged");
    return failures;
}

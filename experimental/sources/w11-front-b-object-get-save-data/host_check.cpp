// Host check for objectGetSaveData__FPci. Not an EE plan input.
// The data callee is scripted per pass: result, eligible count, pool bytes added.
#include "object_get_save_data.cpp"
#include <cstring>
char* poolStart;
char* poolEnd;
struct Pass { int result; int eligible; int pool; int count; };
static Pass script[8];
static int calls, seen_pass[8], seen_size, zeroed_ok;
static char* seen_data;
extern "C" int getDataForAllObjects__FPciPiiT2T2(char* data, int size, int* count, int pass, int* eligible, int* omitted) {
    if (calls < 8) seen_pass[calls] = pass;
    seen_size = size;
    seen_data = data;
    zeroed_ok += (*eligible == 0 && *omitted == 0 && poolStart == poolEnd && poolStart != nullptr);
    const Pass& p = script[pass < 8 ? pass : 7];
    ++calls;
    *count = p.count;
    *eligible = p.eligible;
    *omitted = 99;
    if (p.result > 0) for (int i = 0; i < p.result && i < size; ++i) data[i] = (char)(i * 3 + 1);
    for (int i = 0; i < p.pool; ++i) *poolEnd++ = (char)(0x80 + i);
    return p.result;
}
alignas(8) static char out[70000];
static void run_reset() {
    std::memset(out, 0x5a, sizeof out);
    std::memset(script, 0, sizeof script);
    calls = 0;
    zeroed_ok = 0;
    poolStart = poolEnd = nullptr;
}
int main() {
    // fits on the first pass; pool bytes follow data; header; end rounded to 4
    run_reset();
    script[0] = {10, 0, 5, 0x12345};
    int r = objectGetSaveData__FPci(out, 1000);
    // 8 + 10 + 5 = 23 -> 24
    if (r != 24 || calls != 1 || seen_pass[0] != 0 || seen_size != 52000 || zeroed_ok != 1) return 1;
    if (*(unsigned short*)out != 0x2345 || *(unsigned short*)(out + 2) != 1 || *(int*)(out + 4) != 18) return 2;
    for (int i = 0; i < 10; ++i) if (out[8 + i] != (char)(i * 3 + 1)) return 3;
    for (int i = 0; i < 5; ++i) if (out[18 + i] != (char)(0x80 + i)) return 4;
    if (poolStart != out + 18 || poolEnd != out + 24) return 5;
    if (out[24] != 0x5a) return 6;
    // already aligned end
    run_reset();
    script[0] = {8, 0, 0, 3};
    if (objectGetSaveData__FPci(out, 1000) != 16 || poolEnd != out + 16 || poolStart != out + 16) return 7;
    // -1 retries with the next pass; pools are reset each time
    run_reset();
    script[0] = {-1, 0, 7, 0};
    script[1] = {4, 0, 0, 2};
    if (objectGetSaveData__FPci(out, 1000) != 12 || calls != 2 || seen_pass[1] != 1 || zeroed_ok != 2) return 8;
    // over 52000 -> -2, exactly 52000 is allowed
    run_reset();
    script[0] = {52001, 0, 0, 0};
    if (objectGetSaveData__FPci(out, 100000) != -2) return 9;
    run_reset();
    script[0] = {52000, 0, 0, 0};
    if (objectGetSaveData__FPci(out, 100000) != 52008) return 10;
    // pool of 5000 -> -2, 4999 allowed
    run_reset();
    script[0] = {4, 0, 5000, 0};
    if (objectGetSaveData__FPci(out, 100000) != -2 || calls != 1) return 11;
    run_reset();
    script[0] = {4, 0, 4999, 0};
    if (objectGetSaveData__FPci(out, 100000) != ((8 + 4 + 4999 + 3) & ~3)) return 12;
    // too big with eligible objects: tries the next pass until it fits
    run_reset();
    script[0] = {100, 5, 0, 0};
    script[1] = {60, 5, 0, 1};
    script[2] = {20, 5, 0, 2};
    if (objectGetSaveData__FPci(out, 40) != 28 || calls != 3 || *(unsigned short*)out != 2) return 13;
    // exactly room is accepted
    run_reset();
    script[0] = {32, 3, 0, 0};
    if (objectGetSaveData__FPci(out, 40) != 40 || calls != 1) return 14;
    run_reset();
    script[0] = {33, 3, 0, 0};
    script[1] = {32, 3, 0, 0};
    if (objectGetSaveData__FPci(out, 40) != 40 || calls != 2) return 15;
    // too big and eligible below the pass: no further pass, -1 (eligible 1 at pass 2)
    run_reset();
    script[0] = {100, 9, 0, 0};
    script[1] = {100, 9, 0, 0};
    script[2] = {100, 1, 0, 0};
    if (objectGetSaveData__FPci(out, 40) != -1 || calls != 3) return 16;
    // fits but eligible is nonzero and not above the pass -> -1
    run_reset();
    script[0] = {100, 9, 0, 0};
    script[1] = {100, 9, 0, 0};
    script[2] = {8, 2, 0, 0};
    if (objectGetSaveData__FPci(out, 40) != -1 || calls != 3) return 17;
    // eligible 0 and too big: pass 0 retries, pass 1 carries on and writes anyway
    run_reset();
    script[0] = {100, 0, 0, 0};
    script[1] = {100, 0, 0, 0};
    if (objectGetSaveData__FPci(out, 40) != 108 || calls != 2) return 18;
    if (*(int*)(out + 4) != 108) return 19;
    return 0;
}

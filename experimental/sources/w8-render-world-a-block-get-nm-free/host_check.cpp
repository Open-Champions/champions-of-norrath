#include "block_get_nm_free.cpp"
#include <cstdio>
static int calls;
static int answer;
extern "C" int norrath_block_get_number_free() { ++calls; return answer; }
int main() {
    int failures = 0;
    const int values[] = {0, 1, 7, -3, 123456};
    for (int v : values) {
        answer = v;
        calls = 0;
        if (blockGetNmFree__Fv() != v || calls != 1) { std::fprintf(stderr, "fail: value %d\n", v); ++failures; }
    }
    return failures;
}

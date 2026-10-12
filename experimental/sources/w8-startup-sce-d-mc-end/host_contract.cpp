#include "mc_end.cpp"

#include <cstdio>

int semaidRegFunc;
int semaid_timerfunc;
static int deleted[4];
static int deletes;
static int first_word_at_second_delete;

extern "C" int DeleteSema(int id) {
    if (deletes < 4) deleted[deletes] = id;
    deletes += 1;
    if (deletes == 2) first_word_at_second_delete = semaidRegFunc;
    return 0;
}

int main() {
    semaidRegFunc = 7;
    semaid_timerfunc = 9;
    if (sceMcEnd() != 1) return 1;
    if (deletes != 2 || deleted[0] != 7 || deleted[1] != 9) return 2;
    if (first_word_at_second_delete != -1) return 3; /* cleared before the second delete */
    if (semaidRegFunc != -1 || semaid_timerfunc != 9) return 4;
    deletes = 0;
    if (sceMcEnd() != 1 || deletes != 0 || semaid_timerfunc != 9) return 5; /* negative: nothing deleted */
    semaidRegFunc = 0;
    if (sceMcEnd() != 1 || deletes != 2 || deleted[0] != 0) return 6; /* zero counts as nonnegative */
    std::puts("mc end contract passed");
    return 0;
}

// Not an EE plan input. Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "run_gold.cpp"
#include <cstdio>
#include <initializer_list>
#include <cstring>

extern "C" {
alignas(8) unsigned char goldNumber[288 + 64];
alignas(8) unsigned char player[4 * 23984];
int countUp[4 + 8];  // slack so a negative owner would not leave the object
int frameNm;
}
static int bad;
static void expect(const char* what, long got, long want) {
    if (got != want) { std::printf("FAIL %s: got %ld want %ld\n", what, got, want); bad = 1; }
}
static int& field(int rec, int off) { return *reinterpret_cast<int*>(goldNumber + rec * 36 + off); }
static int& gold(int p) { return *reinterpret_cast<int*>(player + p * 23984 + 232); }

int main() {
    std::memset(goldNumber, 0, sizeof goldNumber);
    std::memset(player, 0, sizeof player);
    std::memset(countUp, 0, sizeof countUp);
    frameNm = 1000;
    // A fresh record (age < 43 frames) stays; one aged exactly 43 frames (129 >= 128) moves; 42 (126) stays.
    field(0, 24) = 1000 - 42; field(0, 28) = 50; goldNumber[0 * 36 + 32] = 1;
    field(1, 24) = 1000 - 43; field(1, 28) = 60; goldNumber[1 * 36 + 32] = 2;
    field(7, 24) = 0; field(7, 28) = -16; goldNumber[7 * 36 + 32] = 3;  // old record with a negative amount
    field(5, 24) = 0; field(5, 28) = 0; goldNumber[5 * 36 + 32] = 0;     // zero amount is skipped
    runGold__Fv();
    expect("young stays", field(0, 28), 50);
    expect("old cleared", field(1, 28), 0);
    expect("negative cleared", field(7, 28), 0);
    // Owner 2 received 60: step (60+7)>>3 = 8 added to the player, 52 left. Owner 3 got -16: untouched by step 2.
    expect("count 2", countUp[2], 52);
    expect("gold 2", gold(2), 8);
    expect("count 3", countUp[3], -16);
    expect("gold 3", gold(3), 0);
    expect("gold 1", gold(1), 0);
    // Repeated calls drain the remainder: 52 -> step 7 (59>>3) -> 45 -> 6 ... until 0.
    int total = 8;
    for (int k = 0; k < 40 && countUp[2] > 0; ++k) {
        const int before = countUp[2];
        runGold__Fv();
        total += (before + 7) >> 3;
    }
    expect("drained", countUp[2], 0);
    expect("gold total", gold(2), 60);
    // A count of 1 steps by 1; 8 steps by 1 (15>>3); 9 steps by 2.
    for (int c : {1, 8, 9}) {
        std::memset(countUp, 0, sizeof countUp);
        gold(0) = 100;
        countUp[0] = c;
        runGold__Fv();
        expect("step", gold(0) - 100, (c + 7) >> 3);
        expect("rest", countUp[0], c - ((c + 7) >> 3));
    }
    // The frame comparison is signed: a record newer than the frame count counts as fresh.
    std::memset(goldNumber, 0, sizeof goldNumber);
    std::memset(countUp, 0, sizeof countUp);
    field(2, 24) = 2000; field(2, 28) = 5;
    runGold__Fv();
    expect("future record stays", field(2, 28), 5);
    (void)total;
    std::printf(bad ? "FAIL\n" : "ok: run gold\n");
    return bad;
}

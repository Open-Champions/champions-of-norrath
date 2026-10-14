extern "C" {
alignas(8) unsigned char goldNumber[8 * 36];
alignas(8) unsigned char player[4 * 23984];
int countUp[4];
int goldAlpha[4];
int playerGettinIt[4];
void hudFinishGold__Fv();
}

static int& total(int index) {
    return *reinterpret_cast<int*>(player + index * 23984 + 232);
}
static int& amount(int rec) {
    return *reinterpret_cast<int*>(goldNumber + rec * 36 + 28);
}
static void owner(int rec, int who) {
    goldNumber[rec * 36 + 32] = static_cast<unsigned char>(who);
}

int main() {
    for (int i = 0; i < 4; ++i) {
        total(i) = 100 * (i + 1);
        countUp[i] = 0;
        goldAlpha[i] = 9;
        playerGettinIt[i] = 9;
    }
    amount(0) = 5; owner(0, 2);
    amount(1) = 0; owner(1, 1);       // zero: skipped, nothing added
    amount(2) = 7; owner(2, 2);
    amount(7) = -3; owner(7, 0);      // negative record amounts are applied
    countUp[1] = 11;
    countUp[2] = -4;                  // not positive: skipped and kept
    countUp[3] = 1;
    hudFinishGold__Fv();
    if (total(0) != 97) return 1;
    if (total(1) != 211) return 2;
    if (total(2) != 300 + 12 - 0) return 3;
    if (total(3) != 401) return 4;
    for (int r = 0; r < 8; ++r) if (amount(r) != 0) return 5;
    if (countUp[1] != 0 || countUp[2] != -4 || countUp[3] != 0) return 6;
    for (int i = 0; i < 4; ++i) if (goldAlpha[i] != 0 || playerGettinIt[i] != 0) return 7;
    return 0;
}

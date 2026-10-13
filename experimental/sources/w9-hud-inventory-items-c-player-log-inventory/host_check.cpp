// Host check: the routine must leave the player table untouched.
extern "C" void playerLogInventory__Fi(int);
extern "C" unsigned char player[3 * 23984];
unsigned char player[3 * 23984];
int main() {
    for (unsigned i = 0; i < sizeof player; ++i) player[i] = static_cast<unsigned char>(i * 7 + 3);
    unsigned char copy[sizeof player];
    for (unsigned i = 0; i < sizeof player; ++i) copy[i] = player[i];
    for (int owner = 0; owner < 3; ++owner) playerLogInventory__Fi(owner);
    for (unsigned i = 0; i < sizeof player; ++i) if (player[i] != copy[i]) return 1;
    return 0;
}

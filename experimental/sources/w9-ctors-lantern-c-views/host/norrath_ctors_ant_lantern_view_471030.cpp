// Host comparison only. Not an EE build input.
extern "C" const char norrath_ctors_ant_lantern_view_471030[18][16];
int main() {
    const char* const expected[18] = {
    "weaprack",
    "smalcrat",
    "baskcold",
    "greenpot",
    "kelbask1",
    "kelbask2",
    "gothpic1",
    "gothpic2",
    "gothpic3",
    "gothpic4",
    "gothpic5",
    "gothpic6",
    "crispy01",
    "crispy02",
    "crispy03",
    "gothpot1",
    "gothpot2",
    "ap_barrel"};
    for (unsigned slot = 0; slot < 18; ++slot) {
        unsigned i = 0;
        for (; expected[slot][i] != 0; ++i) {
            if (norrath_ctors_ant_lantern_view_471030[slot][i] != expected[slot][i]) return 1;
        }
        for (; i < 16; ++i) {
            if (norrath_ctors_ant_lantern_view_471030[slot][i] != 0) return 2;
        }
    }
    return 0;
}

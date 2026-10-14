// Observed experienceTable at 0x0034b570, 204 bytes: 51 32-bit words, indexed
// by the level byte of a player record in givePlayerExperience. The words are
// non-decreasing from 0 and the last one is 99999999. Their derivation is not
// established.
extern "C" {
int experienceTable[51] = {
    0, 1100, 3200, 6300, 10400, 15500, 21600, 28700,
    36800, 45900, 56000, 67100, 79200, 92300, 106400, 121500,
    137600, 154700, 172800, 191900, 212000, 233100, 255200, 278300,
    302400, 327500, 353600, 380700, 408800, 437900, 468000, 499100,
    531200, 564300, 598400, 633500, 669600, 706700, 744800, 783900,
    824000, 865100, 907200, 950300, 994400, 1039500, 1085600, 1132700,
    1180800, 1229900, 99999999,
};
}

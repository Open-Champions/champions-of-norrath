// Observed haloColor at 0x00408570, 64 bytes in .data, read as sixteen words: 255, 200, 100, 0,
// then 255, 200, 100, 0, then eight zero words. The authored recall-spell routine indexes it as
// four rows of four ints.
// Inferred: four RGBA-like int rows, of which the first two are preset. The role of the preset
// colours is not established.
extern "C" {
int haloColor[4][4] = {{255, 200, 100, 0}, {255, 200, 100, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
}

// Observed padInitPadIndexMap__Fv at 0x00168228, 44 bytes.
// It stores bytes 3, 2, 1, and 0 at padIndexMap+3 through padIndexMap+0
// (0x00484078). The resulting word is 0x03020100. The three zero words inside
// the body are nops.
extern "C" {
extern unsigned char padIndexMap[4];

void padInitPadIndexMap__Fv() {
    for (int value = 3; value >= 0; --value) padIndexMap[value] = static_cast<unsigned char>(value);
}
}

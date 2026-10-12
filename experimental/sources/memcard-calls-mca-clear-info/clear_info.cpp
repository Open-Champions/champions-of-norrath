// Observed: MCA_ClearInfo__Fv at 0x0025c688 is 92 bytes. It writes eight
// words. MCA_Present receives 0 and MCA_Formatted receives -1, index 0
// through 7, present then formatted at each index.
// Inferred: the two groups of four are the two ports and four slots. The
// final eight pairs do not depend on that grouping.
// Unknown: which later read treats -1 as unformatted.

extern "C" int MCA_Present[8];
extern "C" int MCA_Formatted[8];

extern "C" void MCA_ClearInfo__Fv() {
    for (int index = 0; index < 8; ++index) {
        MCA_Present[index] = 0;
        MCA_Formatted[index] = -1;
    }
}

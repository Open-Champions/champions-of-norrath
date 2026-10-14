// Observed: padIndexMap is the 4 bytes at 0x00484078, zero in the image (the
// next symbol, modelClippingOn, starts at 0x0048407c). padInitPadIndexMap__Fv
// writes 0, 1, 2, 3 into it; padBigRumble and padProcess index it.
// Inferred: the bytes map a logical pad number to a physical pad record.
// Unknown: nothing beyond the extent.
extern "C" signed char padIndexMap[4] = {0, 0, 0, 0};

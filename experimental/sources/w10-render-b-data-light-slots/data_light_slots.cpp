// Observed: lightInc at 0x004085b0 and lightIntensity at 0x004085c0, sixteen bytes each in
// .data with file bytes all zero; lightStart at 0x0037f960, 36 bytes in .data, all zero in the
// file. lightIntensity is used as four ints and lightInc as four floats (the recall-spell
// routine stores the float 0.05 bits 0x3d4ccccc and ints for zero); the other authored
// user clears both with integer zero stores, which is the same bits. lightStart is read by the
// world load routine as ints and as a Point3 at word 6.
// Inferred: per-slot light fade state. Unknown: the meaning of the nine lightStart words.
// Note: the object-infra package also defines this 36-byte object under a view name
// (norrath_factory_marker_data_view); only one of the two should be linked.
extern "C" {
float lightInc[4];
int lightIntensity[4];
int lightStart[9];
}

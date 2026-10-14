// Observed padChangeIndex__Fii at 0x00168258, 32 bytes. It exchanges the bytes
// at positions `first` and `second` of padIndexMap (0x00484078, four bytes,
// reached from gp at -14712). The byte at `second` is read as signed and the
// byte at `first` as unsigned; both are stored back as plain bytes, so the
// signedness has no visible effect. No range check is done on either index.
extern "C" {
extern unsigned char padIndexMap[4];

void padChangeIndex__Fii(int first, int second) {
    const signed char kept = static_cast<signed char>(padIndexMap[second]);
    padIndexMap[second] = padIndexMap[first];
    padIndexMap[first] = static_cast<unsigned char>(kept);
}
}

// Observed lgCodecInit at 0x002eaf50, 8 bytes. It returns 0 and touches
// nothing else. The call in commSystemInit__Fv passes no argument and ignores the
// result.
extern "C" int lgCodecInit() {
    return 0;
}

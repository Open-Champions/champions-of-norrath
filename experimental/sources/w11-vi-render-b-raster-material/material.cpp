// Observed: Material__8VIRasteri at 0x01113880 is 8 bytes. It returns its second argument
// unchanged and touches no memory.
// Unknown: what the integer names; callers decide.
extern "C" {
struct VIRaster;

int Material__8VIRasteri(VIRaster*, int value) {
    return value;
}
}

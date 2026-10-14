// Observed __ti8VIEnvRay, 8 bytes of zeroed type-info node storage in the image.
// The first word is the set flag that the getter __tf8VIEnvRay tests. The remaining bytes are
// the registered name and base pointers, which this unit does not write.
// Inferred from the getter layout only. Unknown: the node fields after the first word.
extern "C" {
unsigned char __ti8VIEnvRay[8] __attribute__((aligned(4))) = {};
}

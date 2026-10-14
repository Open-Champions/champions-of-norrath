// Observed: OnResize__14VIEffectVolumeRC6VIBBox at 0x0114a2c8 is 76 bytes. Arguments: the object and a bounding box of two 3-float
// corners (min at +0, max at +12). It stores 1 in the words at +72 and +32, copies the min corner to +8 and the max corner to +20,
// and returns nothing.
// Inferred: +8 and +20 are the volume's own box (the constructor zeroes them); +32 is a "box set" flag and +72 a changed flag.
// Unknown: whether other state depends on the flags.
extern "C" {
void OnResize__14VIEffectVolumeRC6VIBBox(unsigned char* self, const float* box) {
    float* minimum = reinterpret_cast<float*>(self + 8);
    float* maximum = reinterpret_cast<float*>(self + 20);
    minimum[0] = box[0];
    *reinterpret_cast<unsigned*>(self + 72) = 1;
    minimum[1] = box[1];
    *reinterpret_cast<unsigned*>(self + 32) = 1;
    minimum[2] = box[2];
    maximum[0] = box[3];
    maximum[1] = box[4];
    maximum[2] = box[5];
}
}

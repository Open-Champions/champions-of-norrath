// Observed: Init__14VIEffectVolumeRC6VIBBox at 0x0114a150 is 180 bytes. Arguments: the object and a bounding box (min at +0, max at
// +12). It stores 1 at +32, copies the box corners to +8 (min) and +20 (max), and calls
// SetType__14VIEffectVolume18VIEffectVolumeTypef(this, 0, -1.0f). Afterwards it stores 1 at +72, 1.0f at +44 and +40, 20.0f at +48,
// zero in the eight bytes at +0 and in the words at +52, +56 and +60. It returns 0.
// Inferred: this is the initializer that follows construction; type 0 with a negative scalar keeps the default scalar, which the
// routine then overwrites with 1.0f and 20.0f. The same defaults are written by __14VIEffectVolume.
// Unknown: what +0/+4, +40, +44, +48 and +52..+60 mean beyond the stored defaults.
extern "C" {
void SetType__14VIEffectVolume18VIEffectVolumeTypef(void* self, unsigned type, float value);

int Init__14VIEffectVolumeRC6VIBBox(unsigned char* self, const float* box) {
    float* minimum = reinterpret_cast<float*>(self + 8);
    float* maximum = reinterpret_cast<float*>(self + 20);
    *reinterpret_cast<unsigned*>(self + 32) = 1;
    minimum[0] = box[0];
    minimum[1] = box[1];
    minimum[2] = box[2];
    maximum[0] = box[3];
    maximum[1] = box[4];
    maximum[2] = box[5];
    SetType__14VIEffectVolume18VIEffectVolumeTypef(self, 0, -1.0f);
    *reinterpret_cast<unsigned*>(self + 72) = 1;
    *reinterpret_cast<float*>(self + 44) = 1.0f;
    *reinterpret_cast<float*>(self + 48) = 20.0f;
    *reinterpret_cast<unsigned*>(self + 0) = 0;
    *reinterpret_cast<unsigned*>(self + 4) = 0;
    *reinterpret_cast<float*>(self + 40) = 1.0f;
    *reinterpret_cast<unsigned*>(self + 52) = 0;
    *reinterpret_cast<unsigned*>(self + 56) = 0;
    *reinterpret_cast<unsigned*>(self + 60) = 0;
    return 0;
}
}

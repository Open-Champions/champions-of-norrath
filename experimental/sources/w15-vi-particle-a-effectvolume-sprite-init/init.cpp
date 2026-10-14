// Observed: Init__20VIEffectVolumeSpriteRC6VIBBox at 0x0114dfe0 is 196 bytes. Arguments: the sprite and a bounding box (min at +0,
// max at +12). It copies the min corner to sprite+8 and the max corner to sprite+20, calls Init__8VISphereRC6VIBBox(sprite + 32,
// sprite + 8), copies the min corner to sprite+64 and the max corner to sprite+76, then calls
// Init__14VIEffectVolumeRC6VIBBox(sprite + 88, box). It returns 0.
// Inferred: +88 is the embedded VIEffectVolume that __20VIEffectVolumeSprite constructs, +32 is an embedded sphere, and +64/+76 are
// the two three-float vectors the constructor zeroes (here a second copy of the box).
// Unknown: what the second copy at +64/+76 is used for.
extern "C" {
void Init__8VISphereRC6VIBBox(void* sphere, const void* box);
int Init__14VIEffectVolumeRC6VIBBox(void* volume, const void* box);

int Init__20VIEffectVolumeSpriteRC6VIBBox(unsigned char* self, const float* box) {
    float* minimum = reinterpret_cast<float*>(self + 8);
    float* maximum = reinterpret_cast<float*>(self + 20);
    minimum[0] = box[0];
    minimum[1] = box[1];
    minimum[2] = box[2];
    maximum[0] = box[3];
    maximum[1] = box[4];
    maximum[2] = box[5];
    Init__8VISphereRC6VIBBox(self + 32, minimum);
    float* first = reinterpret_cast<float*>(self + 64);
    float* second = reinterpret_cast<float*>(self + 76);
    first[0] = box[0];
    first[1] = box[1];
    first[2] = box[2];
    second[0] = box[3];
    second[1] = box[4];
    second[2] = box[5];
    Init__14VIEffectVolumeRC6VIBBox(self + 88, box);
    return 0;
}
}

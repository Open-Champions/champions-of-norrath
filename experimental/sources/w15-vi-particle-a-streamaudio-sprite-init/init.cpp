// Observed: Init__19VIStreamAudioSpriteRC6VIBBox at 0x01018f68 is 100 bytes. Arguments: the sprite and a bounding box (min at +0, max
// at +12). It copies the min corner to sprite+8 and the max corner to sprite+20, then calls Init__8VISphereRC6VIBBox(sprite + 32,
// sprite + 8) so the sphere at +32 is derived from the box just stored. It returns 0.
// Inferred: +8 and +20 are the sprite's box and +32 an embedded VISphere; the same sequence starts Init__20VIEffectVolumeSprite.
// Unknown: the size of the embedded sphere and what the 0 return means to the caller.
extern "C" {
void Init__8VISphereRC6VIBBox(void* sphere, const void* box);

int Init__19VIStreamAudioSpriteRC6VIBBox(unsigned char* self, const float* box) {
    float* minimum = reinterpret_cast<float*>(self + 8);
    float* maximum = reinterpret_cast<float*>(self + 20);
    minimum[0] = box[0];
    minimum[1] = box[1];
    minimum[2] = box[2];
    maximum[0] = box[3];
    maximum[1] = box[4];
    maximum[2] = box[5];
    Init__8VISphereRC6VIBBox(self + 32, minimum);
    return 0;
}
}

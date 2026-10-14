// Observed: Init__13VIPointSprite11VIPointType at 0x010cb588 is 156 bytes. Arguments: the sprite and a point type value. It stores
// 0.0f in the three floats at +32 and 0.1f (bits 0x3dcccccc) at +44, stores the type value at +64, then sets the box corners from
// that center and radius: +8..+16 = center - radius per axis, +20..+28 = center + radius per axis. It returns 0.
// Inferred: +32 is a center, +44 a radius, +8 and +20 are the sprite box min/max (as in the stream-audio and effect-volume sprites).
// Unknown: what the point type values select, and why a fixed 0.1f radius is used.
extern "C" {
int Init__13VIPointSprite11VIPointType(unsigned char* self, unsigned type) {
    float* box_minimum = reinterpret_cast<float*>(self + 8);
    float* box_maximum = reinterpret_cast<float*>(self + 20);
    float* center = reinterpret_cast<float*>(self + 32);
    float radius = 0.0f;
    *reinterpret_cast<float*>(self + 44) = 0.1f;
    center[1] = 0.0f;
    center[2] = 0.0f;
    center[0] = 0.0f;
    *reinterpret_cast<unsigned*>(self + 64) = type;
    radius = *reinterpret_cast<float*>(self + 44);
    box_minimum[0] = -radius;
    box_minimum[1] = center[1] - radius;
    box_minimum[2] = center[2] - radius;
    box_maximum[0] = center[0] + radius;
    box_maximum[1] = center[1] + radius;
    box_maximum[2] = center[2] + radius;
    return 0;
}
}

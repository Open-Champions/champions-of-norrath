// Observed setDirLight__Fsssssfb at 0x00109568, 420 bytes, a file-local symbol in
// the image. Arguments: five shorts (yaw angle, pitch angle, and three colour
// components), a float and a flag byte.
// 1. noise = ef1Noise(0x007efe41 + (frameNm << 4)); k = the float argument times
//    noise. The three components are each multiplied by (1 - k) and truncated to a
//    short; the fourth short is zero.
// 2. With c = icos(yaw), s = isin(yaw): B = (c * icos(pitch), s * icos(pitch), 0)
//    (the z product is 0 * icos(pitch)), then the direction is
//    (B.x, B.y, B.z + isin(pitch)).
// 3. A nonzero flag calls lightSetSecondaryDirectional(direction, colour); a zero
//    flag calls lightSetPrimaryDirectional(direction, colour).
// Inferred: the direction is a unit vector from yaw and pitch and the float is a
// flicker amount; callers pass the tag text "r,g,b,..." values of light1/light2.
// Unknown: the exact meaning of the 0x007efe41 offset into ef1Noise.
struct Point3 {
    float x;
    float y;
    float z;
};

extern "C" {
extern int frameNm;
float ef1Noise__Fi(int index);
float icos__Fi(int angle);
float isin__Fi(int angle);
void lightSetSecondaryDirectional__FG6Point3Ps(Point3* direction, short* color);
void lightSetPrimaryDirectional__FG6Point3Ps(Point3* direction, short* color);

void setDirLight__Fsssssfb(short yaw, short pitch, short red, short green, short blue, float amount, char secondary) {
    short color[4];
    color[0] = red;
    color[1] = green;
    color[2] = blue;
    color[3] = 0;
    const float flicker = amount * ef1Noise__Fi(0x007efe41 + (frameNm << 4));
    const float remain = 1.0f - flicker;
    color[0] = static_cast<short>(static_cast<int>(static_cast<float>(color[0]) * remain));
    color[2] = static_cast<short>(static_cast<int>(static_cast<float>(color[2]) * remain));
    color[1] = static_cast<short>(static_cast<int>(static_cast<float>(color[1]) * remain));

    Point3 heading;
    heading.x = icos__Fi(yaw);
    heading.y = isin__Fi(yaw);
    heading.z = 0.0f;
    const float pitch_cos = icos__Fi(pitch);
    Point3 flat = {heading.x * pitch_cos, heading.y * pitch_cos, heading.z * pitch_cos};
    const float pitch_sin = isin__Fi(pitch);
    Point3 direction = {flat.x + 0.0f, flat.y, flat.z + pitch_sin};
    if (secondary != 0) {
        lightSetSecondaryDirectional__FG6Point3Ps(&direction, color);
    } else {
        lightSetPrimaryDirectional__FG6Point3Ps(&direction, color);
    }
}
}

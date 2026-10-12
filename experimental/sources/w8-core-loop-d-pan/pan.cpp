// Observed pan__FG6Point3ffPfT3 at 0x0024cd38, 396 bytes. The Point3 argument
// arrives as an address (a0), then the singles a (f12) and b (f13), then two
// output addresses.
//  * offset = point - worldCameraPos (0x0037ee80, three singles) and
//    length = sqrt(offset.x^2 + offset.y^2 + offset.z^2).
//  * level: 1.0 when length is not above 360.0; otherwise 1 - (length - a) *
//    (1 / (b - a)), raised to 0.0 when it falls below zero.
//  * side = (offset.x*m[0] + offset.y*m[3] + offset.z*m[6]) * (1 / b), where m
//    is worldCameraMatrix (0x0037ef00) read as singles at byte offsets 0, 12
//    and 24; side is limited to [-1, 1].
//  * *left = level * (1 - side) is stored through the first output pointer and
//    *right = level * (side + 1) through the second.
// The image also stores the three matrix singles to a stack slot that is never
// read; that is not reproduced. Unknown: the names of the singles a and b (the
// caller in the sound code passes 360.0 and 840.0 for the default case), and
// what the matrix entries mean.
struct Point3 { float x, y, z; };
extern "C" float worldCameraPos[3];
extern "C" float worldCameraMatrix[9];

extern "C" void pan__FG6Point3ffPfT3(Point3 point, float a, float b, float* left, float* right) {
    float dx = point.x - worldCameraPos[0];
    float dy = point.y - worldCameraPos[1];
    float dz = point.z - worldCameraPos[2];
    float length = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    float level;
    if (360.0f < length) {
        level = 1.0f - (length - a) * (1.0f / (b - a));
        if (level < 0.0f) level = 0.0f;
    } else {
        level = 1.0f;
    }
    float inverse = 1.0f / b;
    float side = (dx * worldCameraMatrix[0] + dy * worldCameraMatrix[3] + dz * worldCameraMatrix[6]) * inverse;
    if (side < -1.0f) side = -1.0f;
    if (1.0f < side) side = 1.0f;
    *left = level * (1.0f - side);
    *right = level * (side + 1.0f);
}

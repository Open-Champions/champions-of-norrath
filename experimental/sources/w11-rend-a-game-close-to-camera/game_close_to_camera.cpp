// Observed gameCloseToCamera__FP6Point3 at 0x001d0e08, 104 bytes. Compares the x and y
// of the point with worldViewCenter (0x0037ee90, floats; z unused). Returns 1 when
// x is within 480.0f of worldViewCenter.x and y is within 480.0f of worldViewCenter.y
// (both inclusive), else 0. The bound is the 32-bit constant 0x43f00000. Nothing here
// was executed on the EE.
struct Point3 {
    float x, y, z;
};
extern "C" {
extern float worldViewCenter[3];
int gameCloseToCamera__FP6Point3(Point3* p) {
    const float r = 480.0f;
    if (p->x <= worldViewCenter[0] + r && worldViewCenter[0] - r <= p->x &&
        p->y <= worldViewCenter[1] + r && worldViewCenter[1] - r <= p->y)
        return 1;
    return 0;
}
}

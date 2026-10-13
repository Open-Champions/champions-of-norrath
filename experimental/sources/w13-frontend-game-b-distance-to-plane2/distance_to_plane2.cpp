// Observed distanceToPlane2__FP6Point3P6Point2T0 at 0x0015a218, extent 48.
// (origin, direction, point) -> float. Only the first two floats of each vector
// are read (x at +0, y at +4); z of the Point3 arguments is ignored:
//   result = direction.x * (point.x - origin.x) + direction.y * (point.y - origin.y)
// The x term is added to the y term in that order. Point2 is two floats.
// Inferred: the Point2 is a plane normal in the horizontal plane and the result is
// the signed distance along it; the name and layout suggest this, no caller was
// checked. Unknown: whether the Point2 is required to be unit length.
struct Point3 {
    float x, y, z;
};
struct Point2 {
    float x, y;
};
extern "C" float distanceToPlane2__FP6Point3P6Point2T0(const Point3* origin, const Point2* direction, const Point3* point) {
    const float dy = point->y - origin->y;
    const float dx = point->x - origin->x;
    return direction->x * dx + direction->y * dy;
}

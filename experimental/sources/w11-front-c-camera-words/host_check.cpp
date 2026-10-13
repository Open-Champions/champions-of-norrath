// Host check for the camera words. Not an EE plan input.
struct Point3 {
    float x, y, z;
};
extern "C" {
extern short pitch1, pitch2;
extern Point3 v1, v2;
}
int main() {
    static_assert(sizeof(pitch1) == 2 && sizeof(Point3) == 12, "observed sizes");
    if (pitch1 || pitch2) return 1;
    return v1.x != 0 || v1.y != 0 || v1.z != 0 || v2.x != 0 || v2.y != 0 || v2.z != 0;
}

// Host check for shite. Not an EE plan input.
struct Point3 {
    float x, y, z;
};
extern "C" Point3 shite;
int main() { return shite.x != 0 || shite.y != 0 || shite.z != 0; }

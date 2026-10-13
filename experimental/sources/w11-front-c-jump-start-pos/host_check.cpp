// Host check for startPos. Not an EE plan input.
struct Point3 {
    float x, y, z;
};
extern "C" Point3 startPos;
int main() { return startPos.x != 0 || startPos.y != 0 || startPos.z != 0; }

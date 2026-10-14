// Host check for c.1089. Not an EE plan input.
#include "camera_static.cpp"

int main() {
    if (sizeof(norrath_core_loop_a_camera_static) != 384) return 1;
    if (sizeof(norrath_core_loop_a_camera_static) < 137 + 1) return 2;
    for (unsigned char b : norrath_core_loop_a_camera_static) if (b) return 3;
    return 0;
}

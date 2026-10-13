#include "collision_frame.cpp"
#include <cstring>
int main() {
    if (sizeof(collisionFrame) != 4) return 1;
    if (collisionFrame != 0) return 2;
    return 0;
}

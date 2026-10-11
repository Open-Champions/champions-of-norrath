#include "reverse_camera_yaw.cpp"
#include <cstdio>
static_assert(sizeof(gameReverseCameraYaw) == 4, "gameReverseCameraYaw is one word");
int main() {
    if (gameReverseCameraYaw != 0) return 1;
    gameReverseCameraYaw = 1;
    if (gameReverseCameraYaw != 1) return 2;
    std::puts("gameReverseCameraYaw contract passed");
    return 0;
}

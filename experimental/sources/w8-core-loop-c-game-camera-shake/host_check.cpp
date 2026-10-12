// Host contract for gameCameraShake. Not an EE plan input.
#include "game_camera_shake.cpp"

int main() {
    if (gameCameraShake != 0.0f) return 1;
    if (sizeof gameCameraShake != 4u) return 2;
    gameCameraShake = gameCameraShake + 1;
    if (gameCameraShake != 0.0f + 1) return 3;
    return 0;
}

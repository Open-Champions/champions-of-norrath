// Host contract for coreStartSwitchScene__Fi. Not an EE plan input.
#include "start_switch_scene.cpp"

int coreSwitchScene = 0;
int coreSwitchSceneStart = 0;
int drawFrameNm = 1234;

int main() {
    coreStartSwitchScene__Fi(7);
    if (coreSwitchScene != 7) return 1;
    if (coreSwitchSceneStart != 1234) return 2;
    drawFrameNm = -5;
    coreStartSwitchScene__Fi(-1);
    if (coreSwitchScene != -1 || coreSwitchSceneStart != -5) return 3;
    return 0;
}

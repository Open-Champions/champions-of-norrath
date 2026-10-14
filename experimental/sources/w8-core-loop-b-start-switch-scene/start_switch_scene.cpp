// Observed coreStartSwitchScene__Fi at 0x002d0db0, 16 bytes.
// Stores the argument in coreSwitchScene (0x00486684) and the current word of
// drawFrameNm (0x004844a8) in coreSwitchSceneStart (0x00486688). It returns nothing.
// Inferred: the second word records the frame number at which the switch was requested.
// Unknown: what scene numbers mean and who consumes the pair.
extern "C" int coreSwitchScene;
extern "C" int coreSwitchSceneStart;
extern "C" int drawFrameNm;

extern "C" void coreStartSwitchScene__Fi(int scene) {
    int frame = drawFrameNm;
    coreSwitchScene = scene;
    coreSwitchSceneStart = frame;
}

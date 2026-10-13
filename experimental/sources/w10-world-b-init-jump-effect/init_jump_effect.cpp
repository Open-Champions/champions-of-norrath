// Observed initJumpEffect__Fv at 0x00284838, 192 bytes in SLUS_205.65. Takes no argument and
// returns nothing. It:
//  1. copies the three floats of worldCameraPos (0x0037ee80, 12 bytes) to the vector startPos
//     (0x008d5fc0);
//  2. sets the float jumpGateTime (0x00486d98) to zero;
//  3. asks the main camera (the pointer gameCam) for its last target with
//     GetLastTarget__6Camera(result, camera), where the result is a local Point3;
//  4. stores endPos (0x008d5fd0) as {target.x + 0.0f, target.y, target.z + 90.0f}.
// The x add is observed as an add of a zero vector component (it turns a negative zero into a
// positive zero); y is copied without an add.
// Inferred: startPos and endPos are the two ends of the camera move used by the jump-gate
// effect (neighbouring names are jumpGate...), the start being the world camera position.
// Unknown: the units of 90 and whether worldCameraPos is the camera's eye or its focus.
struct Point3 { float x, y, z; };
extern "C" void* GetLastTarget__6Camera(void* result, void* camera);
extern "C" void* gameCam;
extern Point3 worldCameraPos;
extern Point3 startPos, endPos;
extern float jumpGateTime;

extern "C" void initJumpEffect__Fv() {
    startPos.x = worldCameraPos.x;
    startPos.y = worldCameraPos.y;
    startPos.z = worldCameraPos.z;
    jumpGateTime = 0.0f;
    Point3 target;
    GetLastTarget__6Camera(&target, gameCam);
    const Point3 offset = {0.0f, 0.0f, 90.0f};
    endPos.x = target.x + offset.x;
    endPos.y = target.y;
    endPos.z = target.z + offset.z;
}

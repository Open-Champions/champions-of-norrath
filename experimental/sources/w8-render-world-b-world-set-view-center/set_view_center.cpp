// Observed worldSetViewCenter__Fv at 0x0017b740, 160 bytes.
// worldCameraMatrix (0x0037ef00, 48 bytes) is read as twelve floats. Elements
// 2, 5 and 8 (byte offsets 8, 20 and 32) are each multiplied by 600.0f and
// added to the matching component of worldCameraPos (0x0037ee80, three
// floats). The three sums are written to worldViewCenter (0x0037ee90, three
// floats). Nothing is returned. The original also builds two temporary
// vectors on the stack; they are not observable after return.
// worldCameraMatrix and worldCameraPos are defined elsewhere; worldViewCenter
// is defined by another source of this package.
extern "C" {
extern float worldCameraMatrix[12];
extern float worldCameraPos[3];
extern float worldViewCenter[3];
void worldSetViewCenter__Fv() {
    const float scale = 600.0f;
    const float x = worldCameraMatrix[2] * scale;
    const float y = worldCameraMatrix[5] * scale;
    const float z = worldCameraMatrix[8] * scale;
    worldViewCenter[0] = worldCameraPos[0] + x;
    worldViewCenter[1] = worldCameraPos[1] + y;
    worldViewCenter[2] = worldCameraPos[2] + z;
}
}

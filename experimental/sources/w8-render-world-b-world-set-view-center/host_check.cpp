extern "C" {
float worldCameraMatrix[12];
float worldCameraPos[3];
float worldViewCenter[3];
void worldSetViewCenter__Fv();
}
int main() {
    for (int i = 0; i < 12; ++i) worldCameraMatrix[i] = float(100 + i);
    worldCameraMatrix[2] = 0.5f; worldCameraMatrix[5] = -1.0f; worldCameraMatrix[8] = 0.25f;
    worldCameraPos[0] = 10.0f; worldCameraPos[1] = 20.0f; worldCameraPos[2] = 30.0f;
    worldViewCenter[0] = worldViewCenter[1] = worldViewCenter[2] = -1.0f;
    worldSetViewCenter__Fv();
    if (worldViewCenter[0] != 310.0f) return 1;
    if (worldViewCenter[1] != -580.0f) return 2;
    if (worldViewCenter[2] != 180.0f) return 3;
    // Inputs are not modified and the other matrix elements are unused.
    if (worldCameraPos[0] != 10.0f || worldCameraMatrix[0] != 100.0f || worldCameraMatrix[2] != 0.5f) return 4;
    return 0;
}

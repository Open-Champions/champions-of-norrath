struct Point3 { float x, y, z; };
extern "C" {
float worldViewCenter[3];
int gameCloseToCamera__FP6Point3(Point3* p);
}
int main() {
    worldViewCenter[0] = 100.0f; worldViewCenter[1] = -200.0f; worldViewCenter[2] = 5.0f;
    Point3 p = {100.0f, -200.0f, 1e9f};
    if (gameCloseToCamera__FP6Point3(&p) != 1) return 1;
    p = {580.0f, -200.0f, 0}; if (gameCloseToCamera__FP6Point3(&p) != 1) return 2;   // x edge inclusive
    p = {-380.0f, -680.0f, 0}; if (gameCloseToCamera__FP6Point3(&p) != 1) return 3;  // low edges inclusive
    p = {580.01f, -200.0f, 0}; if (gameCloseToCamera__FP6Point3(&p) != 0) return 4;
    p = {-380.5f, -200.0f, 0}; if (gameCloseToCamera__FP6Point3(&p) != 0) return 5;
    p = {100.0f, 281.0f, 0}; if (gameCloseToCamera__FP6Point3(&p) != 0) return 6;
    p = {100.0f, -681.0f, 0}; if (gameCloseToCamera__FP6Point3(&p) != 0) return 7;
    return 0;
}

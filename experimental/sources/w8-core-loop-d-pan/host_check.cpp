// Host contract for pan__FG6Point3ffPfT3. Not an EE plan input.
#include <cmath>
#include "pan.cpp"
extern "C" {
float worldCameraPos[3];
float worldCameraMatrix[9];
}
static bool near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main() {
    float l = -9, r = -9;
    for (float& f : worldCameraMatrix) f = 0;
    worldCameraMatrix[0] = 1;                       // side follows x
    pan__FG6Point3ffPfT3({0, 0, 0}, 360, 840, &l, &r);
    if (!near(l, 1) || !near(r, 1)) return 1;       // at the camera: full level, centered
    pan__FG6Point3ffPfT3({420, 0, 0}, 360, 840, &l, &r);   // beyond 360: attenuated
    if (!near(l, 0.4375f) || !near(r, 1.3125f)) return 2;
    pan__FG6Point3ffPfT3({300, 0, 0}, 360, 840, &l, &r);   // within 360: level stays 1
    if (!near(l, 1 - 300.f / 840) || !near(r, 1 + 300.f / 840)) return 3;
    pan__FG6Point3ffPfT3({-1000, 0, 0}, 360, 840, &l, &r); // side clamped to -1, level clamped to 0
    if (!near(l, 0) || !near(r, 0)) return 4;
    worldCameraPos[0] = 500;                        // offsets are taken from the camera
    pan__FG6Point3ffPfT3({500, 0, 0}, 360, 840, &l, &r);
    if (!near(l, 1) || !near(r, 1)) return 5;
    worldCameraPos[0] = 0; worldCameraMatrix[0] = 4; // side clamped at +1
    pan__FG6Point3ffPfT3({350, 0, 0}, 360, 840, &l, &r);
    if (!near(l, 0) || !near(r, 2)) return 6;
    worldCameraMatrix[0] = 0; worldCameraMatrix[3] = 1; worldCameraMatrix[6] = 1;
    pan__FG6Point3ffPfT3({0, 84, 84}, 360, 840, &l, &r);   // y and z feed the side too
    if (!near(l, 0.8f) || !near(r, 1.2f)) return 7;
    return 0;
}

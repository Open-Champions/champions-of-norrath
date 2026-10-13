#include "call.cpp"

static int order[4];
static int count;
static void* args[2][8];
static int update_result;
static VIVect3Local* axes_seen[3];
static float axes_at_calc[3][3];

extern "C" int CalcAxes__8VICameraRC7VIVect3R7VIVect3N22(void* camera, void* point, VIVect3Local* x, VIVect3Local* y, VIVect3Local* z) {
    order[count++] = 1;
    args[0][0] = camera; args[0][1] = point;
    axes_seen[0] = x; axes_seen[1] = y; axes_seen[2] = z;
    VIVect3Local* v[3] = {x, y, z};
    for (int i = 0; i < 3; ++i) {
        axes_at_calc[i][0] = v[i]->x; axes_at_calc[i][1] = v[i]->y; axes_at_calc[i][2] = v[i]->z;
    }
    // Calc writes something so Update would see it; the contract only checks identity.
    x->x = 1.0f;
    return 0;
}
extern "C" int Update__8VICameraRC7VIVect3R7VIVect3N22R10VIMatrix44(void* camera, void* point, VIVect3Local* x, VIVect3Local* y, VIVect3Local* z, void* matrix) {
    order[count++] = 2;
    args[1][0] = camera; args[1][1] = point;
    args[1][2] = x; args[1][3] = y; args[1][4] = z; args[1][5] = matrix;
    return update_result;
}

int main() {
    unsigned char camera[256];
    int a, b;
    update_result = 0;
    int result = __cl__8VICameraR7VIVect3T1(camera, &a, &b);
    if (result != 0 || count != 2 || order[0] != 1 || order[1] != 2) return 1;
    if (args[0][0] != camera || args[0][1] != &b) return 2;           // axes use the second vector
    if (args[1][0] != camera || args[1][1] != &a) return 3;           // update uses the first vector
    if (args[1][2] != axes_seen[0] || args[1][3] != axes_seen[1] || args[1][4] != axes_seen[2]) return 4;
    if (args[1][5] != camera + 112) return 5;
    for (auto& axis : axes_at_calc)
        for (float f : axis) if (f != 0.0f) return 6;                  // zeroed before the first call
    update_result = 5;
    if (__cl__8VICameraR7VIVect3T1(camera, &a, &b) != 0) return 7;
    update_result = -1;
    if (__cl__8VICameraR7VIVect3T1(camera, &a, &b) != -1) return 8;
    update_result = -9;
    if (__cl__8VICameraR7VIVect3T1(camera, &a, &b) != -1) return 9;
    return 0;
}

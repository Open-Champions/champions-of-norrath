// Host contract for modelGetBoundingBox__FP12_modelHeaderlPiN52. Not an EE plan input.
#include <cstdio>
#include <cstring>
struct _modelHeader;
extern "C" void modelGetBoundingBox__FP12_modelHeaderlPiN52(_modelHeader*, long, int*, int*, int*, int*, int*, int*);
int main() {
    alignas(16) static signed char h[256];
    memset(h, 0, sizeof h);
    h[18] = 3; h[38] = 1;                       // boxes at 40 + 1 * 16 = 56
    signed char* b = h + 56;
    signed char b0[6] = {-5, 2, 3, 4, 5, 6}, b1[6] = {-9, 7, 1, 2, 9, 8}, b2[6] = {0, -3, -4, 20, 1, -30};
    memcpy(b, b0, 6); memcpy(b + 10, b1, 6); memcpy(b + 20, b2, 6);
    int o[6];
    modelGetBoundingBox__FP12_modelHeaderlPiN52((_modelHeader*)h, 5, o, o + 1, o + 2, o + 3, o + 4, o + 5);   // boxes 0 and 2
    int want[6] = {-5, -3, -4, 20, 5, 6};
    for (int i = 0; i < 6; i++) if (o[i] != want[i]) return 1 + i;
    modelGetBoundingBox__FP12_modelHeaderlPiN52((_modelHeader*)h, 0, o, o + 1, o + 2, o + 3, o + 4, o + 5);
    if (o[0] != 100000 || o[3] != -100000 || o[5] != -100000) return 10;
    modelGetBoundingBox__FP12_modelHeaderlPiN52((_modelHeader*)h, 2, o, o + 1, o + 2, o + 3, o + 4, o + 5);   // box 1 only
    if (o[0] != -9 || o[4] != 9 || o[2] != 1) return 11;
    h[18] = 0;
    modelGetBoundingBox__FP12_modelHeaderlPiN52((_modelHeader*)h, -1, o, o + 1, o + 2, o + 3, o + 4, o + 5);
    if (o[1] != 100000 || o[4] != -100000) return 12;
    std::printf("modelGetBoundingBox contract passed\n");
    return 0;
}

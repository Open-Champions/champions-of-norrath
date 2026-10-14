// Observed: Identity__10VIMatrix44 at 0x010d0818 is 44 bytes and returns nothing.
// It uses VU0 macro-mode vector ops to form (0,0,0,1) and rotates the lanes with
// vmr32 to get (0,0,1,0), (0,1,0,0) and (1,0,0,0), then stores the four 16-byte
// vectors at offsets 48, 32, 16 and 0 of the object, so the 64 bytes become the
// 4x4 identity in row-major order.
// Inferred: authored as sixteen float stores; the original's quadword stores
// and COP2 use are not reproduced. Unknown: any alignment requirement of the caller.
extern "C" {
void Identity__10VIMatrix44(void* matrix) {
    float* m = static_cast<float*>(matrix);
    m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f; m[15] = 1.0f;
    m[8] = 0.0f; m[9] = 0.0f; m[10] = 1.0f; m[11] = 0.0f;
    m[4] = 0.0f; m[5] = 1.0f; m[6] = 0.0f; m[7] = 0.0f;
    m[0] = 1.0f; m[1] = 0.0f; m[2] = 0.0f; m[3] = 0.0f;
}
}

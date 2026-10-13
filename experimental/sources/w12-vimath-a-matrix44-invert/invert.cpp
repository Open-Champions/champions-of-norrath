// Observed: Invert__10VIMatrix44RC10VIMatrix44 at 0x010d0558 is 584 bytes. Arguments: the
// destination (a0) and a source matrix (a1), both 16 floats as four 16-byte rows. It makes
// a zero-filled local matrix, copies the source into it, and writes the identity matrix
// (1.0f on the diagonal, 0 elsewhere) into the destination. It then runs Gauss-Jordan
// elimination with partial pivoting, one column k = 0..3 at a time, on the pair
// (local, destination):
//   1. pivot search: start with the local element [k][k] and its absolute value; for rows
//      k+1..3 take the element [row][k]; when the best absolute value is strictly less
//      than the candidate's, the candidate becomes the pivot (ties keep the earlier row);
//   2. if the pivot value compares equal to 0.0f the local matrix is destroyed with
//      _$_10VIMatrix44(local, 2) and the function returns -1; the destination is left
//      holding whatever the elimination had produced so far;
//   3. SwapRows__10VIMatrix44ii(local, k, pivot row) and the same for the destination
//      (both calls are made even when the pivot row is k);
//   4. reciprocal = 1.0f / pivot value; all four floats of row k of the local matrix and
//      then of the destination are multiplied by it;
//   5. for each row r from 0 to 3 except k: f = -local[r][k] (read before the row is
//      changed); local row r += local row k * f (per lane, four floats, product rounded
//      separately from the sum), then destination row r += destination row k * f.
// After column 3 it destroys the local matrix with _$_10VIMatrix44(local, 2) and
// returns 0.
// Inferred: the destination receives the inverse of the source when the return value is
// 0; singularity is detected only as an exact zero pivot. The original performed the row
// scale and add with VU0 macro-mode (vmulx, vadd), the authored version uses ordinary
// float arithmetic, with an empty asm statement so that products are not fused into a
// multiply-add; hardware rounding equality with the VU0 is not claimed.
// Unknown: the member names; the local's constructor is inlined as sixteen zero stores.
#if defined(__mips__)
#define NORRATH_KEEP_PRODUCT(value) __asm__("" : "+f"(value))
#else
#define NORRATH_KEEP_PRODUCT(value) ((void)0)
#endif

extern "C" {
int SwapRows__10VIMatrix44ii(float* matrix, int a, int b);
void _$_10VIMatrix44(void* matrix, int flags);

int Invert__10VIMatrix44RC10VIMatrix44(float* dest, const float* source) {
    alignas(16) float local[16];
    for (int i = 0; i < 16; ++i) local[i] = 0.0f;
    for (int i = 0; i < 16; ++i) local[i] = source[i];
    dest[12] = 0.0f; dest[13] = 0.0f; dest[14] = 0.0f; dest[15] = 1.0f;
    dest[8] = 0.0f; dest[9] = 0.0f; dest[10] = 1.0f; dest[11] = 0.0f;
    dest[4] = 0.0f; dest[5] = 1.0f; dest[6] = 0.0f; dest[7] = 0.0f;
    dest[0] = 1.0f; dest[1] = 0.0f; dest[2] = 0.0f; dest[3] = 0.0f;
    for (int k = 0; k < 4; ++k) {
        int pivot_row = k;
        float pivot = local[4 * k + k];
        float best = __builtin_fabsf(pivot);
        for (int row = k + 1; row < 4; ++row) {
            float candidate = local[4 * row + k];
            float magnitude = __builtin_fabsf(candidate);
            if (best < magnitude) {
                best = magnitude;
                pivot = candidate;
                pivot_row = row;
            }
        }
        if (pivot == 0.0f) {
            _$_10VIMatrix44(local, 2);
            return -1;
        }
        SwapRows__10VIMatrix44ii(local, k, pivot_row);
        SwapRows__10VIMatrix44ii(dest, k, pivot_row);
        float reciprocal = 1.0f / pivot;
        for (int i = 0; i < 4; ++i) local[4 * k + i] = local[4 * k + i] * reciprocal;
        for (int i = 0; i < 4; ++i) dest[4 * k + i] = dest[4 * k + i] * reciprocal;
        for (int row = 0; row < 4; ++row) {
            if (row == k) continue;
            float factor = -local[4 * row + k];
            for (int i = 0; i < 4; ++i) {
                float product = local[4 * k + i] * factor;
                NORRATH_KEEP_PRODUCT(product);
                local[4 * row + i] = local[4 * row + i] + product;
            }
            for (int i = 0; i < 4; ++i) {
                float product = dest[4 * k + i] * factor;
                NORRATH_KEEP_PRODUCT(product);
                dest[4 * row + i] = dest[4 * row + i] + product;
            }
        }
    }
    _$_10VIMatrix44(local, 2);
    return 0;
}
}

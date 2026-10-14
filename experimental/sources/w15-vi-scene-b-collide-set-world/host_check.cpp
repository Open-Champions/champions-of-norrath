// Host check only; not a build input. The two callees are replaced by recorders.
#include "set_world.cpp"
static unsigned char *copy_dst, *inv_dst; static const unsigned char *copy_src, *inv_src; static int order, copy_at, inv_at;
extern "C" void sceVu0CopyMatrix(unsigned char* d, const unsigned char* s) { copy_dst = d; copy_src = s; copy_at = ++order; }
extern "C" void InvertAffine__10VIMatrix44RC10VIMatrix44(unsigned char* d, const unsigned char* s) { inv_dst = d; inv_src = s; inv_at = ++order; }
int main() {
    unsigned char record[160], matrix[64];
    if (SetWorld__9VICollideRC10VIMatrix44(record, matrix) != 0) return 1;
    if (copy_dst != record + 16 || copy_src != matrix) return 2;
    if (inv_dst != record + 80 || inv_src != record + 16) return 3;
    return copy_at == 1 && inv_at == 2 ? 0 : 4;
}

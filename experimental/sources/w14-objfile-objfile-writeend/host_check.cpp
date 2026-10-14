// Host check only; not a build input.
#include "writeend.cpp"
static void* seen_object; static unsigned* seen_out; static int result;
extern "C" int WriteEnd__9VIObjFileRUi(void* o, unsigned* out) { seen_object = o; seen_out = out; *out = 0x1234; return result; }
int main() {
    int x;
    const int rs[] = {0, -1, 7, -9};
    for (int r : rs) {
        result = r; seen_object = 0; seen_out = 0;
        if (WriteEnd__9VIObjFile(&x) != r || seen_object != &x || seen_out == 0) return 1;
    }
    return 0;
}

struct Call { int a, b, c, d, e; unsigned f; int g, h, i; int n; } call;
extern "C" void textureSmoothBorder__FiiiiiUiiii(int a, int b, int c, int d, int e, unsigned f, int g, int h, int i) {
    call = {a, b, c, d, e, f, g, h, i, call.n + 1};
}
extern "C" void textureDrawBorder__FiiiiiUiPcii(int, int, int, int, int, unsigned, char*, int, int);

int main() {
    char x;
    textureDrawBorder__FiiiiiUiPcii(10, 20, 30, 40, 50, 0x80402010u, &x, 70, 80);
    if (call.n != 1) return 1;
    if (call.a != 9 || call.b != 16 || call.c != 32 || call.d != 48) return 2;
    if (call.e != 50 || call.f != 0x80402010u || call.g != 2 || call.h != 70 || call.i != 80) return 3;
    textureDrawBorder__FiiiiiUiPcii(-5, 0, -2, -8, -1, 7u, 0, 0, -1);
    if (call.a != -6 || call.b != -4 || call.c != 0 || call.d != 0 || call.i != -1 || call.n != 2) return 4;
    return 0;
}

static int calls;
static int a[5];
static unsigned last;

extern "C" {
int fadeDownVal;
void rectangleDraw__FiiiiiUi(int p0, int p1, int p2, int p3, int p4, unsigned p5) {
    calls += 1;
    a[0] = p0; a[1] = p1; a[2] = p2; a[3] = p3; a[4] = p4;
    last = p5;
}
int hudFadeScreen__Fi(int delta);
}

static int step(int start, int delta, int want) {
    fadeDownVal = start;
    calls = 0;
    int r = hudFadeScreen__Fi(delta);
    if (r != want || fadeDownVal != want) return 1;
    if (calls != 1) return 1;
    if (a[0] != 0 || a[1] != 0 || a[2] != 640 || a[3] != 448 || a[4] != 6) return 1;
    return last != static_cast<unsigned>(want) << 24;
}

int main() {
    if (step(0, 10, 10)) return 1;
    if (step(120, 8, 128)) return 2;
    if (step(120, 9, 128)) return 3;
    if (step(120, 1000, 128)) return 4;
    if (step(5, -5, 0)) return 5;
    if (step(5, -6, 0)) return 6;
    if (step(128, -128, 0)) return 7;
    if (step(50, 0, 50)) return 8;
    return 0;
}

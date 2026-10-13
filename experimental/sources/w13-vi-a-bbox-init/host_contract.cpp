#include "init.cpp"

static int calls, firsts[8];
static const void* seen_box;
static const void* seen_pt[8];
extern "C" void Bound__6VIBBoxiRC7VIVect3(void* box, int first, const void* point) {
    seen_box = box;
    if (calls < 8) { firsts[calls] = first; seen_pt[calls] = point; }
    ++calls;
}

int main() {
    unsigned char pts[48];
    int box;
    if (Init__6VIBBoxiP7VIVect3(&box, 0, pts) != 0 || calls != 0) return 1;
    if (Init__6VIBBoxiP7VIVect3(&box, -3, pts) != 0 || calls != 0) return 2;
    if (Init__6VIBBoxiP7VIVect3(&box, 3, pts) != 0 || calls != 3) return 3;
    if (seen_box != &box) return 4;
    if (firsts[0] != 1 || firsts[1] != 0 || firsts[2] != 0) return 5;
    for (int i = 0; i < 3; ++i) if (seen_pt[i] != pts + 12 * i) return 6;
    return 0;
}

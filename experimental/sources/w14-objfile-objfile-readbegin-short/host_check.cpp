// Host check only; not a build input.
#include "readbegin.cpp"
static void* o_; static unsigned short *a_, *b_; static unsigned long* c_; static unsigned* d_; static int result;
extern "C" int ReadBegin__9VIObjFileRUsT1RUlRUi(void* o, unsigned short* a, unsigned short* b, unsigned long* c, unsigned* d) {
    o_ = o; a_ = a; b_ = b; c_ = c; d_ = d; *c = 1; *d = 2; return result;
}
int main() {
    int x; unsigned short a = 1, b = 2;
    const int rs[] = {0, -1, 5};
    for (int r : rs) {
        result = r; o_ = 0;
        if (ReadBegin__9VIObjFileRUsT1(&x, &a, &b) != r) return 1;
        if (o_ != &x || a_ != &a || b_ != &b || c_ == 0 || d_ == 0) return 2;
    }
    return 0;
}

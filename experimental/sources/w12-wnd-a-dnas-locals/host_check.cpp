// Host check only; not a build input. Checks the initial value recorded from the image and that the
// object is writable with the documented width.
#include "ellipse_count.cpp"
#include <cstdio>
int main() {
    bool ok = norrath_dnas_ellipse_count == 0 && sizeof norrath_dnas_ellipse_count == 4;
    norrath_dnas_ellipse_count = 121;
    ok = ok && norrath_dnas_ellipse_count == 121;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}

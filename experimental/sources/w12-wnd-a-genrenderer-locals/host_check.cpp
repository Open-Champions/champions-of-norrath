// Host check only; not a build input. Checks the initial value recorded from the image and that the
// object is writable with the documented width.
#include "render_count.cpp"
#include <cstdio>
int main() {
    bool ok = norrath_generic_renderer_render_count == 0 && sizeof norrath_generic_renderer_render_count == 4;
    norrath_generic_renderer_render_count = 1000;
    ok = ok && norrath_generic_renderer_render_count == 1000;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}

// Host check only; not a build input.
#include "get_model_view.cpp"
#include <cstdio>
int main() {
    unsigned char a[4] = {9, 9, 9, 9};
    bool ok = GetModelView__C8VIRaster(a) == 0x70000000u && GetModelView__C8VIRaster(0) == 0x70000000u && a[0] == 9;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}

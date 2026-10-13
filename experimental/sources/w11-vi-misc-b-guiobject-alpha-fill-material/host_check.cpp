// Host check only; not a build input. The word starts at -1, is four bytes wide, and has the
// signed 32-bit type that FillRectangle compares with -1.
#include "alpha_fill_material.cpp"
#include <cstdio>

static_assert(sizeof(_11VIGUIObject$m_alphaFillMaterial) == 4, "four bytes");

int main() {
    bool ok = _11VIGUIObject$m_alphaFillMaterial == -1 &&
              static_cast<unsigned>(_11VIGUIObject$m_alphaFillMaterial) == 0xffffffffu;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}

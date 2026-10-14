// Host contract for the font constructor entry. Not an EE plan input.
#include "font_static_init.cpp"
extern "C" { unsigned p[4]; unsigned glistp; }
int main() {
    p[0] = 1; p[1] = 2; p[2] = 3; p[3] = 4; glistp = 0x00a1b2c0u;
    _GLOBAL_$I$_fontStringSize__FP4FontPci();
    if (p[0] != 0x00a1b2c0u || p[1] != 2 || p[2] != 3 || p[3] != 4) return 1;
    glistp = 0x40;
    _GLOBAL_$I$_fontStringSize__FP4FontPci();
    if (p[0] != 0x40) return 2;
    // The local routine is inert for any other argument pair.
    glistp = 0x99;
    staticInitFont(0, 0xffff); staticInitFont(1, 0xfffe); staticInitFont(2, 0xffff);
    if (p[0] != 0x40) return 3;
    return 0;
}

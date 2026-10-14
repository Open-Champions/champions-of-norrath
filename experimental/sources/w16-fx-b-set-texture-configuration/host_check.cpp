// Host check only; not a build input.
#include "set_texture_configuration.cpp"
#include <cstring>
int main() {
    alignas(8) char obj[800]; std::memset(obj, 0xab, sizeof obj);
    SetTextureConfiguration__20VIParticleDefinitioni(obj, 7);
    if (*(int*)(obj + 760) != 7) return 1;
    if ((unsigned char)obj[759] != 0xab || (unsigned char)obj[764] != 0xab) return 2;
    return 0;
}

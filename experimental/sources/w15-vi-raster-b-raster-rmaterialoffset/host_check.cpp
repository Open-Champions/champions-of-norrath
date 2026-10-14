// Host check only; not a build input.
#include "r_material_offset.cpp"

int main() {
    return norrath_r_material_offset == 0 ? 0 : 1;
}

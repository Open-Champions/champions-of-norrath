#include "name_defers.cpp"

int main() {
    if (sizeof(norrath_name_sprite_num_defers) != 4) return 1;
    if (sizeof(norrath_name_sprite_defers) != 0xd80) return 2;
    if (norrath_name_sprite_num_defers != 0) return 3;
    for (unsigned i = 0; i < sizeof(norrath_name_sprite_defers); ++i)
        if (norrath_name_sprite_defers[i] != 0) return 4;
    return 0;
}

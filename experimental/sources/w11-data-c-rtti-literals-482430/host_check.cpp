// Host contract for the three 0x3e4ccccc views. Not an EE plan input.
#include <cstring>
extern "C" const unsigned norrath_rtti_entities_a_view_482430;
extern "C" const unsigned norrath_rtti_entities_a_view_482434;
extern "C" const unsigned norrath_rtti_entities_a_view_482438;
int main() {
    if (norrath_rtti_entities_a_view_482430 != 0x3e4ccccc) return 1;
    if (norrath_rtti_entities_a_view_482434 != 0x3e4ccccc) return 2;
    if (norrath_rtti_entities_a_view_482438 != 0x3e4ccccc) return 3;
    float f;
    std::memcpy(&f, &norrath_rtti_entities_a_view_482430, 4);
    if (!(f > 0.19f && f < 0.2f)) return 4;
    return 0;
}

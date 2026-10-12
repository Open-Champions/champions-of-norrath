#include "type_name.cpp"
#include <cstdio>
#include <cstring>
int main() {
    if (sizeof norrath_spell_blessed_type_name_view != 15u) { std::fprintf(stderr, "fail: size\n"); return 1; }
    if (std::strcmp(reinterpret_cast<const char*>(norrath_spell_blessed_type_name_view), "12SpellBlessed") != 0) { std::fprintf(stderr, "fail: text\n"); return 1; }
    return 0;
}

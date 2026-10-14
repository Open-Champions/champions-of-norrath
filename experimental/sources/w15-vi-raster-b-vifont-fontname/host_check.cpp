// Host check only; not a build input.
#include "font_name.cpp"

int main() {
    static const char want[8] = {'A', 'r', 'i', 'a', 'l', 'U', 'I', 0};
    for (int i = 0; i < 8; ++i) {
        if (norrath_vifont_sm_font_name[i] != want[i]) return 1;
    }
    return sizeof(norrath_vifont_sm_font_name) == 8 ? 0 : 2;
}

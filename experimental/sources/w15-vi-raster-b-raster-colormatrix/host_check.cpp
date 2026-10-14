// Host check only; not a build input.
#include "color_matrix.cpp"

int main() {
    return norrath_color_matrix == 0x0f ? 0 : 1;
}

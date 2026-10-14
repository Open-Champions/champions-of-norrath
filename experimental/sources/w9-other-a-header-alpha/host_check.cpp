#include "header_alpha.cpp"
int main() {
    if (sizeof(headerAlpha) != 4) return 1;
    if (headerAlpha != 0) return 2;
    headerAlpha = 1;
    if (headerAlpha != 1) return 3;
    return 0;
}

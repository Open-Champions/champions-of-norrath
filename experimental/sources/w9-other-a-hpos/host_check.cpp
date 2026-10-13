#include "hpos.cpp"
int main() {
    if (sizeof(hpos) != 144) return 1;
    for (auto& p : hpos) if (p.x != 0 || p.y != 0 || p.z != 0) return 2;
    return 0;
}

#include "active_list.cpp"
#include <cstring>
int main() {
    if (sizeof norrath_trail_active_list != 16) return 1;
    for (unsigned w : norrath_trail_active_list) if (w) return 2;
    return reinterpret_cast<unsigned long>(norrath_trail_active_list) % 8 ? 3 : 0;
}

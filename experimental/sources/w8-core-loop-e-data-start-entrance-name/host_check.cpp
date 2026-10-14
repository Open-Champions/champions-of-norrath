#include "start_entrance_name.cpp"
int main() {
    static_assert(sizeof(startEntranceName) == 32, "32 bytes");
    for (char c : startEntranceName) if (c != 0) return 1;
    return 0;
}

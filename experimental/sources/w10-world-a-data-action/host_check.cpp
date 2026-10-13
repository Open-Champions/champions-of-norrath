#include "action.cpp"
#include <cstring>
int main() {
    if (sizeof(action) != sizeof(void*)) return 1;
    if (action != 0) return 2;
    return 0;
}

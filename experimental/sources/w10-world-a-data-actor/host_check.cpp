#include "actor.cpp"
#include <cstring>
int main() {
    if (sizeof(actor) != sizeof(void*)) return 1;
    if (actor != 0) return 2;
    return 0;
}

#include "async_rand_state.cpp"
#include <cstring>
int main() {
    if (sizeof(asyncRandState) != 4) return 1;
    if (asyncRandState != 0) return 2;
    return 0;
}

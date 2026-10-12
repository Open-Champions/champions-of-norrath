// Host contract for engineScissory2. Not an EE plan input.
#include "engine_scissory2.cpp"

int main() {
    if (engineScissory2 != 447) return 1;
    if (sizeof engineScissory2 != 4u) return 2;
    engineScissory2 = engineScissory2 + 1;
    if (engineScissory2 != 447 + 1) return 3;
    return 0;
}

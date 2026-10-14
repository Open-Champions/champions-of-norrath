// Host contract for engineScissory1. Not an EE plan input.
#include "engine_scissory1.cpp"

int main() {
    if (engineScissory1 != 0) return 1;
    if (sizeof engineScissory1 != 4u) return 2;
    engineScissory1 = engineScissory1 + 1;
    if (engineScissory1 != 0 + 1) return 3;
    return 0;
}

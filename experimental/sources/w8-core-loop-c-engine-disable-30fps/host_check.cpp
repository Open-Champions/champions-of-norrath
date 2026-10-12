// Host contract for engineDisable30fps. Not an EE plan input.
#include "engine_disable_30fps.cpp"

int main() {
    if (engineDisable30fps != 0) return 1;
    if (sizeof engineDisable30fps != 4u) return 2;
    engineDisable30fps = engineDisable30fps + 1;
    if (engineDisable30fps != 0 + 1) return 3;
    return 0;
}

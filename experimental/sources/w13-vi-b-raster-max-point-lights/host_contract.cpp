#include "max_point_lights.cpp"

int main() {
    char object[16] = {0};
    if (MaxPointLights__8VIRaster(object) != 1) return 1;
    if (MaxPointLights__8VIRaster(nullptr) != 1) return 2;
    for (char c : object) if (c) return 3;
    return 0;
}

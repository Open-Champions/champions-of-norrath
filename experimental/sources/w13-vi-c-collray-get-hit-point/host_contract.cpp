// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "collray-get-hit-point.cpp"

int main() {
    float ray[24] = {};
    ray[0] = 1.0f; ray[1] = 2.0f; ray[2] = 3.0f;
    ray[6] = 10.0f; ray[7] = 20.0f; ray[8] = 30.0f;
    ray[17] = 0.5f;
    float out[4] = {-1, -1, -1, -1};
    if (GetHitPoint__9VICollRay(out, ray) != out) return 1;
    if (out[0] != 6.0f || out[1] != 12.0f || out[2] != 18.0f || out[3] != -1.0f) return 2;
    ray[17] = 0.0f;
    GetHitPoint__9VICollRay(out, ray);
    if (out[0] != 1.0f || out[1] != 2.0f || out[2] != 3.0f) return 3;
    return 0;
}

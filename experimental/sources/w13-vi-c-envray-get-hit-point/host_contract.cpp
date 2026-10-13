// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "envray-get-hit-point.cpp"

int main() {
    float ray[16] = {};
    ray[0] = 1.0f; ray[1] = 2.0f; ray[2] = 3.0f;
    ray[3] = 5.0f; ray[4] = 10.0f; ray[5] = 3.0f;
    ray[14] = 0.5f;
    float out[4] = {-1, -1, -1, -1};
    if (GetHitPoint__C8VIEnvRay(out, ray) != out) return 1;
    if (out[0] != 3.0f || out[1] != 6.0f || out[2] != 3.0f || out[3] != -1.0f) return 2;
    ray[14] = 1.0f;
    GetHitPoint__C8VIEnvRay(out, ray);
    if (out[0] != 5.0f || out[1] != 10.0f) return 3;
    return 0;
}

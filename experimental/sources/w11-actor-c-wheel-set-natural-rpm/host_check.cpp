// Host contract for SetNaturalRPM__5Wheel; not an EE input. The vehicle pointer word is an offset from
// a host base. Expected values are computed independently with the same operation order.
#define W11_ACTOR_C_HOST 1
#include "set_natural_rpm.cpp"
#include <cstring>
#include <initializer_list>
unsigned char* w11_actor_c_host_base;
int main() {
    static unsigned char base[8192];
    w11_actor_c_host_base = base;
    int failures = 0;
    for (int i : {0, 1, 3})
        for (float gate : {0.0f, -0.0f, 2.0f, -1.0f})
            for (float den : {1.0f, 4.0f, 0.5f}) {
                std::memset(base, 0x3c, sizeof base);
                unsigned char* wheel = base + 100;
                unsigned char* vehicle = base + 2048;
                *reinterpret_cast<int*>(wheel + 384) = i;
                *reinterpret_cast<unsigned*>(wheel + 396) = 2048;
                *reinterpret_cast<float*>(wheel + 380) = den;
                *reinterpret_cast<float*>(wheel + 364) = 7.0f;
                *reinterpret_cast<float*>(wheel + 360) = 9.0f;
                float B[3] = {1.5f, -2.25f, 3.0f}, A[3] = {0.25f, 4.0f, -0.5f};
                std::memcpy(vehicle + 676 + 4 * i, &gate, 4);
                std::memcpy(vehicle + 1644 + 12 * i, B, 12);
                std::memcpy(vehicle + 1836 + 12 * i, A, 12);
                unsigned char ref[8192];
                std::memcpy(ref, base, sizeof ref);
                SetNaturalRPM__5Wheel(wheel);
                if (gate != 0.0f) {
                    float d = B[0] * A[0];
                    d = d + B[1] * A[1];
                    d = d + B[2] * A[2];
                    volatile float r = d / den;
                    float rr = r * 60.0f;
                    std::memcpy(ref + 100 + 364, &rr, 4);
                    std::memcpy(ref + 100 + 360, &rr, 4);
                }
                if (std::memcmp(ref, base, sizeof ref) != 0) ++failures;
            }
    return failures != 0;
}

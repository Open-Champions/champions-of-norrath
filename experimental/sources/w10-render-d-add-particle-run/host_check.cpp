// Host check for AddParticle::run: neither the object nor the point is modified.
#include <cstdio>
#include <cstring>
extern "C" void run__11AddParticleG6Point3(void* self, const float* point);
int main() {
    unsigned char object[320];
    memset(object, 0x5c, sizeof object);
    unsigned char copy[320];
    memcpy(copy, object, sizeof object);
    float point[3] = {1.0f, 2.0f, 3.0f};
    run__11AddParticleG6Point3(object, point);
    if (memcmp(copy, object, sizeof object) != 0 || point[0] != 1.0f || point[1] != 2.0f || point[2] != 3.0f) {
        puts("FAIL: run changed memory");
        return 1;
    }
    puts("AddParticle run ok");
    return 0;
}

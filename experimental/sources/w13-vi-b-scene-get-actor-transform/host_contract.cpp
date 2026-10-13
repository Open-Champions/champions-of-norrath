#define NORRATH_HOST_CONTRACT 1
#include <string.h>
static unsigned char arena[256];
extern "C" unsigned char* norrath_host_address(int value) { return arena + value; }
#include "get_actor_transform.cpp"

static const void* sa; static const void* sb; static void* sm; static float sf; static int calls;
extern "C" void Transform__10VIMatrix44RC7VIVect3fT1(void* m, const void* a, float angle, const void* b) {
    ++calls; sm = m; sa = a; sb = b; sf = angle;
}

int main() {
    unsigned char matrix[64];
    *reinterpret_cast<float*>(arena + 92) = 2.5f;
    if (GetActorTransform__7VISceneiR10VIMatrix44(0, 16, matrix) != 0) return 1;
    // address 16: vector at 96, float at 108, vector at 84
    *reinterpret_cast<float*>(arena + 108) = 7.25f;
    calls = 0;
    if (GetActorTransform__7VISceneiR10VIMatrix44(0, 16, matrix) != 0) return 2;
    if (calls != 1 || sm != matrix || sa != arena + 96 || sb != arena + 84 || sf != 7.25f) return 3;
    return 0;
}

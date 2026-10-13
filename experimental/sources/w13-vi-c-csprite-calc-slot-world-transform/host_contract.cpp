// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "csprite-calc-slot-world-transform.cpp"
#include <cstring>

static int log_[16], n;
static void* args_[16][5];
static int ints_[16][2];
extern "C" int GetActorTransform__7VISceneiR10VIMatrix44(void* scene, int actor, void* matrix) {
    args_[n][0] = scene; args_[n][1] = matrix; ints_[n][0] = actor;
    // the local matrix is zero-filled when it reaches the call
    if (matrix != 0) {
        const float* f = static_cast<const float*>(matrix);
        for (int i = 0; i < 16; ++i) if (f[i] != 0.0f) log_[15] = 1;
    }
    log_[n++] = 1;
    return 0;
}
extern "C" int CalcNodeTransform__9VIHSpriteiR10VIMatrix44R7VISceneT2(void* sprite, int node, void* matrix, void* scene,
                                                                      void* output) {
    args_[n][0] = sprite; args_[n][1] = matrix; args_[n][2] = scene; args_[n][3] = output; ints_[n][0] = node;
    log_[n++] = 2;
    return 0;
}
extern "C" void _$_10VIMatrix44(void* matrix, int flags) {
    args_[n][0] = matrix; ints_[n][0] = flags;
    log_[n++] = 3;
}

int main() {
    alignas(16) unsigned char sprite[4000];
    alignas(16) unsigned char scene[16], output[64];
    std::memset(sprite, 0, sizeof sprite);
    std::memset(output, 0, sizeof output);
    int slots[3] = {-1, 7, -1};
    std::memcpy(sprite + 3860, slots, sizeof slots);
    // slot -1
    if (CalcSlotWorldTransform__9VICSprite15VICSpriteNodeIDiR7VISceneR10VIMatrix44(sprite, 0, 41, scene, output) != 0) return 1;
    if (n != 2 || log_[0] != 1 || log_[1] != 3) return 2;
    if (args_[0][0] != scene || args_[0][1] != output || ints_[0][0] != 41) return 3;
    if (ints_[1][0] != 2 || args_[1][0] == output) return 4;
    // node 7
    n = 0;
    CalcSlotWorldTransform__9VICSprite15VICSpriteNodeIDiR7VISceneR10VIMatrix44(sprite, 1, 42, scene, output);
    if (n != 3 || log_[0] != 1 || log_[1] != 2 || log_[2] != 3) return 5;
    if (args_[0][0] != scene || ints_[0][0] != 42 || args_[0][1] == output) return 6;
    void* local = args_[0][1];
    if (args_[1][0] != sprite || ints_[1][0] != 7 || args_[1][1] != local || args_[1][2] != scene || args_[1][3] != output) return 7;
    if (args_[2][0] != local || ints_[2][0] != 2) return 8;
    if (log_[15] != 0) return 9;
    return 0;
}

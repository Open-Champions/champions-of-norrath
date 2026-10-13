// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "scene-get-actor-location.cpp"

int main() {
    alignas(8) unsigned char scene[64] = {0};
    alignas(8) unsigned char actor[128] = {0};
    if (GetActorLocation__C7VIScenei(scene, actor) != actor + 68) return 1;
    if (GetActorLocation__C7VIScenei(0, actor + 8) != actor + 76) return 2;
    return 0;
}

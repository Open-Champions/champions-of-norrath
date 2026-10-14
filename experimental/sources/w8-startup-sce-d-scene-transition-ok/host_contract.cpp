#include "scene_transition_ok.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof(sceneTransitionOK) == 4, "one word");
    if (sceneTransitionOK != 0) return 1;
    sceneTransitionOK = 1;
    if (sceneTransitionOK != 1) return 2;
    std::puts("scene transition ok contract passed");
    return 0;
}

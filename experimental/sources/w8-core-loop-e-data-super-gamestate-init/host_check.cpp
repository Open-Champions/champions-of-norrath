#include "super_gamestate_init.cpp"
int main() {
    if (superGamestateInit.name != nullptr || superGamestateInit.state != 0) return 1;
    static const char text[] = "x";
    superGamestateInit.name = text;
    superGamestateInit.state = 4;
    return superGamestateInit.name == text && superGamestateInit.state == 4 ? 0 : 2;
}

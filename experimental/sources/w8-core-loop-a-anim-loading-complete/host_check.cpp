// Host check for animLoadingComplete. Not an EE plan input.
#include "anim_loading_complete.cpp"

int main() {
    if (sizeof(animLoadingComplete) != 1 || animLoadingComplete != 1) return 1;
    return 0;
}

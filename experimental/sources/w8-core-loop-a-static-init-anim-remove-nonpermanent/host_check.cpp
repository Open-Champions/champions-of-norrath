// Host check for _GLOBAL_$I$animRemoveNonpermanentAnimations__FP14AnimationState. Not an EE plan input.
#include "static_init_anim_remove_nonpermanent.cpp"

int main() {
    // The original has no memory effect, so the check calls it repeatedly and confirms control returns.
    _GLOBAL_$I$animRemoveNonpermanentAnimations__FP14AnimationState();
    _GLOBAL_$I$animRemoveNonpermanentAnimations__FP14AnimationState();
    return 0;
}

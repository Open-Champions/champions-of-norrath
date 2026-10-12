// Observed in the option-menu translation unit: the local function
// __static_initialization_and_destruction_0 at 0x001ca7c8 (176 bytes), and
// the two wrappers _GLOBAL_$I$optionMenu (0x001caa00) and
// _GLOBAL_$D$optionMenu (0x001caa20), 32 bytes each, listed in the constructor
// and destructor lists. The wrappers call the local function with
// (1, 0xffff) and (0, 0xffff).
//
// Observed: the local function acts only when its second argument is 0xffff.
// With first argument 1 it walks a local array of three 40-byte elements at
// 0x007d0910 (zero-initialised storage in the image) and stores the byte 1 at
// offset 5 of each, which is the effect of an inline AnimationState
// constructor on zeroed storage. With first argument 0 it walks the same
// array from the last element to the first and calls the external
// _$_14AnimationState(element, 2) on each.
// Inferred: the elements are AnimationState objects, from the callee name.
// Unknown: the other 39 bytes of a constructed element, the name of the
// array, and the original source name of this local function; the helper
// below uses a placeholder name because the compiler reserves the original.
extern "C" {
void _$_14AnimationState(void* object, int flags);
}

namespace {
// `used` keeps the stores: the readers of this array are other functions of
// the same original unit that are not authored yet.
__attribute__((used)) unsigned char option_menu_animation_states[3][40];

__attribute__((noinline)) void option_menu_static_work(int constructing, int priority) {
    if (priority != 0xffff) return;
    if (constructing == 1) {
        for (int i = 0; i < 3; ++i) option_menu_animation_states[i][5] = 1;
    }
    if (constructing == 0) {
        for (int i = 2; i >= 0; --i) _$_14AnimationState(option_menu_animation_states[i], 2);
    }
}
}

extern "C" {
void _GLOBAL_$I$optionMenu() {
    option_menu_static_work(1, 0xffff);
}
}

extern "C" {
void _GLOBAL_$D$optionMenu() {
    option_menu_static_work(0, 0xffff);
}
}

// Host contract for the quest-information constructor/destructor entries. Not an EE plan input.
#include <cstring>
#include "quest_info_static_init.cpp"
static void* calls[8]; static int flagsSeen[8]; static int n;
extern "C" void _$_14AnimationState(void* object, int flags) { calls[n] = object; flagsSeen[n] = flags; ++n; }
int main() {
    unsigned char* a = norrath_quest_info_anim_state_view;
    std::memset(a, 0x5a, 160);
    _GLOBAL_$I$displayQuestInfo__FP11questStructi();
    for (int i = 0; i < 160; ++i) { unsigned char want = (i % 40 == 5) ? 1 : 0x5a; if (a[i] != want) return 1; }
    if (n != 0) return 2;
    _GLOBAL_$D$displayQuestInfo__FP11questStructi();
    if (n != 4) return 3;
    for (int i = 0; i < 4; ++i) if (calls[i] != a + (3 - i) * 40 || flagsSeen[i] != 2) return 4;
    for (int i = 0; i < 160; ++i) { unsigned char want = (i % 40 == 5) ? 1 : 0x5a; if (a[i] != want) return 5; }
    // The local routine does nothing for any other priority or phase.
    n = 0; std::memset(a, 0x11, 160);
    staticInitQuestInfo(1, 0xfffe); staticInitQuestInfo(0, 0); staticInitQuestInfo(2, 0xffff);
    if (n != 0) return 6;
    for (int i = 0; i < 160; ++i) if (a[i] != 0x11) return 7;
    return 0;
}

// Observed _GLOBAL_$I$displayQuestInfo__FP11questStructi (0x001c1c88) and
// _GLOBAL_$D$displayQuestInfo__FP11questStructi (0x001c1ca8), 32 bytes each,
// pinned SLUS_205.65, and the unit's local routine at 0x001c18d8 (156 bytes)
// that both call with the second argument 0xffff. The first argument is 1 for
// the constructor entry and 0 for the destructor entry.
// Observed: with priority 0xffff and phase 1, byte 5 of each of four 40-byte
// records at 0x007d0440 is set to 1 (the other 39 bytes of each record are not
// written). With priority 0xffff and phase 0, the four records are destroyed in
// reverse order by calling _$_14AnimationState(record, 2).
// Inferred: the records are a static array of AnimationState objects of the
// quest-information unit; their local ELF name is animState (one of three
// unit-local objects of that name). Here the array is the zero-filled object
// norrath_quest_info_anim_state_view. Not executed on hardware. The destructor
// body belongs to another package.
extern "C" {
unsigned char norrath_quest_info_anim_state_view[160];
void _$_14AnimationState(void* object, int flags);
}

__attribute__((noinline)) static void staticInitQuestInfo(int phase, int priority) {
    if (priority == 0xffff && phase == 1) {
        for (int i = 0; i < 4; ++i) norrath_quest_info_anim_state_view[i * 40 + 5] = 1;
    }
    if (priority == 0xffff && phase == 0) {
        for (int i = 3; i >= 0; --i) _$_14AnimationState(norrath_quest_info_anim_state_view + i * 40, 2);
    }
}

extern "C" void _GLOBAL_$I$displayQuestInfo__FP11questStructi() { staticInitQuestInfo(1, 0xffff); }
extern "C" void _GLOBAL_$D$displayQuestInfo__FP11questStructi() { staticInitQuestInfo(0, 0xffff); }

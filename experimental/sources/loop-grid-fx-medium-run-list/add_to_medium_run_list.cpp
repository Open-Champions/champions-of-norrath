// Observed objectAddToMediumRunList__FP10GameObject at 0x143908, extent 100.
// This is the same routine the vehicle-actors package published as
// norrath_actor_add_medium_run_list. Importers (actors-a) call it by the name
// norrath_object_add_to_medium_run_list, and the plan rejects aliases, so the
// routine is published here under that name too. A plan should select one of
// the two sources. The storage is the vehicle-actors pair:
// norrath_medium_list_cycle_word (0x483ad4) and norrath_medium_run_list_heads
// (0x4fbf90, fifteen heads).
// The node word at object+4 is removed when nonzero. The cycle word gains one
// and becomes zero when the signed sum is 15 or more; that store happens in the
// link call's delay slot. The link receives the head for the new cycle value,
// object+4 and the object, and its result is returned.
extern "C" unsigned norrath_list_remove(unsigned char* node);
extern "C" void* norrath_list_oneway_link(void* head, void* object_head, void* value);
extern "C" unsigned char norrath_medium_run_list_heads[];
extern "C" unsigned norrath_medium_list_cycle_word;

extern "C" unsigned norrath_object_add_to_medium_run_list(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    const unsigned existing = unsigned(bytes[4]) | (unsigned(bytes[5]) << 8) | (unsigned(bytes[6]) << 16) | (unsigned(bytes[7]) << 24);
    if (existing != 0u) (void)norrath_list_remove(reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(existing)));
    int next = static_cast<int>(norrath_medium_list_cycle_word) + 1;
    if (next >= 15) next = 0;
    norrath_medium_list_cycle_word = static_cast<unsigned>(next);
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(
        norrath_list_oneway_link(norrath_medium_run_list_heads + static_cast<unsigned>(next) * 4u, bytes + 4, object)));
}

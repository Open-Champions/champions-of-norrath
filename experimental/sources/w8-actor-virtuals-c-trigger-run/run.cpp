#include "../w8-actor-virtuals-c-bytes/bytes.hpp"
// Observed msg_run__7Trigger at 0x1d1570, 736 bytes.
// Fields used: float box corners at +76/+80 and +84/+88, script name pointer at
// +92, byte +96 (current triggered state), byte +97 (box searched), word +100
// (countdown), word +104 (index of the triggering player or -1), word +108 (the
// attached TriggerParams object), and the virtual table at +72.
//
// 1. First call only (signed byte +97 is zero): +97 becomes 1. objectFindInBox
//    receives the corner pair {+76, +80, -12000.0} and {+84, +88, 12000.0}
//    (both passed by address), a 512-byte list and capacity 128. Each listed
//    object is asked, through its virtual table's first function pointer (called
//    with no meaningful argument), for its type descriptor; the first whose
//    descriptor equals __tf13TriggerParams() gets this object stored at its +76
//    and this object stores it at +108. The scan ends at the first match or at
//    the list's terminating null. (The image has no index bound; this source stops
//    at 128 entries so the buffer cannot be overrun.)
// 2. A nonzero countdown at +100 is decremented and the call ends.
// 3. Otherwise +104 becomes -1 and the player records are scanned in order below
//    the word nmPlayers (object pointer at +21872 of each 23984-byte record of
//    `player`; null pointers skipped). The virtual slot at byte offset 136 is
//    called with this object and the player object; the first nonzero answer
//    sets the new state to 1 and +104 to that player's index. With no answer the
//    new state is 0.
// 4. When the new state equals the signed byte at +96 the call ends. Otherwise
//    the on/off decision starts as the new state and is overridden by the tags
//    on the +108 object: findTagInt "onOnly" sets it to 1, then "offOnly" sets
//    it to 0 (each also marks a flag that a tag was found); objectDoMyActions
//    runs on the +108 object with the decision when that object is present.
//    currentRunningObject is set to this object around the script start. With
//    decision 1, activePlayer is set to the triggering player's index and the
//    script named at +92 is started; with decision 0 the name is formatted into
//    a buffer through the "not%s" pattern and that script is started. Byte +96 is
//    set to the new state.
// 5. Only when a tag was found (decision overridden by "onOnly" or "offOnly"):
//    if the +108 object exists and tag "timeout" is nonzero on it, a countdown of
//    -1 stays and +96 is cleared, else the countdown becomes that tag's value
//    shifted left by 2 and +96 is cleared. Otherwise tag "needScriptHit" is read
//    from the +108 object (the call is made even when the pointer is null): when
//    it is nonzero and the script start returned zero only +96 is cleared, in
//    every other case the countdown becomes -1 and +96 is cleared.
// Unknown: the roles of the box, the TriggerParams object and the tags, and the
// units of the countdown. The 12000 bound is the pair of floats at 0x480694.
extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(const void* low, const void* high, void** out, int capacity);
extern "C" void* __tf13TriggerParams();
extern "C" int __eq__C9type_infoRC9type_info(void* left, void* right);
extern "C" int norrath_game_object_find_tag_int(void* object, char* tag, int occurrence);
extern "C" void objectDoMyActions__FP10GameObjecti(void* object, int on);
extern "C" int scriptStart__FPc(char* name);
extern "C" int sprintf(char* out, const char* format, ...);
extern "C" unsigned char player[];
extern "C" unsigned char nmPlayers[4];
extern "C" unsigned char activePlayer[4];
extern "C" unsigned char* currentRunningObject;

namespace {
typedef void* (*TypeFunction)();
typedef int (*TestFunction)(void*, void*);

int find_tag(void* object, const char* text) {
    return norrath_game_object_find_tag_int(object, const_cast<char*>(text), 0);
}
}

extern "C" void msg_run__7Trigger(void* self) {
    using namespace w8c;
    static const char on_only[] = {'o', 'n', 'O', 'n', 'l', 'y', '\0'};
    static const char off_only[] = {'o', 'f', 'f', 'O', 'n', 'l', 'y', '\0'};
    static const char timeout[] = {'t', 'i', 'm', 'e', 'o', 'u', 't', '\0'};
    static const char need_hit[] = {'n', 'e', 'e', 'd', 'S', 'c', 'r', 'i', 'p', 't', 'H', 'i', 't', '\0'};
    static const char negated[] = {'n', 'o', 't', '%', 's', '\0'};
    const Address base = address(self);
    int trigger_player = 0;
    if (static_cast<signed char>(read_u8(base + 97u)) == 0) {
        write_u8(base + 97u, 1u);
        const float low[3] = {read_single(base + 76u), read_single(base + 80u), single_from_bits(0xc63b8000u)};
        const float high[3] = {read_single(base + 84u), read_single(base + 88u), single_from_bits(0x463b8000u)};
        void* list[128];
        objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, list, 128);
        for (int i = 0; i < 128 && list[i] != nullptr; ++i) {
            void* const other = list[i];
            const Address table = address(pointer(read_word(address(other) + 72u)));
            void* const type = reinterpret_cast<TypeFunction>(pointer(read_word(table + 4u)))();
            if (__eq__C9type_infoRC9type_info(type, __tf13TriggerParams()) != 0) {
                write_word(address(other) + 76u, address_bits(self));
                write_word(base + 108u, address_bits(other));
                break;
            }
        }
    }
    const unsigned countdown = read_word(base + 100u);
    if (countdown != 0u) {
        write_word(base + 100u, countdown - 1u);
        return;
    }
    write_word(base + 104u, 0xffffffffu);
    int state = 0;
    for (int i = 0; i < signed_word(read_word(address(nmPlayers))); ++i) {
        void* const other = pointer(read_word(address(player) + 21872u + 23984u * static_cast<unsigned>(i)));
        if (other == nullptr) continue;
        void* adjusted;
        TestFunction test = slot_function<TestFunction>(self, 136u, &adjusted);
        if (test(adjusted, other) != 0) {
            trigger_player = i;
            state = 1;
            write_word(base + 104u, static_cast<unsigned>(i));
            break;
        }
    }
    if (state == static_cast<signed char>(read_u8(base + 96u))) return;
    int decision = state;
    int found = 0;
    if (read_word(base + 108u) != 0u) {
        if (find_tag(pointer(read_word(base + 108u)), on_only) != 0) {
            decision = 1;
            found = 1;
        }
        if (read_word(base + 108u) != 0u) {
            if (find_tag(pointer(read_word(base + 108u)), off_only) != 0) {
                decision = 0;
                found = 1;
            }
            if (read_word(base + 108u) != 0u)
                objectDoMyActions__FP10GameObjecti(pointer(read_word(base + 108u)), decision);
        }
    }
    currentRunningObject = static_cast<unsigned char*>(self);
    int started;
    if (decision != 0) {
        write_word(address(activePlayer), static_cast<unsigned>(trigger_player));
        started = scriptStart__FPc(static_cast<char*>(pointer(read_word(base + 92u))));
    } else {
        char name[160];
        sprintf(name, negated, static_cast<char*>(pointer(read_word(base + 92u))));
        started = scriptStart__FPc(name);
    }
    currentRunningObject = nullptr;
    write_u8(base + 96u, static_cast<unsigned>(state));
    if (found == 0) return;
    if (read_word(base + 108u) != 0u) {
        if (find_tag(pointer(read_word(base + 108u)), timeout) != 0) {
            if (read_word(base + 100u) == 0xffffffffu) {
                write_u8(base + 96u, 0u);
                return;
            }
            write_word(base + 100u, static_cast<unsigned>(find_tag(pointer(read_word(base + 108u)), timeout)) << 2);
            write_u8(base + 96u, 0u);
            return;
        }
    }
    if (find_tag(pointer(read_word(base + 108u)), need_hit) != 0 && started == 0) {
        write_u8(base + 96u, 0u);
        return;
    }
    write_word(base + 100u, 0xffffffffu);
    write_u8(base + 96u, 0u);
}

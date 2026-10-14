/* Observed sceSifAddCmdHandler at 0x002d7490, 120 bytes. The command id picks
 * a table by its sign bit. A negative id uses the pointer at word 3 (offset
 * 12) of the command state object at 0x008dea18 and the index
 * (id & 0x7fffffff). A nonnegative id uses the pointer at word 5 (offset 20)
 * and the id itself. The state object is imported under the name
 * norrath_sound_command_state_view; the SIF RPC unit has a different local
 * also called _data_table, at 0x008e0440.
 * Each entry is 12 bytes: the handler at +0, the second argument at +4, and
 * the current $gp at +8. The index is not bounds checked. Returns nothing.
 */
struct HandlerEntry {
    void* handler;
    void* argument;
    unsigned gp;
};

struct DataTable {
    unsigned before[3];
    HandlerEntry* negative_ids;
    unsigned between;
    HandlerEntry* nonnegative_ids;
};

extern "C" DataTable norrath_sound_command_state_view;

#if defined(__mips__) && !defined(NORRATH_HOST_CONTRACT)
static unsigned current_gp() {
    unsigned value;
    __asm__("move %0,$gp" : "=r"(value));
    return value;
}
#else
extern "C" unsigned norrath_host_gp;
static unsigned current_gp() { return norrath_host_gp; }
#endif

extern "C" void sceSifAddCmdHandler(int id, void* handler, void* argument) {
    HandlerEntry* entry = id < 0 ? norrath_sound_command_state_view.negative_ids + (static_cast<unsigned>(id) & 0x7fffffffu)
                                 : norrath_sound_command_state_view.nonnegative_ids + id;
    entry->argument = argument;
    entry->handler = handler;
    entry->gp = current_gp();
}

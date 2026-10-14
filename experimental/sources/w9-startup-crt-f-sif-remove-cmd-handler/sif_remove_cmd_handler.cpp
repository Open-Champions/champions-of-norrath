/* Observed sceSifRemoveCmdHandler at 0x002d7508, 76 bytes. The command id
 * picks a table by its sign bit, as in sceSifAddCmdHandler. A negative id
 * uses the pointer at offset 12 of the command state object at 0x008dea18 and
 * the index (id & 0x7fffffff); a nonnegative id uses the pointer at offset 20
 * and the id itself. Only word 0 (the handler) of the 12-byte entry is set to
 * zero; the argument and gp words stay. The index is not bounds checked.
 * Returns nothing.
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

extern "C" void sceSifRemoveCmdHandler(int command) {
    if (command < 0) {
        norrath_sound_command_state_view.negative_ids[command & 0x7fffffff].handler = 0;
    } else {
        norrath_sound_command_state_view.nonnegative_ids[command].handler = 0;
    }
}

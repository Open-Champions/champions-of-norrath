// Observed throw_helper at 0x002face0, 908 bytes, a local function of the exception-support part of the
// runtime library; __throw calls it with the context, the return address minus one, its own 1144-byte frame
// record and the address of a word that receives a stack adjustment. It returns the address of the handler.
// Arguments: eh (the context __throw gets from get_eh_context), pc, my_udata (__throw's frame record) and
// offset_p. The context's words are the handler slot (offset 0), the information record (offset 8) and a
// re-throw table index (offset 12). In order:
//  1. rethrow = (eh->table_index != 0). The working record udata (a local) is a copy of *my_udata; the
//     other two records (the previous one and a saved one) are locals; handler, saved state and cleanup flags
//     start at zero; the entry pc is remembered.
//  2. Loop: udata = next_stack_level(pc, udata, previous) (the old record becomes previous); a null result
//     ends the search. The frame is of the new kind when the record's table pointer (offset 4) is non-null
//     and its first word is -2. For a re-throw the handler comes from find_exception_handler(table_index,
//     table, eh->info, 1, &cleanup) and eh->table_index is cleared, else for the new kind from
//     find_exception_handler(pc, table, eh->info, 0, &cleanup), else from old_find_exception_handler(pc,
//     table). When a handler is found: with the cleanup flag clear the search stops (only_cleanup = 0);
//     with it set and nothing saved yet, the record is saved (1144 bytes), the handler and pc remembered
//     and only_cleanup set, and the search goes on. Then pc = the word get_reg_addr(udata's return-address
//     column (halfword at offset 1018), udata, previous) points to, truncated to a pointer, minus 1.
//  3. After the loop, a saved state replaces udata, handler and pc; when only_cleanup is set then
//     __unwinding_cleanup() runs. No handler calls __terminate; otherwise eh->handler_label = handler.
//  4. args_size = the 64-bit word at udata offset 24. When pc equals the entry pc, udata becomes my_udata.
//     Otherwise the frames from the throw to the handler's frame are unwound again from a fresh copy of
//     *my_udata: with handler_pc the pc found, pc restarts at the entry pc and, while pc differs from
//     handler_pc, udata = next_stack_level(...), every register index i below 122 other than the return
//     column whose byte at offset 1020 + i of udata is non-zero is passed to copy_reg(i, udata, my_udata),
//     and pc = the truncated return-address word minus 1 again.
//  5. *offset_p (a 64-bit store) = (int)(udata->cfa - my_udata->cfa) + args_size, 64-bit sum; returns handler.
// Inferred: the routine is the GNU runtime's throw_helper; the record fields are named after the public
// frame-state layout only where the offsets agree (cfa at 0, table pointer at 4, args size at 24, the 122
// register slots, return column at 1018, save flags at 1020).
// Unknown: the contents of the record between offsets 8 and 24 and between 32 and 1016, and what
// the helpers do beyond their signatures. __terminate is expected not to return; with a returning stub the
// handler slot is left untouched. Not authored here: next_stack_level, find_exception_handler,
// old_find_exception_handler, get_reg_addr, copy_reg, __unwinding_cleanup (local routines of the original,
// kept as imports under their symbol-table names); __terminate is in the runtime.
extern "C" {
struct NorrathThrowFrame {
    char* cfa;
    void* table;
    long long words8[2];
    long long args_size;
    long long registers[123];
    unsigned short cfa_reg;
    unsigned short return_column;
    char saved[123];
};
struct NorrathThrowContext {
    void* handler_label;
    void* word4;
    void* info;
    void* table_index;
};

NorrathThrowFrame* next_stack_level(void* pc, NorrathThrowFrame* udata, NorrathThrowFrame* previous);
void* find_exception_handler(void* pc, void* table, void* info, int rethrow, int* cleanup);
void* old_find_exception_handler(void* pc, void* table);
long long* get_reg_addr(unsigned reg, NorrathThrowFrame* udata, NorrathThrowFrame* previous);
void copy_reg(unsigned reg, NorrathThrowFrame* udata, NorrathThrowFrame* target);
void __unwinding_cleanup();
void __terminate();

static void* return_address(NorrathThrowFrame* udata, NorrathThrowFrame* previous) {
    return reinterpret_cast<void*>(static_cast<int>(*get_reg_addr(udata->return_column, udata, previous)));
}

void* throw_helper(NorrathThrowContext* eh, void* pc, NorrathThrowFrame* my_udata, long long* offset_p) {
    NorrathThrowFrame ustruct2;
    NorrathThrowFrame ustruct;
    NorrathThrowFrame saved_ustruct;
    NorrathThrowFrame* udata = &ustruct2;
    NorrathThrowFrame* sub_udata = &ustruct;
    void* entry_pc = pc;
    void* saved_pc = nullptr;
    void* ret_val = nullptr;
    void* handler;
    int cleanup = 0;
    int only_cleanup = 0;
    int saved_state = 0;
    void* const info = eh->info;
    int rethrow = eh->table_index != nullptr;
    *udata = *my_udata;
    handler = nullptr;
    for (;;) {
        NorrathThrowFrame* p = udata;
        udata = next_stack_level(pc, udata, sub_udata);
        sub_udata = p;
        if (udata == nullptr) break;
        bool new_model = false;
        if (udata->table != nullptr) new_model = *static_cast<int*>(udata->table) == -2;
        if (rethrow) {
            handler = find_exception_handler(eh->table_index, udata->table, info, 1, &cleanup);
            eh->table_index = nullptr;
            rethrow = 0;
        } else if (new_model) {
            handler = find_exception_handler(pc, udata->table, info, 0, &cleanup);
        } else {
            handler = old_find_exception_handler(pc, udata->table);
        }
        if (handler != nullptr) {
            if (cleanup == 0) {
                only_cleanup = 0;
                break;
            }
            if (saved_state == 0) {
                saved_ustruct = *udata;
                ret_val = handler;
                saved_pc = pc;
                saved_state = 1;
                only_cleanup = 1;
            }
        }
        pc = static_cast<char*>(return_address(udata, sub_udata)) - 1;
    }
    if (saved_state != 0) {
        udata = &saved_ustruct;
        handler = ret_val;
        pc = saved_pc;
        if (only_cleanup != 0) __unwinding_cleanup();
    }
    if (handler != nullptr) eh->handler_label = handler;
    else __terminate();
    const long long args_size = udata->args_size;
    if (pc == entry_pc) {
        udata = my_udata;
    } else {
        void* const handler_pc = pc;
        pc = entry_pc;
        *udata = *my_udata;
        if (pc != handler_pc) {
            do {
                NorrathThrowFrame* p = udata;
                udata = next_stack_level(pc, udata, sub_udata);
                sub_udata = p;
                for (int i = 0; i < 122; ++i)
                    if (i != udata->return_column && udata->saved[i] != 0) copy_reg(i, udata, my_udata);
                pc = static_cast<char*>(return_address(udata, sub_udata)) - 1;
            } while (pc != handler_pc);
        }
    }
    *offset_p = static_cast<long long>(static_cast<int>(udata->cfa - my_udata->cfa)) + args_size;
    return handler;
}
}

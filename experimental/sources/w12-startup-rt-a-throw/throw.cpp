// Observed __throw at 0x002fb070, 336 bytes, pinned SLUS_205.65. Starts a C++ throw: it finds the
// exception context through the function pointer get_eh_context (0x004480dc), walks to its own
// caller and returns control to the handler the unwinder chose. In order:
//   1. context = get_eh_context(); a zero word at context+8 calls __terminate (which is expected not to
//      return; when it does, the routine goes on).
//   2. state = __frame_state_for(<an address inside this function>, <a 1144-byte record on its own stack>);
//      a null result calls __terminate, which is expected not to return.
//   3. word 0 of that record is set to the canonical frame address (the stack pointer on entry).
//   4. handler = throw_helper(context, return address - 1, record, &adjust), a local function of the
//      same library, imported by name. It stores a stack adjustment through the last argument.
//   5. The callee-saved registers (including the float registers $f20..$f31) of the throwing caller are
//      restored, the stack pointer gets the adjustment added, and execution continues at the handler
//      with the context pointer in $v0 and the adjustment in $v1.
// The original ends through a two-word stub placed inside the function (jump to the handler, adding the
// adjustment to the stack pointer in the delay slot); here the compiler's exception return does the
// equivalent. DEVIATION: the handler is entered with the same stack pointer and callee-saved integer and
// float registers (a paired run compared them), but $v0 holds the handler address instead of the context
// pointer, $ra holds the handler address instead of the stub address and the argument registers hold their
// entry values. Whether any landing pad reads $v0 is unknown, so this function is not claimed faithful at
// the handler entry.
// Unknown: the record's layout beyond word 0 and throw_helper's contract.
extern "C" {
struct EhContext {
    void* word0;
    void* word4;
    void* info;
};
struct FrameStateRecord {
    unsigned char bytes[1144];
};

extern void* (*get_eh_context)();
void __terminate();
FrameStateRecord* __frame_state_for(void* pc, FrameStateRecord* state);
void* throw_helper(EhContext* context, void* pc, FrameStateRecord* state, long* adjust);

void __throw() {
    EhContext* context = static_cast<EhContext*>(get_eh_context());
    FrameStateRecord first;
    FrameStateRecord* state = &first;
    long adjust;
    if (context->info == nullptr) __terminate();
here:
    state = __frame_state_for(&&here, state);
    if (state == nullptr) __terminate();
    *reinterpret_cast<void**>(state) = __builtin_dwarf_cfa();
    __builtin_unwind_init();
    void* pc = static_cast<char*>(__builtin_extract_return_addr(__builtin_return_address(0))) - 1;
    void* handler = throw_helper(context, pc, state, &adjust);
    __builtin_eh_return(adjust, handler);
}
}

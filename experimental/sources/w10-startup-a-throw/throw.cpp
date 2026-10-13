// Observed __throw at 0x002fb070, 336 bytes. Starts a C++ throw by walking the stack from its own
// frame. It reads the exception context through the function pointer at 0x004480dc (the
// file-local get_eh_context), calls __terminate if the context's word at +8 is zero, asks
// __frame_state_for for the unwind description of its own call site (the address of a label in
// the function), stores the frame's canonical address in word 0 of that record, and calls the
// file-local throw_helper at 0x002face0 with the context, the return address minus one, the
// record and a pointer to a stack-adjust word. It then returns through an exception-return
// sequence: the stack adjust, the handler address and the context go in v1, a0 and v0, and the
// return address is replaced by a stub (jr a0 with the adjust added to sp in the delay slot).
// Inferred: the record is 1144 bytes (throw_helper copies that much of it); its layout beyond
// word 0 is not examined here. The compiler's own exception-return expansion replaces the
// original's explicit stub: it leaves through the handler address with the stack adjust applied,
// so the handler sees a different ra than the original stub left, and v0 is not set to the
// context as the original does (the compiler's expansion gives no way to place it). Unknown: throw_helper's own
// contract beyond what its arguments show; it is a local function and is imported by name.
// Nothing was executed on the EE.
extern "C" {
struct W10EhContext {
    void* word0;
    void* word4;
    void* info;
};
struct W10FrameState {
    unsigned char bytes[1144];
};

extern void* (*get_eh_context)();
void __terminate();
W10FrameState* __frame_state_for(void* pc, W10FrameState* state);
void* throw_helper(W10EhContext* context, void* pc, W10FrameState* state, long* adjust);

void __throw() {
    W10EhContext* context = static_cast<W10EhContext*>(get_eh_context());
    W10FrameState first;
    W10FrameState* state = &first;
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

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
//      restored from this function's frame, the return address slot is pointed at a two-word stub inside the
//      function, and the function returns into the stub. The stub jumps to the handler (in $a0) and adds the
//      adjustment ($v1) to the stack pointer in the delay slot; $v0 holds the context pointer at that point.
// Here the compiler's exception return restores the registers and adds the adjustment, and a small stub written
// in the inline assembly of this function (carrying the context, handler and adjustment in $t4..$t6, which the
// epilogue leaves alone) loads $v0 = context, $v1 = adjustment, $a0 = handler and jumps to the handler. A paired
// run compared the stack pointer, $v0, $v1, $a0 and the callee-saved integer and float registers at handler entry.
// Not matched: $ra at handler entry holds the address of this unit's stub, not the original stub's address,
// $a1..$a3 hold their entry values and $t4..$t6 hold the stub inputs. Whether any landing pad reads these is unknown.
// Unknown: the record's layout beyond word 0 and throw_helper's contract.
extern "C" {
struct EhContext { void* word0; void* word4; void* info; };
struct FrameStateRecord { unsigned char bytes[1144]; };
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
    void* stub;
    __asm__ volatile(".set push\n\t.set noreorder\n\t.set noat\n\tmove $12,%1\n\tmove $13,%2\n\tmove $14,%3\n\tla %0,2f\n\tb 3f\n\tnop\n"
                     "2:\tmove $2,$12\n\tmove $4,$13\n\tmove $3,$14\n\tjr $4\n\tnop\n3:\t.set pop"
                     : "=&r"(stub) : "r"(context), "r"(handler), "r"(adjust) : "$12", "$13", "$14");
    __builtin_eh_return(adjust, stub);
}
}

// Observed stack.15 at 0x8ddcc0, 1024 nobits bytes in .bss. The thread
// initializer passes this address as the top thread's stack with a 1024
// size word; the stack starts cleared.
// Inferred: this reconstruction name is that object, zero-initialized
// like the nobits image. The C++ name carries the original dotted symbol
// through an asm label, matching how the initializer declares it.
// Unknown: thread use of this stack, and placement at the original
// address.
extern "C" {
unsigned char runtime_tables_init_thread_stack[1024] asm("stack.15") = {};
}

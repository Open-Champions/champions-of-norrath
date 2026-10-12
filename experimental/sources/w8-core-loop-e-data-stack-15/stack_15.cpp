// Observed stack.15 at 0x008ddcc0. The symbol has no recorded size; the next
// symbol, topSema, starts 0x400 bytes later. InitThread (0x002d5eb8) passes
// this address with size 1024 as the stack of a thread it creates, which
// makes the 1024 bytes a thread stack. It is in .bss and zero at load. The
// original name contains a dot, so the label is given through an asm name.
// Alignment is kept at 64 bytes, which the address satisfies; the real
// alignment is not recorded.
extern "C" {
alignas(64) unsigned char stack_15[1024] __asm__("stack.15");
}

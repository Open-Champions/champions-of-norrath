/* Observed: the local kCopy at 0x002dd718, 56 bytes (the copy that
 * InitSystemCallTableAddress passes to FindAddress as a search pattern).
 * It copies a2 >> 2 words from a1 to a0, one word per pass, and returns 0.
 * A zero word count skips the loop. The delay-slot and no-op placement
 * below are the observed bytes: the body is used as a byte pattern, so
 * its encoding has to be exact and a compiled equivalent would not do.
 * Inferred: the pattern is the copy routine that exists inside kernel
 * memory. Unknown: where in kernel memory the pattern is found.
 */
    .set noreorder
    .text
    .balign 8
    .globl kernel_syscall_pattern_kcopy
    .type kernel_syscall_pattern_kcopy,@function
    .ent kernel_syscall_pattern_kcopy
kernel_syscall_pattern_kcopy:
    srl $a2,$a2,2
    beqz $a2,.Lkcopy_done
    daddu $a3,$zero,$zero
    nop
.Lkcopy_loop:
    lw $v1,0($a1)
    addiu $a3,$a3,1
    addiu $a1,$a1,4
    sltu $v0,$a3,$a2
    sw $v1,0($a0)
    addiu $a0,$a0,4
    bnez $v0,.Lkcopy_loop
    nop
.Lkcopy_done:
    jr $ra
    daddu $v0,$zero,$zero
    .end kernel_syscall_pattern_kcopy
    .size kernel_syscall_pattern_kcopy,.-kernel_syscall_pattern_kcopy

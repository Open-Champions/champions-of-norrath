/* Observed: the local kFindAddress at 0x002dd750, 64 bytes (the search
 * pattern that InitSystemCallTableAddress passes to FindAddress). Given a
 * start in a0, an end in a1, and a word in a2, it compares the word at a0,
 * and while a0 is below a1 advances by four and compares again. It returns
 * the address of the first equal word. Otherwise it returns 0.
 * Quirk: the result is cleared by `movz a0,zero,v0`, with v0 holding the
 * word just compared, so a hit on a zero word also returns 0. The body is
 * used as a byte pattern, so its encoding has to be exact and a compiled
 * equivalent would not do.
 * Inferred: the pattern is a search routine that exists inside kernel
 * memory. Unknown: where in kernel memory the pattern is found.
 */
    .set noreorder
    .text
    .balign 16
    .globl kFindAddress
    .type kFindAddress,@function
    .ent kFindAddress
kFindAddress:
    lw $v0,0($a0)
    beq $v0,$a2,.Lfind_found
    sltu $v0,$a0,$a1
    beqzl $v0,.Lfind_out
    movz $a0,$zero,$v0
    addiu $a0,$a0,4
.Lfind_loop:
    lw $v0,0($a0)
    beq $v0,$a2,.Lfind_found
    sltu $v0,$a0,$a1
    bnezl $v0,.Lfind_loop
    addiu $a0,$a0,4
    b .Lfind_out
    movz $a0,$zero,$v0
.Lfind_found:
    movz $a0,$zero,$v0
.Lfind_out:
    jr $ra
    daddu $v0,$a0,$zero
    .end kFindAddress
    .size kFindAddress,.-kFindAddress

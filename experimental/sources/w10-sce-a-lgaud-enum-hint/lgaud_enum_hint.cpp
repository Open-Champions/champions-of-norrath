/* Observed lgAudEnumHint at 0x002ea150, 112 bytes. Argument: a pointer for
 * the hint word. The library lock word (0x00447f7c) equal to -1 returns
 * 0x8000000a with nothing else done. A null pointer prints "lgAudEnumHint:
 * invalid parameter (hint)\n" and returns 0x80000004. Otherwise the first word
 * of the reply block (pointer at 0x00447f78) is copied to the caller, that
 * word is cleared (the pointer is read again for the clear), and the result
 * is 0. The meaning of the word is not established here; the pump that calls
 * it checks whether it is nonzero.
 */
extern "C" {
extern int norrath_lgaud_semaphore_view;
extern unsigned* norrath_lgaud_reply_words_view;

int printf(const char* format, ...);

unsigned lgAudEnumHint(unsigned* hint) {
    if (norrath_lgaud_semaphore_view == -1) return 0x8000000au;
    if (hint == nullptr) {
        printf("lgAudEnumHint: invalid parameter (hint)\n");
        return 0x80000004u;
    }
    *hint = *norrath_lgaud_reply_words_view;
    *norrath_lgaud_reply_words_view = 0;
    return 0;
}
}

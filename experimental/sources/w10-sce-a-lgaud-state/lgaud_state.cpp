/* Observed: four initialised words of the audio-device library in the .data
 * area just after ModifierCodes (0x00447f60, 8 bytes) and before g_codecs
 * (0x00447f90). The image holds:
 *   0x00447f6c  0           upper bound for the byte count of an asynchronous
 *                           read or write (lgAudARead, lgAudAWrite compare it
 *                           with the caller's count)
 *   0x00447f74  0           pointer to the shared request/reply buffer
 *   0x00447f78  0           pointer to the reply word block that the
 *                           background side fills (lgAudEnumHint reads and
 *                           clears its first word)
 *   0x00447f7c  0xffffffff  the library's lock semaphore id; -1 means "not
 *                           initialised" and every call then returns
 *                           0x8000000a
 * The words at 0x00447f68, 0x00447f70, 0x00447f80 and 0x00447f84 are also
 * zero in the image but nothing in this package reads them, so they are not
 * defined here. The symbol table has no names for these words; the names
 * below are descriptive and are not the original's.
 * Inferred: the first three are set by lgAudInit. Unknown: how the upper
 * bound is chosen.
 */
extern "C" {
int norrath_lgaud_size_limit_view = 0;
void* norrath_lgaud_request_buffer_view = 0;
void* norrath_lgaud_reply_words_view = 0;
int norrath_lgaud_semaphore_view = -1;
}

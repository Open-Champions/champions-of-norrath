/* Observed lgAudASync at 0x002eae08, 156 bytes (the symbol's extent also
 * covers the two completion routines that follow it; see
 * lgAudARead and lgAudAWrite). Arguments: a flag and a pointer for the result
 * word. The library lock word (0x00447f7c) equal to -1 returns 0x8000000a with
 * nothing else done. With a nonzero flag, sceSifCheckStatRpc is called once on
 * the library client (0x008ea780); a nonzero status is returned as it is
 * (nothing else is touched); a zero status stores the third word of the
 * asynchronous record (0x008ea930 + 8) through the pointer when it is not null
 * and returns 0. With a zero flag, sceSifCheckStatRpc is polled until it
 * returns zero, then the same store happens and the result is 0.
 * Inferred: the flag selects between a poll and a wait. The meaning of the
 * stored word is the call's own result as left by the completion routine.
 */
extern "C" {
extern int norrath_lgaud_semaphore_view;
extern unsigned char norrath_lgaud_rpc_client_view[];
extern unsigned norrath_lgaud_async_record_view[];

int sceSifCheckStatRpc(void* client);

unsigned lgAudASync(int poll_only, unsigned* result) {
    if (norrath_lgaud_semaphore_view == -1) return 0x8000000au;
    if (poll_only != 0) {
        const int status = sceSifCheckStatRpc(norrath_lgaud_rpc_client_view);
        if (status != 0) return static_cast<unsigned>(status);
        if (result != nullptr) *result = norrath_lgaud_async_record_view[2];
        return 0;
    }
    while (sceSifCheckStatRpc(norrath_lgaud_rpc_client_view) != 0) {
    }
    if (result != nullptr) *result = norrath_lgaud_async_record_view[2];
    return 0;
}
}

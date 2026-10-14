/* Observed CallRpc at 0x002e4e48, 180 bytes. The first argument points at a client
 * object whose signed word at byte offset 48 is a buffer limit. The send size (fourth
 * argument) and the receive size (fifth) are each rounded
 * up to a multiple of 64. When the limit is below either rounded size it prints
 * "[fatal error] insufficient buffer size in Libnet\n" and then
 * "(fno=%d, ssize=%d, rsize=%d, buffer=%d)\n" with the function number, the
 * rounded send size, the rounded receive size and the limit, through scePrintf, and
 * returns -543 (0xfffffde1). Otherwise it returns sceSifMCallRpc(object, function,
 * 0, buffer, rounded send size, buffer, rounded receive size, 0, 0): the same buffer
 * is both the send and the receive area, mode zero, and no end function or
 * argument. Argument names are inferred from that call; the meaning of the
 * object's word at 48 beyond its use as a limit is unknown. Compiled output is not
 * claimed to match the original.
 */
extern "C" {
int scePrintf(const char* format, ...);
int sceSifMCallRpc(void* object, int function, int mode, void* send, int send_size,
                   void* receive, int receive_size, void (*end_function)(void*), void* end_argument);
}

struct CallRpcObject {
    unsigned char before_limit[48];
    int limit;
};

extern "C" int CallRpc(void* object, int function, void* buffer, int send_size, int receive_size) {
    const int rounded_send = (send_size + 63) & -64;
    const int rounded_receive = (receive_size + 63) & -64;
    const int limit = static_cast<CallRpcObject*>(object)->limit;
    if (limit < rounded_send || limit < rounded_receive) {
        scePrintf("[fatal error] insufficient buffer size in Libnet\n");
        scePrintf("(fno=%d, ssize=%d, rsize=%d, buffer=%d)\n", function, rounded_send, rounded_receive, limit);
        return -543;
    }
    return sceSifMCallRpc(object, function, 0, buffer, rounded_send, buffer, rounded_receive, nullptr, nullptr);
}

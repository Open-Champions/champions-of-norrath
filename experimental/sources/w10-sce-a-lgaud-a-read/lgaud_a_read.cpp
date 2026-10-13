/* Observed lgAudARead at 0x002e9bf8, 372 bytes, and its completion routine,
 * a nameless 96-byte routine at 0x002eaea8 (inside the extent of lgAudASync)
 * that is authored here as the file-local read_done. Arguments: a device
 * handle, a one-byte value, a destination and a pointer to the byte count.
 * The library lock word (0x00447f7c) equal to -1 returns 0x8000000a. A null
 * destination prints "lgAudARead: invalid parameter (dest)\n", and a null
 * count pointer or a count above the word at 0x00447f6c prints "lgAudARead:
 * invalid parameter (*size too big)\n"; both return 0x80000004. When
 * sceSifCheckStatRpc on the library client (0x008ea780) is nonzero it prints
 * "lgAudARead: error: another async call is in progress...\n" and returns
 * 0x80000008. Otherwise the lock semaphore is awaited and the shared request
 * buffer (pointer at 0x00447f74) gets the handle in word 3, the count in word
 * 8 and the byte value at byte 18. The asynchronous record (0x008ea930) gets
 * the destination in word 0 and the count pointer in word 1.
 * sceSifCallRpc runs with function 8, mode 1, the buffer as the 48-byte send
 * area, the buffer as receive area of ((count + 63) >> 4) << 4 bytes, the
 * completion routine and the record. A zero call result returns 0 and leaves
 * the lock semaphore taken for the completion routine. A nonzero result stores
 * zero through the count pointer, prints "lgAudARead: rpc call failed\n" and
 * returns 0x80000000 without giving the semaphore back (as observed).
 * The completion routine copies buffer-word-8 bytes from buffer byte 48 to the
 * recorded destination, stores buffer word 8 through the recorded count
 * pointer, stores buffer word 0 in record word 2 and signals the lock
 * semaphore with iSignalSema as a tail call.
 */
extern "C" {
extern int norrath_lgaud_semaphore_view;
extern int norrath_lgaud_size_limit_view;
extern unsigned char* norrath_lgaud_request_buffer_view;
extern unsigned char norrath_lgaud_rpc_client_view[];
extern unsigned norrath_lgaud_async_record_view[];

int printf(const char* format, ...);
int WaitSema(int semaphore);
int iSignalSema(int semaphore);
int sceSifCheckStatRpc(void* client);
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size, void* receive,
                  int receive_size, void* end, void* end_param);
void* memcpy(void* destination, const void* source, __SIZE_TYPE__ count);

static unsigned pointer_bits(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void* norrath_host_word_pointer(unsigned word);  // host check: maps a recorded 32-bit word to its object
static void* word_pointer(unsigned word) { return norrath_host_word_pointer(word); }
#else
static void* word_pointer(unsigned word) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word));
}
#endif

static void read_done(void* record_pointer) {
    unsigned* const record = static_cast<unsigned*>(record_pointer);
    unsigned char* buffer = norrath_lgaud_request_buffer_view;
    memcpy(word_pointer(record[0]), buffer + 48, *reinterpret_cast<unsigned*>(buffer + 32));
    buffer = norrath_lgaud_request_buffer_view;
    *static_cast<unsigned*>(word_pointer(record[1])) = *reinterpret_cast<unsigned*>(buffer + 32);
    buffer = norrath_lgaud_request_buffer_view;
    record[2] = *reinterpret_cast<unsigned*>(buffer);
    iSignalSema(norrath_lgaud_semaphore_view);
}

unsigned lgAudARead(unsigned handle, unsigned char value, void* destination, int* count) {
    if (norrath_lgaud_semaphore_view == -1) return 0x8000000au;
    if (destination == nullptr) {
        printf("lgAudARead: invalid parameter (dest)\n");
        return 0x80000004u;
    }
    if (count == nullptr || norrath_lgaud_size_limit_view < *count) {
        printf("lgAudARead: invalid parameter (*size too big)\n");
        return 0x80000004u;
    }
    if (sceSifCheckStatRpc(norrath_lgaud_rpc_client_view) != 0) {
        printf("lgAudARead: error: another async call is in progress...\n");
        return 0x80000008u;
    }
    WaitSema(norrath_lgaud_semaphore_view);
    unsigned char* buffer = norrath_lgaud_request_buffer_view;
    *reinterpret_cast<unsigned*>(buffer + 12) = handle;
    *reinterpret_cast<int*>(buffer + 32) = *count;
    buffer[18] = value;
    norrath_lgaud_async_record_view[0] = pointer_bits(destination);
    norrath_lgaud_async_record_view[1] = pointer_bits(count);
    const int receive_size = ((*count + 63) >> 4) << 4;
    buffer = norrath_lgaud_request_buffer_view;
    if (sceSifCallRpc(norrath_lgaud_rpc_client_view, 8, 1, buffer, 48, buffer, receive_size,
                      reinterpret_cast<void*>(read_done), norrath_lgaud_async_record_view) == 0)
        return 0;
    *count = 0;
    printf("lgAudARead: rpc call failed\n");
    return 0x80000000u;
}
}

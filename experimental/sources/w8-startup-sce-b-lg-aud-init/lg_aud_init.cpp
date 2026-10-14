// Observed lgAudInit at 0x002e9098, 1204 bytes. Arguments are an optional
// allocator and an optional release function. All of this belongs to the audio
// library the image links in (its build string reads "liblgaud version 1.09.004").
//
// Sequence:
//  1. sceSifInitRpc(0). The uncached alias (address | 0x20000000) of the 64-byte
//     buffer at 0x008ea940 is stored in the word at 0x00447f78 and cleared, and
//     the word at 0x008ea938 is cleared.
//  2. sceSifBindRpc binds the 64-byte client at 0x008ea780 to server 0x50494c42.
//     A negative result prints "rpc binding error" and "system halted." and
//     stops in a loop. A zero word at client offset 36 counts down from
//     0x00010000 to -1 and binds again.
//  3. Operation 16 runs on that client with the 376-byte block at 0x008ea7c0 as
//     send (32 bytes) and receive (48 bytes) after its word 2 is cleared. A
//     nonzero result prints "rpc error" and "system halted." and stops.
//  4. Reply word 2 is the IOP-side version. Exactly 0x01090400 passes. A value
//     whose high half is 0x0109 prints a mismatch warning with both versions
//     (library 1.9.4, then the reply split into bytes 3, 2 and 1) and
//     "continuing."; anything else prints "wrong version" with both versions
//     and "system halted." and stops.
//  5. A null allocator becomes __wrap_malloc and a null release function becomes
//     __wrap_free. They are stored at 0x00447f80 and 0x00447f84.
//  6. Reply word 8 is the size the IOP side wants. When a buffer from an earlier
//     call exists (word 0x00447f70) and the size recorded at 0x00447f68 differs,
//     it is released and cleared; a matching one is kept untouched. With no
//     buffer, a size of at most 368 uses the receive block itself: the aligned
//     pointer (0x00447f74) is that block, the recorded size is 368 and 0x00447f6c
//     is 320. A larger size allocates ((size + 63) & -64) + 64 bytes; failure
//     prints "unable to allocate %d bytes!" and "system halted." and stops.
//     On success the aligned pointer is the allocation rounded up to 64, the
//     recorded size is the reply word and 0x00447f6c is that word minus 48.
//  7. A semaphore id of -1 in the word at 0x00447f7c creates one (24-byte
//     parameter block cleared, words 1 and 2 set to 1); getting -1 back prints
//     "unable to create semaphore!" and "system halted." and stops.
//  8. The 64 bytes at 0x008ea940 are invalidated in the data cache, receive-block
//     word 8 is set to 0x008ea940 and operation 23 runs with a 48-byte send, no
//     receive and no completion function. The build string (0x00447f88) and
//     0x00447f6c are printed with "max streaming data size", and 0 is returned.
//
// The statics are anonymous in the original; they are published here under
// authored names so later audio calls can share them. The roles are inferred
// from this function alone.
extern "C" {
int printf(const char* format, ...);
int sceSifBindRpc(void* client, unsigned server, int mode);
int sceSifCallRpc(void* client, int operation, int mode, void* send, int send_size,
                  void* receive, int receive_size, void* completion,
                  void* completion_argument);
void sceSifInitRpc(int mode);
int CreateSema(void* parameters);
void InvalidDCache(void* start, void* end);
void* memset(void* destination, int value, __SIZE_TYPE__ count);
void* __wrap_malloc(__SIZE_TYPE__ size);
void __wrap_free(void* pointer);
#if !defined(__mips__)
int* norrath_lgaud_host_uncached(void* address);
#endif

alignas(64) unsigned char norrath_lgaud_client[64];
alignas(64) unsigned norrath_lgaud_rpc_block[94];
int norrath_lgaud_word_after_block;
alignas(64) unsigned char norrath_lgaud_stream_buffer[64];

int norrath_lgaud_recorded_size;
int norrath_lgaud_data_size = 0;
unsigned char* norrath_lgaud_allocation;
unsigned char* norrath_lgaud_aligned_buffer;
int* norrath_lgaud_uncached_word;
int norrath_lgaud_semaphore = -1;
void* (*norrath_lgaud_allocator)(__SIZE_TYPE__);
void (*norrath_lgaud_release)(void*);
const char* norrath_lgaud_build_string =
    "liblgaud version 1.09.004, built on Sep 15 2003 at 18:00:05";

static int* uncached_alias(void* address) {
#if defined(__mips__)
    return reinterpret_cast<int*>(reinterpret_cast<__UINTPTR_TYPE__>(address) | 0x20000000u);
#else
    return norrath_lgaud_host_uncached(address);
#endif
}

static void halt() {
    printf("lgAudInit: system halted.\n");
    for (;;) __asm__ volatile("");
}

static void print_versions(unsigned irx_version) {
    printf("lgAudInit:  liblgaud.a is version %d.%02d.%03d\n", 1, 9, 4);
    const int signed_version = static_cast<int>(irx_version);
    printf("lgAudInit:  lgaud.irx is version %d.%02d.%03d\n", signed_version >> 24,
           (signed_version >> 16) & 0xff, (signed_version >> 8) & 0xff);
}

int lgAudInit(void* (*allocator)(__SIZE_TYPE__), void (*release)(void*)) {
    sceSifInitRpc(0);
    norrath_lgaud_uncached_word = uncached_alias(norrath_lgaud_stream_buffer);
    *norrath_lgaud_uncached_word = 0;
    norrath_lgaud_word_after_block = 0;

    for (;;) {
        if (sceSifBindRpc(norrath_lgaud_client, 0x50494c42u, 0) < 0) {
            printf("lgAudInit: rpc binding error\n");
            halt();
        }
        if (reinterpret_cast<unsigned*>(norrath_lgaud_client)[9] != 0) break;
        volatile int remaining = 0x00010000;
        do {
            remaining = remaining - 1;
        } while (remaining != -1);
    }

    norrath_lgaud_rpc_block[2] = 0;
    if (sceSifCallRpc(norrath_lgaud_client, 16, 0, norrath_lgaud_rpc_block, 32,
                      norrath_lgaud_rpc_block, 48, 0, 0) != 0) {
        printf("lgAudInit: rpc error\n");
        halt();
    }
    const unsigned version = norrath_lgaud_rpc_block[2];
    if (version != 0x01090400u) {
        if ((version & 0xffff0000u) == 0x01090000u) {
            printf("lgAudInit: warning: version mismatch between lgdev.irx and liblgdev.a \n");
            print_versions(norrath_lgaud_rpc_block[2]);
            printf("lgAudInit: continuing.\n");
        } else {
            printf("lgAudInit: wrong version of lgaud.irx!\n");
            print_versions(norrath_lgaud_rpc_block[2]);
            halt();
        }
    }

    if (allocator == 0) allocator = __wrap_malloc;
    if (release == 0) release = __wrap_free;
    norrath_lgaud_allocator = allocator;
    norrath_lgaud_release = release;

    unsigned char* kept = norrath_lgaud_allocation;
    if (kept != 0) {
        if (norrath_lgaud_recorded_size != static_cast<int>(norrath_lgaud_rpc_block[8])) {
            release(kept);
            norrath_lgaud_allocation = 0;
            kept = 0;
        }
    }
    if (kept == 0) {
        const unsigned wanted = norrath_lgaud_rpc_block[8];
        if (wanted <= 368) {
            norrath_lgaud_aligned_buffer = reinterpret_cast<unsigned char*>(norrath_lgaud_rpc_block);
            norrath_lgaud_recorded_size = 368;
            norrath_lgaud_data_size = 320;
        } else {
            const unsigned size = ((wanted + 63) & ~63u) + 64;
            unsigned char* block = static_cast<unsigned char*>(allocator(size));
            norrath_lgaud_allocation = block;
            if (block == 0) {
                printf("lgAudInit: unable to allocate %d bytes!\n", size);
                halt();
            }
            const unsigned reply_size = norrath_lgaud_rpc_block[8];
            norrath_lgaud_aligned_buffer = reinterpret_cast<unsigned char*>(
                (reinterpret_cast<__UINTPTR_TYPE__>(norrath_lgaud_allocation) + 63) & ~static_cast<__UINTPTR_TYPE__>(63));
            norrath_lgaud_recorded_size = static_cast<int>(reply_size);
            norrath_lgaud_data_size = static_cast<int>(reply_size) - 48;
        }
    }

    if (norrath_lgaud_semaphore == -1) {
        unsigned parameters[6];
        memset(parameters, 0, 24);
        parameters[1] = 1;
        parameters[2] = 1;
        parameters[5] = 0;
        norrath_lgaud_semaphore = CreateSema(parameters);
        if (norrath_lgaud_semaphore == -1) {
            printf("lgAudInit: unable to create semaphore!\n");
            halt();
        }
    }

    InvalidDCache(norrath_lgaud_stream_buffer, norrath_lgaud_stream_buffer + 63);
    norrath_lgaud_rpc_block[8] = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(norrath_lgaud_stream_buffer));
    sceSifCallRpc(norrath_lgaud_client, 23, 0, norrath_lgaud_rpc_block, 48,
                  norrath_lgaud_rpc_block, 0, 0, 0);
    printf("%s\nlgAudInit: max streaming data size: %d\n", norrath_lgaud_build_string,
           norrath_lgaud_data_size);
    return 0;
}
}

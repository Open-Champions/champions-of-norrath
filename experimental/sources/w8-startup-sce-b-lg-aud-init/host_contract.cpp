#include <csetjmp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define printf norrath_host_printf
#include "lg_aud_init.cpp"
#undef printf

static std::jmp_buf escape;
static bool halted;
static char text[16][160];
static int text_count;

static int bind_calls, bind_answer_at, bind_result;
static int call_count;
static int ops[8];
static unsigned reply_version, reply_size;
static int call16_result;
static unsigned op16_send_size, op16_receive_size, op23_send_size, op23_receive_size;
static unsigned op23_word8;
static void* op16_block;
static int semaphore_result, semaphore_calls;
static unsigned semaphore_params[6];
static void* invalidated_start;
static void* invalidated_end;
static int wrap_malloc_calls, wrap_free_calls, custom_alloc_calls, custom_free_calls;
static unsigned last_alloc_size;
static void* free_arg;
static unsigned char arena[4096];
static int alloc_fails;
static int uncached_cell;
static int init_rpc_calls;

extern "C" int norrath_host_printf(const char* format, ...) {
    __builtin_va_list ap;
    __builtin_va_start(ap, format);
    std::vsnprintf(text[text_count], 160, format, ap);
    __builtin_va_end(ap);
    const bool stop = std::strstr(text[text_count], "system halted") != 0;
    ++text_count;
    if (stop) {
        halted = true;
        std::longjmp(escape, 1);
    }
    return 0;
}
extern "C" void sceSifInitRpc(int) { ++init_rpc_calls; }
extern "C" int sceSifBindRpc(void* client, unsigned server, int mode) {
    ++bind_calls;
    if (server != 0x50494c42u || mode != 0 || client != norrath_lgaud_client) std::exit(40);
    if (bind_calls >= bind_answer_at) reinterpret_cast<unsigned*>(client)[9] = 1;
    return bind_result;
}
extern "C" int sceSifCallRpc(void* client, int operation, int mode, void* send, int send_size,
                             void* receive, int receive_size, void* completion, void* completion_argument) {
    if (client != norrath_lgaud_client || mode != 0 || completion != 0 || completion_argument != 0) std::exit(41);
    if (send != norrath_lgaud_rpc_block || receive != norrath_lgaud_rpc_block) std::exit(42);
    ops[call_count++] = operation;
    unsigned* block = norrath_lgaud_rpc_block;
    if (operation == 16) {
        op16_block = send; op16_send_size = send_size; op16_receive_size = receive_size;
        if (block[2] != 0) std::exit(43);
        block[2] = reply_version;
        block[8] = reply_size;
        return call16_result;
    }
    op23_send_size = send_size; op23_receive_size = receive_size; op23_word8 = block[8];
    return 0;
}
extern "C" int CreateSema(void* parameters) {
    ++semaphore_calls;
    std::memcpy(semaphore_params, parameters, 24);
    return semaphore_result;
}
extern "C" void InvalidDCache(void* start, void* end) { invalidated_start = start; invalidated_end = end; }
extern "C" void* __wrap_malloc(__SIZE_TYPE__ size) {
    ++wrap_malloc_calls; last_alloc_size = static_cast<unsigned>(size);
    return alloc_fails ? 0 : arena + 1;
}
extern "C" void __wrap_free(void* p) { ++wrap_free_calls; free_arg = p; }
extern "C" int* norrath_lgaud_host_uncached(void* address) {
    if (address != norrath_lgaud_stream_buffer) std::exit(44);
    uncached_cell = 0x7777;
    return &uncached_cell;
}
static void* custom_alloc(__SIZE_TYPE__ size) { ++custom_alloc_calls; last_alloc_size = static_cast<unsigned>(size); return arena + 100; }
static void custom_free(void* p) { ++custom_free_calls; free_arg = p; }

static void reset_all() {
    std::memset(norrath_lgaud_client, 0, sizeof norrath_lgaud_client);
    std::memset(norrath_lgaud_rpc_block, 0, sizeof norrath_lgaud_rpc_block);
    norrath_lgaud_word_after_block = 0x1234;
    norrath_lgaud_recorded_size = 0; norrath_lgaud_data_size = 0;
    norrath_lgaud_allocation = 0; norrath_lgaud_aligned_buffer = 0;
    norrath_lgaud_semaphore = -1;
    norrath_lgaud_allocator = 0; norrath_lgaud_release = 0;
    halted = false; text_count = 0; bind_calls = 0; bind_answer_at = 1; bind_result = 0;
    call_count = 0; reply_version = 0x01090400u; reply_size = 100; call16_result = 0;
    semaphore_result = 9; semaphore_calls = 0; wrap_malloc_calls = wrap_free_calls = 0;
    custom_alloc_calls = custom_free_calls = 0; alloc_fails = 0; init_rpc_calls = 0;
    invalidated_start = invalidated_end = 0;
}

static int run(void* (*alloc)(__SIZE_TYPE__), void (*release)(void*)) {
    if (setjmp(escape) == 0) return lgAudInit(alloc, release);
    return -1;
}

int main() {
    // Small reply, server answers on the third bind, default allocators.
    reset_all();
    bind_answer_at = 3;
    if (run(0, 0) != 0 || halted) return 1;
    if (bind_calls != 3 || init_rpc_calls != 1 || uncached_cell != 0 || norrath_lgaud_word_after_block != 0) return 2;
    if (call_count != 2 || ops[0] != 16 || ops[1] != 23) return 3;
    if (op16_send_size != 32 || op16_receive_size != 48 || op23_send_size != 48 || op23_receive_size != 0) return 4;
    if (norrath_lgaud_allocator != __wrap_malloc || norrath_lgaud_release != __wrap_free) return 5;
    if (norrath_lgaud_allocation != 0 || wrap_malloc_calls != 0) return 6;
    if (norrath_lgaud_aligned_buffer != reinterpret_cast<unsigned char*>(norrath_lgaud_rpc_block)) return 7;
    if (norrath_lgaud_recorded_size != 368 || norrath_lgaud_data_size != 320) return 8;
    if (semaphore_calls != 1 || norrath_lgaud_semaphore != 9) return 9;
    if (semaphore_params[0] != 0 || semaphore_params[1] != 1 || semaphore_params[2] != 1 || semaphore_params[5] != 0) return 10;
    if (invalidated_start != norrath_lgaud_stream_buffer || invalidated_end != norrath_lgaud_stream_buffer + 63) return 11;
    if (op23_word8 != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(norrath_lgaud_stream_buffer))) return 12;
    if (std::strcmp(text[text_count - 1], "liblgaud version 1.09.004, built on Sep 15 2003 at 18:00:05\n"
                                             "lgAudInit: max streaming data size: 320\n") != 0) return 13;

    // The boundary: 368 stays inside the block, 369 allocates.
    reset_all(); reply_size = 368;
    if (run(0, 0) != 0 || wrap_malloc_calls != 0) return 14;
    reset_all(); reply_size = 369;
    if (run(0, 0) != 0 || wrap_malloc_calls != 1 || last_alloc_size != 448 || norrath_lgaud_recorded_size != 369) return 15;
    if (norrath_lgaud_data_size != 321) return 16;

    // Large reply with a custom allocator: size rounding and 64-byte alignment of the result.
    reset_all(); reply_size = 1000;
    if (run(custom_alloc, custom_free) != 0 || custom_alloc_calls != 1 || last_alloc_size != 1088) return 17;
    if (wrap_malloc_calls != 0 || norrath_lgaud_allocation != arena + 100) return 18;
    if ((reinterpret_cast<__UINTPTR_TYPE__>(norrath_lgaud_aligned_buffer) & 63) != 0) return 19;
    if (norrath_lgaud_aligned_buffer < arena + 100 || norrath_lgaud_aligned_buffer >= arena + 100 + 64) return 20;
    if (norrath_lgaud_recorded_size != 1000 || norrath_lgaud_data_size != 952) return 21;

    // Second call, same size: allocation kept, nothing released or allocated, semaphore reused.
    unsigned char* kept = norrath_lgaud_allocation;
    unsigned char* kept_aligned = norrath_lgaud_aligned_buffer;
    call_count = 0; text_count = 0; semaphore_calls = 0; custom_alloc_calls = 0;
    std::memset(norrath_lgaud_rpc_block, 0, sizeof norrath_lgaud_rpc_block);
    if (run(custom_alloc, custom_free) != 0) return 22;
    if (norrath_lgaud_allocation != kept || norrath_lgaud_aligned_buffer != kept_aligned) return 23;
    if (custom_alloc_calls != 0 || custom_free_calls != 0 || semaphore_calls != 0) return 24;

    // Third call, different size: the old allocation is released first.
    std::memset(norrath_lgaud_rpc_block, 0, sizeof norrath_lgaud_rpc_block);
    reply_size = 2000;
    if (run(custom_alloc, custom_free) != 0 || custom_free_calls != 1 || free_arg != kept) return 25;
    if (custom_alloc_calls != 1 || last_alloc_size != 2048 + 64 || norrath_lgaud_recorded_size != 2000) return 26;

    // Different size that shrinks back into the block: released, no new allocation.
    std::memset(norrath_lgaud_rpc_block, 0, sizeof norrath_lgaud_rpc_block);
    custom_alloc_calls = 0; custom_free_calls = 0;
    reply_size = 50;
    if (run(custom_alloc, custom_free) != 0 || custom_free_calls != 1 || custom_alloc_calls != 0) return 27;
    if (norrath_lgaud_allocation != 0 || norrath_lgaud_recorded_size != 368) return 28;

    // Version mismatch with matching high half: warning and continue.
    reset_all(); reply_version = 0x01090203u;
    if (run(0, 0) != 0 || halted || call_count != 2) return 29;
    if (std::strstr(text[0], "warning: version mismatch") == 0) return 30;
    if (std::strcmp(text[1], "lgAudInit:  liblgaud.a is version 1.09.004\n") != 0) return 31;
    if (std::strcmp(text[2], "lgAudInit:  lgaud.irx is version 1.09.002\n") != 0) return 32;
    if (std::strcmp(text[3], "lgAudInit: continuing.\n") != 0) return 33;

    // Wrong version stops after printing both versions; the reply word's sign is shifted arithmetically.
    reset_all(); reply_version = 0x82000105u;
    if (run(0, 0) != -1 || !halted || call_count != 1) return 34;
    if (std::strcmp(text[0], "lgAudInit: wrong version of lgaud.irx!\n") != 0) return 35;
    if (std::strcmp(text[2], "lgAudInit:  lgaud.irx is version -126.00.001\n") != 0) return 36;

    // Bind failure, RPC failure, allocation failure and semaphore failure each stop.
    reset_all(); bind_result = -1;
    if (run(0, 0) != -1 || std::strcmp(text[0], "lgAudInit: rpc binding error\n") != 0 || call_count != 0) return 37;
    reset_all(); call16_result = -1;
    if (run(0, 0) != -1 || std::strcmp(text[0], "lgAudInit: rpc error\n") != 0 || call_count != 1) return 38;
    reset_all(); reply_size = 5000; alloc_fails = 1;
    if (run(0, 0) != -1 || std::strcmp(text[0], "lgAudInit: unable to allocate 5120 bytes!\n") != 0) return 39;
    if (semaphore_calls != 0 || call_count != 1) return 45;
    reset_all(); semaphore_result = -1;
    if (run(0, 0) != -1 || std::strcmp(text[0], "lgAudInit: unable to create semaphore!\n") != 0 || call_count != 1) return 46;
    std::puts("lgAudInit passed");
    return 0;
}

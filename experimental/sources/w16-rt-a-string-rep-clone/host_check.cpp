// Host check (not a plan input). Build (the clone function name is renamed so the object links beside this file):
//   c++ -O1 -std=c++17 -c -DW16_HOST_CHECK -D'clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep=w16_clone' \
//       -Dmemcpy=w16_memcpy -Dexit=w16_exit -o /tmp/w16_rc_src.o rep_clone.cpp
//   c++ -O1 -std=c++17 -o /tmp/w16_rc host_check.cpp /tmp/w16_rc_src.o && /tmp/w16_rc
// Address words are offsets into a host arena. The allocator, the stream routines, the chunk allocator and exit are
// stand-ins that record what they were asked.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

unsigned char* w16_host_arena;
struct Rep { unsigned length, capacity; int references, shared; };
extern "C" char* w16_clone(Rep* source);

extern "C" {
unsigned free_lists[16] __asm__("_t24__default_alloc_template2b0i0$_S_free_list");
void (*oom_handler)() __asm__("_t23__malloc_alloc_template1i0$__malloc_alloc_oom_handler");
char error_stream[8] __asm__("cerr");
unsigned chunk_alloc(unsigned size, int* count) __asm__("_S_chunk_alloc__t24__default_alloc_template2b0i0UiRi");
}

static std::string trace;
static unsigned next_free = 0x40000;
static std::vector<unsigned> malloc_results;
static int chunk_count = 1;
static unsigned chunk_base = 0x20000;
struct Exit { int status; };

extern "C" void* __wrap_malloc(unsigned size) {
    trace += "malloc(" + std::to_string(size) + ");";
    if (!malloc_results.empty()) {
        const unsigned r = malloc_results.front();
        malloc_results.erase(malloc_results.begin());
        return r == 0 ? nullptr : w16_host_arena + r;
    }
    const unsigned r = next_free;
    next_free += 0x400;
    return w16_host_arena + r;
}
unsigned chunk_alloc(unsigned size, int* count) {
    trace += "chunk(" + std::to_string(size) + ");";
    *count = chunk_count;
    return chunk_base;
}
extern "C" void* __ls__7ostreamPCc(void* stream, const char* text) { trace += std::string("ls(") + text + ");"; return stream; }
extern "C" void* endl__FR7ostream(void* stream) { trace += "endl;"; return stream; }
extern "C" void w16_exit(int status) { throw Exit{status}; }
extern "C" void* w16_memcpy(void* to, const void* from, unsigned n) { trace += "memcpy(" + std::to_string(n) + ");"; return std::memcpy(to, from, n); }
static int handler_calls;
static void handler() { ++handler_calls; trace += "handler;"; }

static unsigned word(unsigned off) { return *reinterpret_cast<unsigned*>(w16_host_arena + off); }
static void setword(unsigned off, unsigned v) { *reinterpret_cast<unsigned*>(w16_host_arena + off) = v; }

static unsigned expected_capacity(unsigned length) { unsigned c = 16; while (c < length + 1) c <<= 1; return c; }

int main() {
    w16_host_arena = new unsigned char[1 << 20];
    unsigned cases = 0;
    const unsigned lengths[] = {0, 1, 14, 15, 16, 17, 30, 31, 32, 62, 63, 64, 100, 111, 112, 113, 127, 128, 129, 300};
    for (unsigned length : lengths) {
        for (int mode = 0; mode < 3; ++mode) {  // empty list with chunk counts 1 and 5, then a populated list
            std::memset(w16_host_arena, 0, 1 << 20);
            std::memset(free_lists, 0, sizeof free_lists);
            oom_handler = nullptr;
            trace.clear(); malloc_results.clear(); next_free = 0x40000;
            chunk_count = mode == 1 ? 5 : 1;
            Rep* src = reinterpret_cast<Rep*>(w16_host_arena + 0x1000);
            src->length = length; src->capacity = 0xdead; src->references = 3; src->shared = 1;
            for (unsigned i = 0; i < length; ++i) w16_host_arena[0x1010 + i] = static_cast<unsigned char>(i * 5 + 1);
            const unsigned capacity = expected_capacity(length);
            const unsigned size = capacity + 16;
            const unsigned index = ((capacity + 23) >> 3) - 1;
            if (mode == 2 && size <= 128) { free_lists[index] = 0x30000; setword(0x30000, 0x30100); setword(0x30100, 0); }
            char* result = w16_clone(src);
            const unsigned block = static_cast<unsigned>(result - 16 - reinterpret_cast<char*>(w16_host_arena));
            Rep* rep = reinterpret_cast<Rep*>(w16_host_arena + block);
            if (rep->length != length || rep->capacity != capacity || rep->references != 1 || rep->shared != 0) { std::printf("header len %u mode %d\n", length, mode); return 1; }
            if (length && std::memcmp(result, w16_host_arena + 0x1010, length) != 0) return 2;
            if (src->length != length || src->references != 3) return 3;  // source untouched
            if (size > 128) {
                if (trace != "malloc(" + std::to_string(size) + ");" + (length ? "memcpy(" + std::to_string(length) + ");" : "")) { std::printf("big trace %s\n", trace.c_str()); return 4; }
            } else if (mode == 2) {
                if (block != 0x30000 || free_lists[index] != 0x30100) return 5;
                if (trace.find("chunk") != std::string::npos) return 6;
            } else {
                const unsigned rounded = (capacity + 23) & ~7u;
                if (block != chunk_base || trace.find("chunk(" + std::to_string(rounded) + ");") != 0) { std::printf("chunk trace %s\n", trace.c_str()); return 7; }
                if (chunk_count == 1) {
                    if (free_lists[index] != 0) return 8;
                } else {
                    // blocks 1..count-1 linked in address order, the last link zero
                    unsigned at = free_lists[index];
                    if (at != chunk_base + rounded) return 9;
                    for (int i = 1; i < chunk_count; ++i) {
                        const unsigned next = i == chunk_count - 1 ? 0 : at + rounded;
                        if (word(at) != next) { std::printf("link %d\n", i); return 10; }
                        at = next;
                    }
                }
            }
            ++cases;
        }
    }
    // Large allocation failure: the handler is called and the allocation retried until it succeeds.
    {
        std::memset(free_lists, 0, sizeof free_lists);
        trace.clear(); handler_calls = 0;
        oom_handler = handler;
        malloc_results = {0, 0, 0x50000};
        Rep* src = reinterpret_cast<Rep*>(w16_host_arena + 0x1000);
        src->length = 200;
        char* result = w16_clone(src);
        if (handler_calls != 2 || result != reinterpret_cast<char*>(w16_host_arena + 0x50010)) return 11;
        ++cases;
    }
    // No handler: the message is written and exit(1) ends the call.
    {
        trace.clear();
        oom_handler = nullptr;
        malloc_results = {0};
        Rep* src = reinterpret_cast<Rep*>(w16_host_arena + 0x1000);
        src->length = 200;
        bool exited = false;
        try { w16_clone(src); } catch (const Exit& e) { exited = e.status == 1; }
        if (!exited || trace != "malloc(" + std::to_string(expected_capacity(200) + 16) + ");ls(out of memory);endl;") { std::printf("trace %s\n", trace.c_str()); return 12; }
        ++cases;
    }
    std::printf("%u string-rep clone cases passed\n", cases);
    return 0;
}

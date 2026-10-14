// Host check only; not a build input. Both callees are recorders.
#define NORRATH_HOST_ARENA 1
#include "query_obj.cpp"
#include <cstdint>
static unsigned char arena[0x1000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
extern "C" void* norrath_host_function(unsigned) { return nullptr; }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }
static const unsigned char *t_self, *t_matrix; static float* t_out; static float zero_seen[6]; static int order, t_at, q_at;
static const unsigned char *q_box; static const float* q_local; static unsigned char *q_list, *q_buffer, *q_self; static float q_copy[6];
extern "C" void Transform__C6VIBBoxRC10VIMatrix44R6VIBBox(const unsigned char* s, const unsigned char* m, float* r) {
    t_self = s; t_matrix = m; t_out = r; t_at = ++order;
    for (int i = 0; i < 6; ++i) { zero_seen[i] = r[i]; r[i] = 100.0f + static_cast<float>(i); }
}
extern "C" int Query__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer(unsigned char* s, const unsigned char* b, const float* l, unsigned char* li, unsigned char* bu) {
    q_self = s; q_box = b; q_local = l; q_list = li; q_buffer = bu; q_at = ++order; for (int i = 0; i < 6; ++i) q_copy[i] = l[i];
    return 1234;
}
int main() {
    unsigned char* self = arena; unsigned char* table = arena + 0x800; unsigned char* buffers = arena + 0x300;
    unsigned char* box = arena + 0x500; unsigned char* list = arena + 0x600;
    *reinterpret_cast<unsigned*>(self + 536) = word_of(table);
    for (int k = 0; k < 4; ++k) *reinterpret_cast<unsigned*>(table + 12 * k) = word_of(buffers + 0x20 * k);
    int r = QueryObj__9VICollideRC6VIBBoxR14VIQueryTriListi(self, box, list, 3);
    if (r != 1234) return 1;
    if (t_self != box || t_matrix != self + 80 || t_at != 1 || q_at != 2) return 2;
    for (int i = 0; i < 6; ++i) if (zero_seen[i] != 0.0f) return 3;
    if (q_self != self || q_box != box || q_local != t_out || q_list != list || q_buffer != buffers + 0x60) return 4;
    for (int i = 0; i < 6; ++i) if (q_copy[i] != 100.0f + static_cast<float>(i)) return 5;
    return 0;
}

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
static void host_callback(void* arg);
#define NORRATH_WORD_ADDRESS(word) ((word) == 1 ? reinterpret_cast<void*>(host_callback) : host_address(word))
#include "parse_xm_obj.cpp"

static std::string tr; static unsigned id_in; static int find_ret, find_type, find_handle, end_rets[4], end_calls, sizes[2], size_calls, begin_calls, vec_sizes[2], vec_calls, first_word;
static unsigned short tags[2], second_in; static void* cb_arg; static void* buffers[2]; static void* deleted; static int read_sizes[2], read_calls; static int floats;
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tags[begin_calls++]; *second = second_in; tr += "B;"; return 0; }
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = id_in; tr += "U;"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* v) { *v = 0; ++floats; tr += "F;"; return 0; }
extern "C" int Read__9VIObjFilePUci(void*, unsigned char* b, int n) { if (b != buffers[read_calls]) tr += "BADBUF"; if (read_calls == 0) memcpy(b, &first_word, 4); read_sizes[read_calls++] = n; tr += "R;"; return 0; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_rets[end_calls++]; }
extern "C" int ObjectSize__9VIObjFile(void*) { tr += "Z;"; return sizes[size_calls++]; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* d, unsigned id, int* t, int* h) {
    if (d != reinterpret_cast<void*>(0x30) || id != id_in) tr += "BADFIND"; *t = find_type; *h = find_handle; tr += "Find;"; return find_ret; }
extern "C" void* __builtin_vec_new(unsigned n) { vec_sizes[vec_calls] = static_cast<int>(n); buffers[vec_calls] = calloc(n ? n : 1, 1); tr += "N;"; return buffers[vec_calls++]; }
extern "C" void __builtin_vec_delete(void* p) { deleted = p; tr += "D;"; }
static void host_callback(void* a) { cb_arg = a; tr += "cb;"; }

static int run(unsigned cb) {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[1] = cb; w[2] = 0x1234; w[8] = 0x30; w[9] = 0x55; tr.clear(); end_calls = 0; begin_calls = 0; size_calls = 0; vec_calls = 0; read_calls = 0; deleted = nullptr; cb_arg = nullptr; floats = 0;
    return ParseXmObj__10VIESFParse(parse);
}
int main() {
    id_in = 7; find_ret = 0; find_type = 14; find_handle = 11; for (int& e : end_rets) e = 0; tags[0] = 0xb040; tags[1] = 0xb060; second_in = 0;
    sizes[0] = 100; sizes[1] = 40; first_word = 0;
    if (run(0) != 11 || tr != "B;U;Find;Z;N;R;E;B;Z;N;R;E;E;" || vec_sizes[0] != 88 || vec_sizes[1] != 40 || read_sizes[0] != 88 || read_sizes[1] != 40 || deleted) return 1;
    second_in = 9; if (run(1) != 11 || floats != 2 || cb_arg != reinterpret_cast<void*>(0x1234) || tr.substr(0, 9) != "cb;B;U;F;") return 2;
    second_in = 0; find_ret = 1; if (run(0) != 11 || tr != "B;U;Find;E;E;") return 3;
    find_type = 3; if (run(0) != -1 || tr != "B;U;Find;") return 4;
    find_type = 14; find_ret = 0; first_word = 5; if (run(0) != -1 || tr != "B;U;Find;Z;N;R;E;D;" || deleted != buffers[0]) return 5;
    first_word = 0; end_rets[0] = -1; if (run(0) != -1 || tr != "B;U;Find;Z;N;R;E;") return 6;
    end_rets[0] = 0; tags[1] = 0xb061; if (run(0) != -1 || tr != "B;U;Find;Z;N;R;E;B;") return 7;
    tags[1] = 0xb060; end_rets[1] = -1; if (run(0) != -1) return 8;
    end_rets[1] = 0; end_rets[2] = -1; if (run(0) != -1) return 9;
    end_rets[2] = 0; tags[0] = 0xb041; if (run(0) != -1 || tr != "B;E;") return 10;
    tags[0] = 0xb040; id_in = 0; if (run(0) != 0 || tr.find("Find") != std::string::npos) return 11;
    return 0;
}

#include <stdint.h>
#include <stdlib.h>
#include <string>
static void* host_address(unsigned word) { return reinterpret_cast<void*>(static_cast<uintptr_t>(word)); }
static void host_callback(void* arg);

#define NORRATH_WORD_ADDRESS(word) ((word) == 1 ? reinterpret_cast<void*>(host_callback) : host_address(word))
#include "parse_adpcm_obj.cpp"

static std::string tr; static unsigned id_in; static int find_ret, find_type, find_handle, end_rets[4], end_calls; static unsigned short tags[2];
static int ints[4]; static int int_calls, begin_calls, buf_size, read_size; static void* cb_arg; static void* buffer_seen; static void* deleted;
extern "C" int ReadBegin__9VIObjFileRUsT1(void* f, unsigned short* tag, unsigned short* second) {
    if (f != reinterpret_cast<void*>(0x55)) tr += "BADFILE"; *tag = tags[begin_calls++]; *second = 0; tr += "B;"; return 0; }
extern "C" int Read__9VIObjFileRUi(void*, unsigned* v) { *v = id_in; tr += "U;"; return 0; }
extern "C" int Read__9VIObjFileRi(void*, int* v) { *v = ints[int_calls++ & 3]; tr += "I;"; return 0; }
extern "C" int Read__9VIObjFileRf(void*, float* v) { *v = 0; tr += "F;"; return 0; }
extern "C" int Read__9VIObjFilePUci(void*, unsigned char* b, int n) { if (b != buffer_seen) tr += "BADBUF"; read_size = n; tr += "R;"; return 0; }
extern "C" int ReadEnd__9VIObjFile(void*) { tr += "E;"; return end_rets[end_calls++]; }
extern "C" int Find__12VIDictionaryUiR14VIResourceTypeRi(void* d, unsigned id, int* t, int* h) {
    if (d != reinterpret_cast<void*>(0x30) || id != id_in) tr += "BADFIND"; *t = find_type; *h = find_handle; tr += "Find;"; return find_ret; }
extern "C" void* __builtin_vec_new(unsigned n) { buf_size = static_cast<int>(n); buffer_seen = malloc(n ? n : 1); tr += "N;"; return buffer_seen; }
extern "C" void __builtin_vec_delete(void* p) { deleted = p; tr += "D;"; free(p); }
static void host_callback(void* a) { cb_arg = a; tr += "cb;"; }

static int run(unsigned cb) {
    alignas(8) unsigned char parse[64] = {0}; unsigned* w = reinterpret_cast<unsigned*>(parse);
    w[1] = cb; w[2] = 0x1234; w[8] = 0x30; w[9] = 0x55; tr.clear(); end_calls = 0; int_calls = 0; begin_calls = 0; deleted = nullptr; cb_arg = nullptr;
    return ParseAdpcmObj__10VIESFParse(parse);
}
int main() {
    id_in = 7; find_ret = 0; find_type = 14; find_handle = 11; for (int& e : end_rets) e = 0; tags[0] = 0xb010; tags[1] = 0xb020;
    ints[0] = 3; ints[1] = 5; ints[2] = 0; ints[3] = 0;
    // no dictionary hit: second header, (a * b) * 16 byte buffer, read, freed, two ReadEnd
    if (run(0) != 11 || tr != "B;U;I;I;I;F;F;I;E;Find;B;N;R;D;E;E;" || buf_size != 240 || read_size != 240 || deleted != buffer_seen) return 1;
    if (run(1) != 11 || cb_arg != reinterpret_cast<void*>(0x1234) || tr.substr(0, 5) != "cb;B;") return 2;
    find_ret = 1; if (run(0) != 11 || tr != "B;U;I;I;I;F;F;I;E;Find;E;") return 3;
    find_type = 3; if (run(0) != -1 || tr != "B;U;I;I;I;F;F;I;E;Find;") return 4;
    find_type = 14; end_rets[0] = -1; if (run(0) != -1 || tr != "B;U;I;I;I;F;F;I;E;") return 5;
    end_rets[0] = 0; find_ret = 0; tags[1] = 0xb021; if (run(0) != -1 || tr != "B;U;I;I;I;F;F;I;E;Find;B;E;") return 6;
    tags[1] = 0xb020; end_rets[1] = -1; if (run(0) != -1 || tr.substr(tr.size() - 4) != "D;E;") return 7;
    end_rets[1] = 0; end_rets[2] = -1; if (run(0) != -1) return 8;
    end_rets[2] = 0; tags[0] = 0xb011; if (run(0) != -1 || tr != "B;E;") return 9;
    tags[0] = 0xb010; id_in = 0; if (run(0) != 0 || tr.find("Find") != std::string::npos) return 10;
    return 0;
}

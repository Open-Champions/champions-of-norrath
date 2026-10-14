// Host check only; not a build input. Node, element and table words are tags translated to host objects. The list callees are scripted
// over three nodes whose element words are file 1, file 2 and null; each file's table has its destructor in the second entry.
#define NORRATH_HOST_CONTRACT 1
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char vt_VIFlatFileDirectory[0x18] __asm__("_vt$19VIFlatFileDirectory");
unsigned char vt_VIFlatFile[0x18] __asm__("_vt$10VIFlatFile");
static unsigned object[40];
static unsigned node1[2], node2[2], node3[2];
static unsigned char file1[64], file2[64];
static VtEntry table[4];
static unsigned char buf0[8], buf1[8];
static std::string trace;
static const unsigned N1 = 0x100, N2 = 0x200, N3 = 0x300, F1 = 0x1000, F2 = 0x2000, TBL = 0x3000, B0 = 0x4000, B1 = 0x5000;
extern "C" char* norrath_host_translate(unsigned a) {
    switch (a) {
    case N1: return reinterpret_cast<char*>(node1);
    case N2: return reinterpret_cast<char*>(node2);
    case N3: return reinterpret_cast<char*>(node3);
    case F1: return reinterpret_cast<char*>(file1);
    case F2: return reinterpret_cast<char*>(file2);
    case TBL: return reinterpret_cast<char*>(table);
    case B0: return reinterpret_cast<char*>(buf0);
    case B1: return reinterpret_cast<char*>(buf1);
    default: return nullptr;
    }
}
static std::string name(const void* p) {
    if (p == buf0) return "b0"; if (p == buf1) return "b1"; if (p == object) return "obj";
    if (p == object + 6) return "list"; return "?";
}
extern "C" int Begin__Ct6VIList1ZP10VIFlatFile(const void* l) { trace += "begin:" + name(l) + ";"; return N1; }
extern "C" int End__Ct6VIList1ZP10VIFlatFile(const void* l) { trace += "end:" + name(l) + ";"; return 0x7fffffff; }
extern "C" int Erase__t6VIList1ZP10VIFlatFilei(void* l, int n) {
    trace += "erase:" + name(l) + "," + std::to_string(n) + ";";
    return n == (int)N1 ? N2 : n == (int)N2 ? N3 : 0x7fffffff;
}
extern "C" void _$_t6VIList1ZP10VIFlatFile(void* l, int f) { trace += "listdtor:" + name(l) + "," + std::to_string(f) + ";"; }
extern "C" void __wrap_free(void* p) { trace += "free:" + name(p) + ";"; }
extern "C" void __builtin_delete(void* p) { trace += "delete:" + name(p) + ";"; }
static void file_dtor(void* self, int flags) {
    trace += "file:" + std::string(self == file1 + 8 ? "f1" : self == file2 + 8 ? "f2" : "?") + "," + std::to_string(flags) + ";";
}
static void wrong(void*, int) { trace += "wrong-entry;"; }
int main() {
    bool ok = true;
    const unsigned vtd = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_VIFlatFileDirectory));
    const unsigned vtf = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_VIFlatFile));
    table[0] = {0, 0, wrong}; table[1] = {8, 0, file_dtor}; table[2] = {0, 0, wrong};
    for (unsigned has0 : {0u, 1u})
        for (unsigned has1 : {0u, 1u})
            for (int flags : {0, 1, 2, 3, -1}) {
                trace.clear();
                std::memset(object, 0x5a, sizeof object);
                node1[0] = F1; node2[0] = F2; node3[0] = 0;
                *reinterpret_cast<unsigned*>(file1 + 16) = TBL; *reinterpret_cast<unsigned*>(file2 + 16) = TBL;
                object[0] = has0 ? B0 : 0; object[1] = has1 ? B1 : 0;
                _$_19VIFlatFileDirectory(object, flags);
                std::string want = "begin:list;end:list;erase:list,256;file:f1,3;end:list;erase:list,512;file:f2,3;end:list;erase:list,768;end:list;listdtor:list,2;";
                if (has0) want += "free:b0;";
                if (has1) want += "free:b1;";
                if (flags & 1) want += "delete:obj;";
                if (trace != want) { std::puts(trace.c_str()); ok = false; }
                if (object[4] != vtf || object[0] != 0 || object[1] != 0 || object[2] != 0xffffffffu || object[3] != 0) ok = false;
                for (int i = 5; i < 40; ++i) if (object[i] != 0x5a5a5a5au) ok = false;
            }
    (void)vtd;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}

// Host check only; not a build input. It exercises the existing source
// ../w15-vi-strings-dict-a-vistringtable-loadrawfile/loadrawfile.cpp, which has no committed check of its own.
#include <string>
#include <string.h>
#include "../w15-vi-strings-dict-a-vistringtable-loadrawfile/loadrawfile.cpp"
extern "C" {
const char* __VIAssertf_file; int __VIAssertf_line;
}
static std::string tr; static int openRet, initRet, readRet, seekRet[2], seekCalls; static long long tellRet;
static int assertfCond; static std::string assertfFormat; static int assertLine;
extern "C" void __6VIFile(void*) { tr += "c;"; }
extern "C" void _$_6VIFile(void*, int flags) { tr += flags == 2 ? "d;" : "d?;"; }
extern "C" int Open__6VIFilePCci(void*, const char* name, int mode) { tr += std::string("o:") + name + ":" + std::to_string(mode) + ";"; return openRet; }
extern "C" int Seek__6VIFilelQ26VIFile10SeekOrigin(void*, long off, int origin) { tr += "s" + std::to_string(off) + "," + std::to_string(origin) + ";"; return seekRet[seekCalls++ < 1 ? 0 : 1]; }
extern "C" long long Tell__6VIFile(void*) { tr += "t;"; return tellRet; }
extern "C" int Read__6VIFilePvi(void*, void* dst, int n) { tr += "r" + std::to_string(n) + (dst == reinterpret_cast<void*>(0x1234) ? "ok;" : "?;"); return readRet; }
extern "C" int Init__t7VIArray1Zci(void*, int n) { tr += "i" + std::to_string(n) + ";"; return initRet; }
extern "C" void _VIAssertf(int cond, const char* fmt, ...) { assertfCond = cond; assertfFormat = fmt; tr += "A;"; }
extern "C" void _VIAssert(const char* expr, const char*, int line) { assertLine = line; tr += std::string("X:") + expr + ";"; }
static int run(unsigned char* array) { tr.clear(); seekCalls = 0; return LoadRawFile__13VIStringTablePCcRt7VIArray1Zc("f.txt", array); }
int main() {
    alignas(8) unsigned char array[16] = {0};
    void* data = reinterpret_cast<void*>(0x1234);
    memcpy(array + 4, &data, sizeof data);
    openRet = 0; initRet = 0; readRet = 0; seekRet[0] = 0; seekRet[1] = 0; tellRet = 100;
    if (run(array) != 0 || tr != "c;o:f.txt:33;A;s0,2;t;t;i100;s0,1;r100ok;d;") return 1;
    if (assertfCond != 1 || assertfFormat != "file open failed: %s" || __VIAssertf_line != 245) return 2;
    openRet = -1; run(array);
    if (assertfCond != 0) return 3;
    openRet = 0;
    seekRet[0] = 5; if (run(array) != 5 || tr != "c;o:f.txt:33;A;s0,2;d;") return 4;
    seekRet[0] = 0; tellRet = 0x100000000LL;
    run(array); if (tr.find("X:File.Tell() <= 0xFFFFFFFF;") == std::string::npos || assertLine != 253) return 5;
    tellRet = 0xffffffffLL; run(array); if (tr.find("X:") != std::string::npos) return 6;
    tellRet = 0; if (run(array) != 0 || tr != "c;o:f.txt:33;A;s0,2;t;t;i0;d;") return 7;
    tellRet = 8; initRet = -2; if (run(array) != -2 || tr != "c;o:f.txt:33;A;s0,2;t;t;i8;d;") return 8;
    initRet = 0; seekRet[1] = 3; if (run(array) != 3 || tr != "c;o:f.txt:33;A;s0,2;t;t;i8;s0,1;d;") return 9;
    seekRet[1] = 0; readRet = -1; if (run(array) != -1) return 10;
    return 0;
}

// Host contract for LoadSRSServer__Fv. Not an EE plan input. A fake VIFile feeds known bytes;
// the stubs record the call order. memset/strchr/printf are the host's own.
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <string>
#include "load_srs_server.cpp"
static std::string trace, printed, last_path;
static int open_result, tell_result; static const char* content; static int content_len;
static int last_read_count, last_mode; static void* file_seen;
extern "C" void __6VIFile(void* f) { file_seen = f; trace += "ctor;"; }
extern "C" void _$_6VIFile(void* f, int flags) { trace += f == file_seen ? "dtor" : "dtor?"; trace += std::to_string(flags) + ";"; }
extern "C" int Open__6VIFilePCci(void*, const char* p, int m) { last_path = p; last_mode = m; trace += "open;"; return open_result; }
extern "C" int Seek__6VIFilelQ26VIFile10SeekOrigin(void*, long off, int o) { trace += "seek" + std::to_string(off) + "," + std::to_string(o) + ";"; return 0; }
extern "C" long Tell__6VIFile(void*) { trace += "tell;"; return tell_result; }
extern "C" int Read__6VIFilePvi(void*, void* d, int n) { trace += "read;"; last_read_count = n; std::memcpy(d, content, content_len); return content_len; }
extern "C" void Close__6VIFile(void*) { trace += "close;"; }
int main() {
    // success: text with a line feed and a carriage return; the first one cuts
    open_result = 0; tell_result = 40; content = "alpha-server\r\nsecond line\n"; content_len = 26;
    char* r = LoadSRSServer__Fv();
    if (!r || std::strcmp(r, "alpha-server") != 0) return 1;
    if (trace != "ctor;open;seek0,2;tell;seek0,1;read;close;dtor2;") return 2;
    if (last_path != "data/ui/srs.txt" || last_mode != 33 || last_read_count != 512) return 3;
    char* again = LoadSRSServer__Fv();
    if (again != r) return 4;                                   // same static buffer
    // line feed before the carriage return
    trace.clear(); content = "beta\nrest\rmore"; content_len = 14;
    r = LoadSRSServer__Fv();
    if (std::strcmp(r, "beta") != 0) return 5;
    // only a carriage return
    content = "gamma\rrest"; content_len = 10;
    r = LoadSRSServer__Fv();
    if (std::strcmp(r, "gamma") != 0) return 6;
    // the buffer is cleared before reading: stale bytes beyond a shorter file do not survive
    content = "x"; content_len = 1;
    r = LoadSRSServer__Fv();
    if (std::strcmp(r, "x") != 0 || r[1] != 0 || r[5] != 0 || r[511] != 0) return 7;
    // open failure
    trace.clear(); open_result = -1;
    if (LoadSRSServer__Fv() != 0 || trace != "ctor;open;dtor2;") return 8;
    // empty / unreadable size
    trace.clear(); open_result = 0; tell_result = 0;
    if (LoadSRSServer__Fv() != 0 || trace != "ctor;open;seek0,2;tell;dtor2;") return 9;
    tell_result = -4; trace.clear();
    if (LoadSRSServer__Fv() != 0) return 10;
    tell_result = static_cast<int>(0x80000000u);               // sign matters
    if (LoadSRSServer__Fv() != 0) return 11;
    std::puts("load_srs_server cases passed");
    return 0;
}

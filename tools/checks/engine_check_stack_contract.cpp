#include <cstring>
#include <string>

#define NORRATH_HOST_CONTRACT 1
#include "../../experimental/sources/engine-init-startup/check_stack.cpp"

static int stop_code;
static int prints;
static int zero_stores;
static int loads;
static unsigned char bytes[5121];
static const char* seen_site;
static int seen_line;
static std::string seen_text;

static void fail(int code) {
    stop_code = code;
}

extern "C" unsigned char norrath_host_load8(unsigned address) {
    ++loads;
    if (address < 0x01fc0000u || address > 0x01fc0000u + 5120u) return 0;
    return bytes[address - 0x01fc0000u];
}

extern "C" void norrath_host_store32(unsigned address, unsigned value) {
    if (address == 0u && value == 0u) ++zero_stores;
    else fail(2);
}

extern "C" void scePrintf(const char* text, const char* site, int line) {
    seen_text = text ? text : "";
    seen_site = site;
    seen_line = line;
    ++prints;
}

static void fill(unsigned char value) {
    for (unsigned i = 0; i < 5121u; ++i) bytes[i] = value;
}

static void reset() {
    prints = 0;
    zero_stores = 0;
    loads = 0;
    seen_site = nullptr;
    seen_line = -1;
    seen_text.clear();
    stop_code = 0;
}

int main() {
    const char* site = "engine.cc";
    fill(0xa5u);
    bytes[5120] = 0;
    reset();
    _engineCheckStack__FPCci(site, 1075);
    if (stop_code != 0 || prints != 0 || zero_stores != 0 || loads != 5120) return 1;

    fill(0xa5u);
    bytes[0] = 0;
    reset();
    _engineCheckStack__FPCci(site, 1075);
    if (stop_code != 0 || prints != 1 || zero_stores != 1) return 2;
    if (seen_text != "stack error!  %s:%d\n" || seen_site != site || seen_line != 1075) return 3;
    if (loads != 5120) return 4;

    fill(0xa5u);
    bytes[10] = 1;
    bytes[5119] = 2;
    reset();
    const char* other = "other.cc";
    _engineCheckStack__FPCci(other, 42);
    if (prints != 2 || zero_stores != 2 || seen_site != other || seen_line != 42) return 5;
    if (seen_text != "stack error!  %s:%d\n") return 6;
    return 0;
}

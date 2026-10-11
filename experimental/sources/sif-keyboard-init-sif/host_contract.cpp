#include "init_sif.cpp"

#include <cstdarg>
#include <cstdio>

extern "C" unsigned char _ctype_[258] = {};

static int reboot_results[4];
static int reboot_count;
static int sync_results[4];
static int sync_count;
static int fail_at_load;
static int load_count;
static char sequence[160];
static int sequence_used;

struct SeenLoad {
    char path[80];
    int argument_length;
    const void* argument;
    unsigned char bytes[96];
};

static SeenLoad loads[24];
static int sprintf_count;
static int sprintf_value;
static char sprintf_formats[24][40];
static char sprintf_names[24][32];
static int exit_status;
static int exit_calls;
static int power_calls;
static int text_calls;
static int comm_calls;
static int heap_calls;
static const char* reboot_image;

static void mark(char step) {
    if (sequence_used < 159)
        sequence[sequence_used++] = step;
    sequence[sequence_used] = 0;
}

static int same(const char* left, const char* right) {
    int index = 0;
    for (; left[index] != 0 || right[index] != 0; ++index) {
        if (left[index] != right[index])
            return 0;
    }
    return 1;
}

static void copy_text(char* destination, int limit, const char* source) {
    int index = 0;
    for (; index < limit - 1 && source[index] != 0; ++index)
        destination[index] = source[index];
    destination[index] = 0;
}

static int decimal(char* destination, int value) {
    char digits[16];
    int count = 0;
    int working = value;
    if (working == 0)
        digits[count++] = '0';
    while (working > 0) {
        digits[count++] = static_cast<char>('0' + (working % 10));
        working /= 10;
    }
    int out = 0;
    for (int index = count - 1; index >= 0; --index)
        destination[out++] = digits[index];
    destination[out] = 0;
    return out;
}

extern "C" void sceSifInitRpc(int mode) {
    mark('I');
    if (mode != 0)
        mark('!');
}
extern "C" int sceCdInit(int mode) {
    mark('C');
    if (mode != 0)
        mark('!');
    return 0;
}
extern "C" int sceSifRebootIop(const char* image) {
    mark('R');
    reboot_image = image;
    const int result = reboot_results[reboot_count < 4 ? reboot_count : 3];
    ++reboot_count;
    return result;
}
extern "C" int sceSifSyncIop() {
    mark('Y');
    const int result = sync_results[sync_count < 4 ? sync_count : 3];
    ++sync_count;
    return result;
}
extern "C" void sceSifLoadFileReset() { mark('F'); }
extern "C" void sceFsReset() { mark('S'); }
extern "C" int sceCdMmode(int mode) {
    mark('M');
    if (mode != 2)
        mark('!');
    return 0;
}
extern "C" int sprintf(char* destination, const char* format, ...) {
    mark('P');
    std::va_list args;
    va_start(args, format);
    int written = 0;
    if (sprintf_count < 24)
        copy_text(sprintf_formats[sprintf_count], 40, format);
    if (same(format, "maxstream=%d")) {
        const int value = va_arg(args, int);
        sprintf_value = value;
        copy_text(destination, 32, "maxstream=");
        written = 10 + decimal(destination + 10, value);
    } else if (same(format, "cdrom0:\\bg\\0mod\\%s;1")) {
        const char* name = va_arg(args, const char*);
        if (sprintf_count < 24)
            copy_text(sprintf_names[sprintf_count], 32, name);
        copy_text(destination, 80, "cdrom0:\\bg\\0mod\\");
        int index = 0;
        while (destination[index] != 0)
            ++index;
        int name_index = 0;
        for (; name[name_index] != 0; ++name_index)
            destination[index++] = name[name_index];
        destination[index++] = ';';
        destination[index++] = '1';
        destination[index] = 0;
        written = index;
    } else {
        destination[0] = 0;
        mark('!');
    }
    ++sprintf_count;
    va_end(args);
    return written;
}
extern "C" int scePrintf(const char* format, ...) {
    mark('T');
    std::va_list args;
    va_start(args, format);
    if (same(format, "load iop module '%s'\n")) {
        const char* path = va_arg(args, const char*);
        if (load_count < 24)
            copy_text(loads[load_count].path, 80, path);
    } else if (!same(format, "Can't load iop modules!\n")) {
        mark('!');
    }
    va_end(args);
    return 0;
}
extern "C" int sceSifLoadModule(const char* path, int argument_length, const void* argument) {
    mark('L');
    if (load_count < 24) {
        copy_text(loads[load_count].path, 80, path);
        loads[load_count].argument_length = argument_length;
        loads[load_count].argument = argument;
        const unsigned char* bytes = static_cast<const unsigned char*>(argument);
        const int count = argument_length < 96 ? argument_length : 96;
        for (int index = 0; index < count; ++index)
            loads[load_count].bytes[index] = bytes[index];
    }
    const int index = load_count++;
    if (fail_at_load >= 0 && index == fail_at_load)
        return -1;
    return 0;
}
extern "C" unsigned int strlen(const char* text) {
    unsigned int count = 0;
    while (text[count] != 0)
        ++count;
    return count;
}
extern "C" void exit(int status) {
    mark('E');
    exit_status = status;
    ++exit_calls;
}
extern "C" void PreparePowerOff__Fv() { mark('W'); ++power_calls; }
extern "C" void textSystemInit__Fv() { mark('X'); ++text_calls; }
extern "C" void commSystemInit__Fv() { mark('O'); ++comm_calls; }
extern "C" void sceSifInitIopHeap() { mark('H'); ++heap_calls; }

static void reset() {
    reboot_count = 0;
    sync_count = 0;
    load_count = 0;
    sequence_used = 0;
    sequence[0] = 0;
    sprintf_count = 0;
    sprintf_value = 0;
    exit_status = -1;
    exit_calls = 0;
    power_calls = 0;
    text_calls = 0;
    comm_calls = 0;
    heap_calls = 0;
    reboot_image = 0;
    for (int index = 0; index < 258; ++index)
        _ctype_[index] = 0;
    _ctype_['c' + 1] = 2;
    _ctype_['s' + 1] = 2;
}

static int expect(const char* label, int code, int condition) {
    if (condition)
        return 0;
    std::printf("%s failed (%d): %s\n", label, code, sequence);
    return code;
}

int main() {
    if (sizeof path_buffer != 80) return 1;
    if (sizeof usbd_argument != 43) return 2;
    if (sizeof netcnf_argument != 94) return 3;
    if (sizeof inetctl_argument != 20) return 4;
    if (sizeof modules / sizeof modules[0] != 20) return 5;

    static const char* paths[20] = {
        "cdrom0:\\bg\\0mod\\Sio2man.irx;1",
        "cdrom0:\\bg\\0mod\\mtapman.irx;1",
        "cdrom0:\\bg\\0mod\\mCman.irx;1",
        "cdrom0:\\bg\\0mod\\mCServ.irx;1",
        "cdrom0:\\bg\\0mod\\padman.irx;1",
        "cdrom0:\\bg\\0mod\\CdvdStm.irx;1",
        "cdrom0:\\bg\\0mod\\libSd.irx;1",
        "cdrom0:\\bg\\0mod\\Sdrdrv.irx;1",
        "cdrom0:\\bg\\0mod\\hello.irx;1",
        "cdrom0:\\bg\\0mod\\uSbd.irx;1",
        "cdrom0:\\bg\\0mod\\lgkbm.irx;1",
        "cdrom0:\\bg\\0mod\\lgaud.irx;1",
        "cdrom0:\\bg\\0mod\\dev9.irx;1",
        "cdrom0:\\bg\\0mod\\netCnf.irx;1",
        "cdrom0:\\bg\\0mod\\inet.irx;1",
        "cdrom0:\\bg\\0mod\\inetCtl.irx;1",
        "cdrom0:\\bg\\0mod\\mSifrpC.irx;1",
        "cdrom0:\\bg\\0mod\\libnet.irx;1",
        "cdrom0:\\bg\\0mod\\eznetCtl.irx;1",
        "cdrom0:\\bg\\0mod\\eznetCnf.irx;1",
    };
    static const unsigned char usbd_bytes[] = {
        'h', 'u', 'b', '=', '1', 0,
        'd', 'e', 'v', '=', '1', '1', 0,
        'e', 'd', '=', '3', '2', 0,
        'g', 't', 'd', '=', '6', '4', 0,
        'i', 't', 'd', '=', '6', '4', 0,
        'i', 'o', 'r', 'e', 'q', '=', '1', '2', '8', 0,
    };
    static const char netcnf_bytes[] =
        "icon=cdrom0:\\CONF\\NETICON\\JAPANESE\\SYS_NET.ICO\0"
        "iconsys=cdrom0:\\CONF\\NETICON\\JAPANESE\\ICON.SYS";
    static const char inetctl_bytes[] = "-no_auto\0-no_decode";

    reset();
    reboot_results[0] = 0;
    reboot_results[1] = -5;
    sync_results[0] = 0;
    sync_results[1] = 0;
    sync_results[2] = 3;
    fail_at_load = -1;
    initSif__Fv();

    const char* success = "ICRRYYYIFSCMPPTLPTLPTLPTLPTLPTLPTLPTLPTLPTLPTLPTLPTLPTLPTLPTLPTLPTLPTLPTLWXOH";
    if (int code = expect("success order", 10, same(sequence, success))) return code;
    if (int code = expect("reboot image", 11, reboot_image != 0 && same(reboot_image, "cdrom0:\\BG\\0MOD\\DNAS280.IMG;1"))) return code;
    if (int code = expect("reboot count", 12, reboot_count == 2 && sync_count == 3)) return code;
    if (int code = expect("stream value", 13, sprintf_value == 1280 && sprintf_count == 21)) return code;
    if (int code = expect("maxstream format", 14, same(sprintf_formats[0], "maxstream=%d"))) return code;
    if (int code = expect("no exit", 15, exit_calls == 0 && power_calls == 1 && text_calls == 1 && comm_calls == 1 && heap_calls == 1)) return code;
    if (int code = expect("load count", 16, load_count == 20)) return code;
    for (int index = 0; index < 20; ++index) {
        if (!same(loads[index].path, paths[index])) {
            std::printf("path %d got %s\n", index, loads[index].path);
            return 20 + index;
        }
        if (!same(sprintf_formats[index + 1], "cdrom0:\\bg\\0mod\\%s;1"))
            return 50 + index;
    }
    for (int index = 0; index < 20; ++index) {
        if (index == 9 || index == 11 || index == 13 || index == 15)
            continue;
        if (loads[index].argument_length != 0 || loads[index].argument != 0)
            return 70 + index;
    }
    if (loads[9].argument_length != 43 || loads[9].argument == 0) return 80;
    for (int index = 0; index < 43; ++index) {
        if (loads[9].bytes[index] != static_cast<unsigned char>(usbd_bytes[index]))
            return 81;
    }
    if (loads[11].argument_length != 15 || loads[11].argument == 0) return 82;
    const char* stream = "maxstream=1280";
    for (int index = 0; index < 15; ++index) {
        if (loads[11].bytes[index] != static_cast<unsigned char>(stream[index]))
            return 83;
    }
    if (loads[13].argument_length != 94) return 84;
    for (int index = 0; index < 94; ++index) {
        if (loads[13].bytes[index] != static_cast<unsigned char>(netcnf_bytes[index]))
            return 85;
    }
    if (loads[15].argument_length != 20) return 86;
    for (int index = 0; index < 20; ++index) {
        if (loads[15].bytes[index] != static_cast<unsigned char>(inetctl_bytes[index]))
            return 87;
    }

    reset();
    reboot_results[0] = 1;
    sync_results[0] = 1;
    fail_at_load = 8;
    initSif__Fv();
    const char* failure = "ICRYIFSCMPPTLPTLPTLPTLPTLPTLPTLPTLPTLTEWXOH";
    if (int code = expect("failure order", 90, same(sequence, failure))) return code;
    if (int code = expect("stopped", 91, load_count == 9 && exit_calls == 1 && exit_status == 0)) return code;
    if (int code = expect("fall through", 92, power_calls == 1 && text_calls == 1 && comm_calls == 1 && heap_calls == 1)) return code;
    if (!same(loads[8].path, "cdrom0:\\bg\\0mod\\hello.irx;1")) return 93;

    std::puts("initSif passed");
    return 0;
}

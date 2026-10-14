// Observed initSif__Fv at 0x0018e6a0, 2676 bytes, local binding in .text.
// engineInit calls this name, so the definition is published global.
// The bytes spin on sceSifRebootIop until it is nonzero, then on
// sceSifSyncIop until it is nonzero. A negative module load prints the
// failure text and calls exit(0). If exit returns, the bytes continue into
// the four tail calls. The path text is the local buffer.744 at 0x00790fa0.
// draw is 80 bytes later. The fold starts at offset 7 and reads
// _ctype_ at 0x0047e1f0 through the byte at (_ctype_ + 1 + sign-extended
// character). Bit 2 stores that character minus 32. The module names and
// argument blobs below are rodata with no symbols; the addresses are the
// ones the loads use. What the IOP does with the image and the modules is
// unknown.
extern "C" void sceSifInitRpc(int mode);
extern "C" int sceCdInit(int mode);
extern "C" int sceSifRebootIop(const char* image);
extern "C" int sceSifSyncIop();
extern "C" void sceSifLoadFileReset();
extern "C" void sceFsReset();
extern "C" int sceCdMmode(int mode);
extern "C" int sprintf(char* destination, const char* format, ...);
extern "C" int scePrintf(const char* format, ...);
extern "C" int sceSifLoadModule(const char* path, int argument_length, const void* argument);
extern "C" unsigned int strlen(const char* text);
extern "C" void exit(int status);
extern "C" void PreparePowerOff__Fv();
extern "C" void textSystemInit__Fv();
extern "C" void commSystemInit__Fv();
extern "C" void sceSifInitIopHeap();
extern "C" unsigned char _ctype_[];

static unsigned char path_buffer[80];

// 0x004640d0, 43 bytes, passed to usbd.irx.
static const char usbd_argument[] =
    "hub=1\0"
    "dev=11\0"
    "ed=32\0"
    "gtd=64\0"
    "itd=64\0"
    "ioreq=128";

// 0x00464140, 94 bytes, passed to netcnf.irx.
static const char netcnf_argument[] =
    "icon=cdrom0:\\CONF\\NETICON\\JAPANESE\\SYS_NET.ICO\0"
    "iconsys=cdrom0:\\CONF\\NETICON\\JAPANESE\\ICON.SYS";

// 0x004641c0, 20 bytes, passed to inetctl.irx.
static const char inetctl_argument[] = "-no_auto\0-no_decode";

struct ModuleRequest {
    const char* name;
    int argument_length;
    const void* argument;
};

// argument_length -1 is the lgaud.irx site: strlen of the maxstream text, plus one.
static const ModuleRequest modules[] = {
    {"sio2man.irx", 0, 0},
    {"mtapman.irx", 0, 0},
    {"mcman.irx", 0, 0},
    {"mcserv.irx", 0, 0},
    {"padman.irx", 0, 0},
    {"cdvdstm.irx", 0, 0},
    {"libsd.irx", 0, 0},
    {"sdrdrv.irx", 0, 0},
    {"hello.irx", 0, 0},
    {"usbd.irx", 43, usbd_argument},
    {"lgkbm.irx", 0, 0},
    {"lgaud.irx", -1, 0},
    {"dev9.irx", 0, 0},
    {"netcnf.irx", 94, netcnf_argument},
    {"inet.irx", 0, 0},
    {"inetctl.irx", 20, inetctl_argument},
    {"msifrpc.irx", 0, 0},
    {"libnet.irx", 0, 0},
    {"eznetctl.irx", 0, 0},
    {"eznetcnf.irx", 0, 0},
};

extern "C" void initSif__Fv() {
    sceSifInitRpc(0);
    sceCdInit(0);
    while (sceSifRebootIop("cdrom0:\\BG\\0MOD\\DNAS280.IMG;1") == 0) {
    }
    while (sceSifSyncIop() == 0) {
    }
    sceSifInitRpc(0);
    sceSifLoadFileReset();
    sceFsReset();
    sceCdInit(0);
    sceCdMmode(2);

    char stream_text[32];
    sprintf(stream_text, "maxstream=%d", 1280);

    int failed = 0;
    const int module_count = static_cast<int>(sizeof modules / sizeof modules[0]);
    for (int index = 0; index < module_count; ++index) {
        const ModuleRequest& module = modules[index];
        sprintf(reinterpret_cast<char*>(path_buffer), "cdrom0:\\bg\\0mod\\%s;1", module.name);
        unsigned char* cursor = path_buffer + 7;
        while (*cursor != 0) {
            const int signed_byte = static_cast<signed char>(*cursor);
            const unsigned int flags = _ctype_[signed_byte + 1];
            int stored = signed_byte;
            if ((flags & 2) != 0)
                stored = signed_byte - 32;
            *cursor = static_cast<unsigned char>(stored);
            ++cursor;
        }
        scePrintf("load iop module '%s'\n", path_buffer);
        int argument_length = module.argument_length;
        const void* argument = module.argument;
        if (argument_length < 0) {
            argument_length = static_cast<int>(strlen(stream_text)) + 1;
            argument = stream_text;
        }
        if (sceSifLoadModule(reinterpret_cast<const char*>(path_buffer), argument_length, argument) < 0) {
            failed = 1;
            break;
        }
    }
    if (failed) {
        scePrintf("Can't load iop modules!\n");
        exit(0);
    }
    PreparePowerOff__Fv();
    textSystemInit__Fv();
    commSystemInit__Fv();
    sceSifInitIopHeap();
}

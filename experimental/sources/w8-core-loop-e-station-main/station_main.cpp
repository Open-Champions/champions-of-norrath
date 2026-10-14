// Observed station_main__FiPCcPcT2T1T1i at 0x0116cf48, 344 bytes.
// The VIStation object lives in this function's stack frame, which is 0x17b00
// bytes including the 64 bytes of saved registers; 96960 bytes are reserved
// for the object here as an upper bound, not its real size.
// It prints "creating VIStation...\n", constructs the object (__9VIStation),
// prints "created VIStation\n", and calls sceGsSyncV(0) twice. Init is
// called with the object and the seven arguments in order (mode, name,
// username, password, text, copied, launch byte). A negative result destroys
// the object with _$_9VIStation(object, 2) and returns -1. Otherwise Process
// runs until its result is nonzero, printing "station_main: process\n" after
// each zero result. Then "station_main: free\n", Free, "station_main: free
// ok!\n", and the object is destroyed with flags 2. Process result 2 returns
// 1; anything else returns 0.
// Unknown: what the VIStation result codes mean.
extern "C" {
void* __9VIStation(void* object);
void _$_9VIStation(void* object, int flags);
int Init__9VIStationiPCcPcT3T2T2i(void* object, int mode, const char* name,
                                  char* username, char* password,
                                  const char* text, const char* copied,
                                  int launch_byte);
int Process__9VIStation(void* object);
void Free__9VIStation(void* object);
int sceGsSyncV(int mode);
int printf(const char* format, ...);
}

extern "C" int station_main__FiPCcPcT2T1T1i(int mode, const char* name, char* username,
                                             char* password, const char* text,
                                             const char* copied, int launch_byte) {
    alignas(16) unsigned char object[96960];
    printf("creating VIStation...\n");
    __9VIStation(object);
    printf("created VIStation\n");
    sceGsSyncV(0);
    sceGsSyncV(0);
    if (Init__9VIStationiPCcPcT3T2T2i(object, mode, name, username, password, text, copied,
                                       launch_byte) < 0) {
        _$_9VIStation(object, 2);
        return -1;
    }
    int outcome;
    for (;;) {
        outcome = Process__9VIStation(object);
        if (outcome != 0) break;
        printf("station_main: process\n");
    }
    printf("station_main: free\n");
    Free__9VIStation(object);
    printf("station_main: free ok!\n");
    _$_9VIStation(object, 2);
    return outcome == 2 ? 1 : 0;
}

// Observed srs_main at 0x0116ca68, 1172 bytes. It returns an int.
// The body lives in the .srs image. Names below are the ELF symbols at the
// addresses this function uses. The text arguments have no ELF symbol.
// The ninth argument is stored by launchSRS at 0(sp) and is not read here.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" void norrath_host_or32(unsigned address, unsigned mask);
#define NORRATH_OR32(address, mask) norrath_host_or32((address), (mask))
#else
#define NORRATH_OR32(address, mask)                                 \
    do {                                                            \
        volatile unsigned* word =                                   \
            reinterpret_cast<volatile unsigned*>(address);          \
        *word = *word | (mask);                                     \
    } while (0)
#endif

extern "C" {
struct VILauncher;
struct VINetworkStatus;
void __10VILauncher(VILauncher* object);
void __15VINetworkStatus(VINetworkStatus* object);
void _$_15VINetworkStatus(VINetworkStatus* object, int flags);
void Initialize__15VINetworkStatus(VINetworkStatus* object);
int CheckNetwork__15VINetworkStatus(VINetworkStatus* object);
int printf(const char* text, ...);
void* memset(void* destination, int value, unsigned count);
void sceGsResetPath();
void sceDmaReset(int value);
void sceSifInitRpc();
void sceSifInitIopHeap();
int Init__10VILauncherb(VILauncher* object, int network_up);
void exit(int code);
void SetDefault__10VILanguage12VILanguageID(int value);
void VIMemCardSetProductId__FPCc(const char* text);
void VIMemCardSetProductTitle__FPCc(const char* text);
void VIDisplayLoadingScreen__Fb(int value);
void Init__7VITimer();
int Setup__10VILauncher(VILauncher* object);
void ClearVRAM__FUcUcUcUc(unsigned char a, unsigned char b, unsigned char c, unsigned char d);
int GetDefault__10VILanguage();
int sprintf(char* destination, const char* format, int value);
void Free__10VILauncher(VILauncher* object);
void LoadExecPS2(const char* path, int argc, char** argv);
void Close__7VITimer();
void ezNetCtlDownInterface(int value);
void exitEzNetCnf();
void exitEzNetCtl();
void _$_10VILauncher(VILauncher* object, int flags);
int Station__10VILauncheriPCcN22i(
    VILauncher* object, int mode, const char* name, const char* text_135,
    const char* copied_text, int launch_byte);
char* strcpy(char* destination, const char* source);

int g_mode = 3;
int g_myIndex;
int norrath_srs_first_flag __asm__("first.3069") = 1;
char g_ips[96];
int g_ports[4];
char g_localClientIps[96];
int g_localClientPorts[4];
int g_numClients;
char g_localIp[24];
char* s_argv[2];
char s_buf[32];

char norrath_srs_marker_1[] = "1\n";
char norrath_srs_marker_2[] = "2\n";
char norrath_srs_marker_a[] = "a\n";
char norrath_srs_marker_c[] = "c\n";
char norrath_srs_marker_b[] = "b\n";
char norrath_srs_marker_3[] = "3\n";
char norrath_srs_server_mode[] = "Server mode\n";
char norrath_srs_client_mode[] = "Client mode\n";
char norrath_srs_unknown_mode[] = "Unknown mode\n";
char norrath_srs_launcher_failed[] = "launcher init() failed!\n";
char norrath_srs_product_id[] = "BASLUS-20565EQONLINE";
char norrath_srs_product_title[] = "EverQuest Online\nAdventures";
char norrath_srs_language_word[] = "language";
char norrath_srs_decimal_format[] = "%d";
char norrath_srs_ntgui_path[] = "cdrom0:\\NTGUI2.ELF;1";
char norrath_srs_whoop_text[] = "Whoop de doo!\n";

int srs_main__FPCcPiT1PPcT1T1T3T1bT0T0i(
    const char* name, int* flag_word, int* count_word, char** first_slots, int* second_words,
    int* zero_word, char** third_slots, int* fourth_words, int low_bit_clear,
    const char* text_135, const char* copied_text, int launch_byte) {
    (void)low_bit_clear;
    alignas(8) unsigned char launcher_bytes[8];
    alignas(8) unsigned char status_bytes[8];
    VILauncher* launcher = reinterpret_cast<VILauncher*>(launcher_bytes);
    VINetworkStatus* status = reinterpret_cast<VINetworkStatus*>(status_bytes);
    __10VILauncher(launcher);
    printf(norrath_srs_marker_1);
    printf(norrath_srs_marker_2);
    printf(norrath_srs_marker_a);
    __15VINetworkStatus(status);
    printf(norrath_srs_marker_c);
    Initialize__15VINetworkStatus(status);
    printf(norrath_srs_marker_b);
    int network = CheckNetwork__15VINetworkStatus(status);
    _$_15VINetworkStatus(status, 2);
    int network_up = network != 0;
    printf(norrath_srs_marker_3);
    int mode = *flag_word;
    if (mode == 1) printf(norrath_srs_server_mode);
    else if (mode == 2) printf(norrath_srs_client_mode);
    else printf(norrath_srs_unknown_mode);
    memset(g_ips, 0, 96);
    memset(g_ports, 0, 16);
    memset(g_localIp, 0, 24);
    memset(g_localClientIps, 0, 96);
    memset(g_localClientPorts, 0, 16);
    sceGsResetPath();
    if (norrath_srs_first_flag == 0) {
        norrath_srs_first_flag = 0;
    } else {
        sceDmaReset(1);
        NORRATH_OR32(0x1000e020u, 2u);
        sceSifInitRpc();
        sceSifInitIopHeap();
        norrath_srs_first_flag = 0;
    }
    int init_result = Init__10VILauncherb(launcher, network_up);
    if (init_result < 0) {
        printf(norrath_srs_launcher_failed);
        exit(-1);
    }
    SetDefault__10VILanguage12VILanguageID(0);
    VIMemCardSetProductId__FPCc(norrath_srs_product_id);
    VIMemCardSetProductTitle__FPCc(norrath_srs_product_title);
    VIDisplayLoadingScreen__Fb(1);
    Init__7VITimer();
    int setup_result = 0;
    if (network_up != 0) setup_result = Setup__10VILauncher(launcher);
    ClearVRAM__FUcUcUcUc(0, 0, 0, 0);
    if (static_cast<unsigned>(setup_result + 1) < 7u) {
        if (setup_result == 2) {
            s_argv[0] = norrath_srs_language_word;
            s_argv[1] = s_buf;
            int language = GetDefault__10VILanguage();
            sprintf(s_buf, norrath_srs_decimal_format, language);
            Free__10VILauncher(launcher);
            LoadExecPS2(norrath_srs_ntgui_path, 2, s_argv);
        } else if (setup_result == 4 || setup_result == 5) {
            Free__10VILauncher(launcher);
            Close__7VITimer();
            ezNetCtlDownInterface(0);
            exitEzNetCnf();
            exitEzNetCtl();
            _$_10VILauncher(launcher, 2);
            return 0;
        }
    }
    int station = Station__10VILauncheriPCcN22i(
        launcher, *flag_word, name, text_135, copied_text, launch_byte);
    int result = station == 1 ? 0 : 100;
    printf(norrath_srs_whoop_text);
    exitEzNetCnf();
    exitEzNetCtl();
    *flag_word = g_mode;
    *count_word = g_numClients;
    int index = 0;
    if (*count_word > 0) {
        while (index < *count_word) {
            strcpy(first_slots[index], g_ips + index * 24);
            second_words[index] = g_ports[index];
            strcpy(third_slots[index], g_localClientIps + index * 24);
            fourth_words[index] = g_localClientPorts[index];
            index += 1;
        }
    }
    *zero_word = g_myIndex;
    ClearVRAM__FUcUcUcUc(0, 0, 0, 0);
    Close__7VITimer();
    _$_10VILauncher(launcher, 2);
    return result;
}
}

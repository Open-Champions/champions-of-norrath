// Observed Station__10VILauncheriPCcN22i at 0x0106c570, 188 bytes. It
// returns the result of station_main. Username is copied to offset 8 and
// password to offset 264. Each string comes from a separate
// VIGetGameOptions call. The password length is unknown. station_main
// receives the mode, the name, those two copies, the two text arguments,
// and the last integer.
extern "C" {
struct VILauncher;
void* VIGetGameOptions__Fv();
char* Username__C13VIGameOptions(void* options);
char* Password__C13VIGameOptions(void* options);
char* strcpy(char* destination, const char* source);
int station_main__FiPCcPcT2T1T1i(
    int mode, const char* name, char* username, char* password,
    const char* text_135, const char* copied, int launch_byte);

int Station__10VILauncheriPCcN22i(
    VILauncher* object, int mode, const char* name, const char* text_135,
    const char* copied, int launch_byte) {
    unsigned char* bytes = reinterpret_cast<unsigned char*>(object);
    char* username = reinterpret_cast<char*>(bytes + 8);
    char* password = reinterpret_cast<char*>(bytes + 264);
    strcpy(username, Username__C13VIGameOptions(VIGetGameOptions__Fv()));
    strcpy(password, Password__C13VIGameOptions(VIGetGameOptions__Fv()));
    return station_main__FiPCcPcT2T1T1i(
        mode, name, username, password, text_135, copied, launch_byte);
}
}

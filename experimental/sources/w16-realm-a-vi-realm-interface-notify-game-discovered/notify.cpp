// Observed notifyGameDiscovered__16VIRealmInterfacePCci at 0x011093a0, 152 bytes, 48-byte frame. With (this, address,
// port), this being the VIRealmInterface object:
//  1. printf("VIRealmInterface::notifyGameDiscovered(%s, %i)\n", address, port).
//  2. strcpy(g_localIp, address); the word g_localPort = port.
//  3. strcpy(this + 2348, address); the word at this + 3372 = port; the word at this + 3468 = 2.
//  4. GetInetAddress__11VIUDPSocketP13VIInetAddressPCci(this + 3396, this + 38012, g_localIp, 1000).
// The original performs the g_localPort store in the delay slot of the second strcpy call; that is the same
// order of effects as far as the callees can see (neither reads g_localPort).
// Inferred: this is the callback of the UDP discovery of a game server; +2348 holds the address text, +3372 the
// port, +3396 the UDP socket (the constructor of VIRealmInterface builds a socket there) and +38012 an address
// result record. The 1000 is passed as the last argument unchanged.
// Unknown: what the value 2 at +3468 means (a state number: the sibling client callback stores 11), what the
// 1000 means (a time-out in some unit is a guess), and the size of the address record.
extern "C" {
int printf(const char* format, ...);
char* strcpy(char* dest, const char* src);
int GetInetAddress__11VIUDPSocketP13VIInetAddressPCci(void* socket, void* address, const char* host, int port);
extern char g_localIp[24];
extern int g_localPort;

void notifyGameDiscovered__16VIRealmInterfacePCci(void* self, const char* address, int port) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    printf("VIRealmInterface::notifyGameDiscovered(%s, %i)\n", address, port);
    strcpy(g_localIp, address);
    g_localPort = port;
    strcpy(reinterpret_cast<char*>(bytes + 2348), address);
    *reinterpret_cast<int*>(bytes + 3372) = port;
    *reinterpret_cast<int*>(bytes + 3468) = 2;
    GetInetAddress__11VIUDPSocketP13VIInetAddressPCci(bytes + 3396, bytes + 38012, g_localIp, 1000);
}
}

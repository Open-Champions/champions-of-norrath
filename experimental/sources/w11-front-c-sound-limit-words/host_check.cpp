// Host check for soundGroupLimit and soundLimit. Not an EE plan input.
extern "C" {
extern unsigned char soundGroupLimit[300];
extern unsigned char soundLimit[600];
}
int main() {
    int twos = 0;
    for (int i = 0; i < 300; ++i) twos += soundGroupLimit[i] == 2;
    if (twos != 297) return 1;
    if (soundGroupLimit[14] != 9 || soundGroupLimit[50] != 8 || soundGroupLimit[51] != 8) return 2;
    for (int i = 0; i < 600; ++i) if (soundLimit[i]) return 3;
    return 0;
}

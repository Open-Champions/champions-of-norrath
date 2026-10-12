#include <cstring>
#include <cstdlib>
struct In { unsigned char b[126]; };
struct Out { unsigned char b[8]; };
extern "C" In norrath_in_msg_entries[8];
extern "C" Out norrath_out_msg_head;
extern "C" int frontEndRunning;
extern "C" void textInit__Fv();
In norrath_in_msg_entries[8];
Out norrath_out_msg_head;
int frontEndRunning;

int main() {
    memset(norrath_in_msg_entries, 0x77, sizeof norrath_in_msg_entries);
    memset(&norrath_out_msg_head, 0x55, sizeof norrath_out_msg_head);
    frontEndRunning = 3;
    textInit__Fv();
    if (norrath_in_msg_entries[3].b[5] != 0x77 || norrath_out_msg_head.b[0] != 0x55) return 1;
    frontEndRunning = 0;
    textInit__Fv();
    for (int e = 0; e < 8; e++)
        for (int i = 0; i < 126; i++)
            if (norrath_in_msg_entries[e].b[i] != (i == 2 ? 0xfe : 0)) return 2;
    const unsigned char want[8] = {1, 0, 0x55, 32, 0, 0x55, 0x55, 0x55};
    if (memcmp(&norrath_out_msg_head, want, 8)) return 3;
    return 0;
}

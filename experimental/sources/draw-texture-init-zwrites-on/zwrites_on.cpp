// Observed zwritesOn at 0x004843f8, 8 zero bytes. zwritesOff begins at
// 0x00484400. initSupersample stores a qword here. The reviewed -G0
// profile places the zero in BSS.

extern "C" {
unsigned long long zwritesOn;
}

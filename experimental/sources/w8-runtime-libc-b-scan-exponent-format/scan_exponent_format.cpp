// Observed string at 0x0047ef98: "e%ld" and its terminating zero, 5 bytes
// (the image pads to 8). The scanner passes it to sprintf with the adjusted
// decimal exponent as a long.
extern "C" const unsigned char norrath_player_scan_exponent_format_view[] = {
    0x65, 0x25, 0x6c, 0x64, 0x00
};

#include "table.cpp"
int main() {
    if (sizeof(_8VIString$ConversionTable1252) != 64) return 1;
    // spot values and the published-mapping entries this table keeps
    if (_8VIString$ConversionTable1252[0x91 - 0x80] != 0x2018) return 2;
    if (_8VIString$ConversionTable1252[0x99 - 0x80] != 0x2122) return 3;
    if (_8VIString$ConversionTable1252[0x9f - 0x80] != 0x0178) return 4;
    if (_8VIString$ConversionTable1252[0x82 - 0x80] != 0x20a1) return 5;
    if (_8VIString$ConversionTable1252[0x9e - 0x80] != 0xfffe) return 6;
    unsigned sum = 0, nfffe = 0;
    for (int i = 0; i < 32; ++i) { sum += _8VIString$ConversionTable1252[i]; nfffe += _8VIString$ConversionTable1252[i] == 0xfffe; }
    return (nfffe == 8 && sum == 0xa11a0) ? 0 : 7;
}

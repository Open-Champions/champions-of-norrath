#include "keyboard_get_next_translated_key.cpp"

#include <cstdio>
#include <initializer_list>

int inesc;
static int feed[16], nfeed, pos;
extern "C" int keyboardGetNextKey__Fv() { return pos < nfeed ? feed[pos++] : -1; }
static int run(std::initializer_list<int> l) {
    nfeed = 0; pos = 0;
    for (int v : l) feed[nfeed++] = v;
    return keyboardGetNextTranslatedKey__Fv();
}

int main() {
    inesc = 0;
    if (run({}) != -1 || inesc != 0) return 1;
    if (run({'a'}) != 'a' || inesc != 0) return 2;   // no translation outside escape
    if (run({27}) != -1 || inesc != 1) return 3;     // escape then nothing
    if (run({'a'}) != 127 || inesc != 0) return 4;
    if (run({27, 'a'}) != 127 || inesc != 0) return 5;
    if (run({27, 27, 27, 'k'}) != 123 || pos != 4) return 6;
    static const int in[]  = {'0','1','2','9','A','a','S','s','W','w','Z','z','I','i','J','j','K','k','M','m','C','c','B','b'};
    static const int out[] = {133,131,132,134,127,127,128,128,129,129,130,130,124,124,125,125,123,123,126,126,136,136,135,135};
    for (unsigned i = 0; i < sizeof in / sizeof *in; ++i) {
        inesc = 1;
        if (run({in[i]}) != out[i] || inesc != 0) return 10 + static_cast<int>(i);
    }
    static const int same[] = {47, 51, 58, 64, 68, 76, 91, 123, 200, 5};
    for (int c : same) {
        inesc = 1;
        if (run({c}) != c || inesc != 0) return 50;
    }
    inesc = 1;
    if (run({27, 27}) != -1 || inesc != 1) return 51;
    std::puts("keyboard get next translated key passed");
    return 0;
}

// Observed: keyboardGetNextTranslatedKey__Fv at 0x002ca550 is 272 bytes. It
// wraps keyboardGetNextKey__Fv with a state word inesc.
//  - inesc is zero: it reads a key; -1 is returned as is; any key other than 27
//    is returned unchanged; key 27 sets inesc to 1 and continues as below.
//  - inesc is nonzero: it keeps reading. -1 is returned (inesc stays set); 27
//    is skipped and the read repeats; any other key clears inesc and is
//    returned, translated when it is in 48..122: '0' (48) 133, '1' 131, '2'
//    132, '9' 134, 'A'/'a' 127, 'S'/'s' 128, 'W'/'w' 129, 'Z'/'z' 130,
//    'I'/'i' 124, 'J'/'j' 125, 'K'/'k' 123, 'M'/'m' 126, 'C'/'c' 136, 'B'/'b'
//    135. Other codes in 48..122, and all codes outside that range, are
//    returned unchanged.
// Inferred: this decodes keys that follow an escape (27), such as cursor and
// function sequences, into values above 122; the letters mapped are the ones
// the table lists, and no terminal protocol is claimed.
// Unknown: what the values 123 to 136 stand for to the callers.

extern "C" int keyboardGetNextKey__Fv();
extern "C" int inesc;

extern "C" int keyboardGetNextTranslatedKey__Fv() {
    if (inesc == 0) {
        int key = keyboardGetNextKey__Fv();
        if (key == -1) return key;
        if (key != 27) return key;
        inesc = 1;
    }
    for (;;) {
        int key = keyboardGetNextKey__Fv();
        if (key == -1) return key;
        if (key == 27) continue;
        switch (key) {
        case '1': key = 131; break;
        case '2': key = 132; break;
        case '0': key = 133; break;
        case '9': key = 134; break;
        case 'W': case 'w': key = 129; break;
        case 'A': case 'a': key = 127; break;
        case 'S': case 's': key = 128; break;
        case 'Z': case 'z': key = 130; break;
        case 'I': case 'i': key = 124; break;
        case 'J': case 'j': key = 125; break;
        case 'K': case 'k': key = 123; break;
        case 'M': case 'm': key = 126; break;
        case 'C': case 'c': key = 136; break;
        case 'B': case 'b': key = 135; break;
        default: break;
        }
        inesc = 0;
        return key;
    }
}

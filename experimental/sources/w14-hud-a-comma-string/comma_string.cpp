// Observed commaString__FPc at 0x001bbf48, 208 bytes. It rewrites a digit
// string in place, inserting ',' separators, and returns nothing.
//
// Observed behavior: with n = strlen(s) and k = (n - 1) / 3 (signed division),
// it runs k passes. Each pass takes the leading-group length from a value that
// starts as n and is replaced by (value % 3), or by value / 2 when that
// remainder is zero. The group length is carried to the next pass. The pass
// moves bytes [group + 4*i - 1, n + k] up one place (so one byte past the new
// terminator is also written) and stores ',' at s[group + 4*i].
// For n = 6 the first group is 3, which looks intended; for n = 9 the same rule
// gives 4. That quirk is reproduced, not corrected. The caller's buffer must
// have room for n + k + 2 bytes.
extern "C" unsigned long strlen(const char*);

extern "C" void commaString__FPc(char* s) {
    int n = (int)strlen(s);
    int k = (n - 1) / 3;
    int end = n + k + 1;
    int group = n;
    for (int i = 0; i < k; ++i) {
        int r = group % 3;
        group = r != 0 ? r : group / 2;
        int at = group + 4 * i;
        for (int j = end; j >= at; --j) s[j] = s[j - 1];
        s[at] = ',';
    }
}

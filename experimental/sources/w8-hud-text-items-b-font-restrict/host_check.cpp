extern "C" {
int restrictChars;
void fontRestrict__Fi(int count);
}

int main() {
    restrictChars = 7;
    fontRestrict__Fi(-1);
    if (restrictChars != 100000) return 1;
    fontRestrict__Fi(0);
    if (restrictChars != 0) return 2;
    fontRestrict__Fi(25);
    if (restrictChars != 25) return 3;
    fontRestrict__Fi(-2);
    if (restrictChars != -2) return 4;
    fontRestrict__Fi(100000);
    if (restrictChars != 100000) return 5;
    return 0;
}

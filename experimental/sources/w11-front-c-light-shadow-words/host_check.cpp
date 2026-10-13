// Host check for the light shadow words. Not an EE plan input.
extern "C" {
extern int saveValid;
extern void* saveBestList[3];
}
int main() {
    if (saveValid) return 1;
    for (void* p : saveBestList) if (p) return 2;
    return 0;
}

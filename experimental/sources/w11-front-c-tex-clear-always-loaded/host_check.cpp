// Host check for texClearAlwaysLoadedPages__Fv. Not an EE plan input.
extern "C" {
int nmAlwaysPages;
void texClearAlwaysLoadedPages__Fv();
}
int main() {
    nmAlwaysPages = 5;
    texClearAlwaysLoadedPages__Fv();
    if (nmAlwaysPages != 0) return 1;
    nmAlwaysPages = -3;
    texClearAlwaysLoadedPages__Fv();
    return nmAlwaysPages != 0 ? 2 : 0;
}

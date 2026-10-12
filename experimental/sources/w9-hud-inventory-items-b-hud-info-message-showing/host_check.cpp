extern "C" {
unsigned short infoMessageTimer;
int hudInfoMessageShowing__Fv();
}

int main() {
    infoMessageTimer = 0;
    if (hudInfoMessageShowing__Fv() != 0) return 1;
    infoMessageTimer = 1;
    if (hudInfoMessageShowing__Fv() != 1) return 2;
    infoMessageTimer = 0x8000;
    if (hudInfoMessageShowing__Fv() != 1) return 3;
    infoMessageTimer = 0xffff;
    return hudInfoMessageShowing__Fv() == 1 ? 0 : 4;
}

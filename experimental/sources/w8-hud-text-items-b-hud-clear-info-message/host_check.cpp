extern "C" {
unsigned short infoMessageTimer;
void hudClearInfoMessage__Fv();
}

int main() {
    infoMessageTimer = 300;
    hudClearInfoMessage__Fv();
    if (infoMessageTimer != 0) return 1;
    infoMessageTimer = 0xffff;
    hudClearInfoMessage__Fv();
    return infoMessageTimer != 0;
}

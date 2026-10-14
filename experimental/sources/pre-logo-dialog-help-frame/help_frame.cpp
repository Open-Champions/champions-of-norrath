// Observed helpMessageFrame__Fv, 0x1c4b18..0x1c5078 in the pinned image.
// Complete global and per-player paths. Controller records have stride 88;
// global dismissal uses netLocalPad+12, per-player dismissal pad+8/+12.
// Inferred types describe only accessed fields; rendering/hardware is untested.
extern "C" {
extern int controlPaused, gamePause, gameControlPaused, GLISTSIZE;
extern unsigned glistp, gstart, padAcceptButton;
extern signed char helpMessageOn, netNetworkMode, particlesPaused, interfaceOn;
extern int nmPlayers, nmPlayersAlive, playersAlive[];
extern unsigned char netLocalPad[], pad[];
extern signed char multiHelpMessageOn[], multiRelease[];
extern unsigned short *title, *body, *multiTitle[], *multiBody[];
extern int multiSide[], padAcceptButtonFontChar;
extern float coreInterfaceBrightness[];
extern void* mediumFont;
unsigned colorScale__FUif(unsigned, float);
void fontStart__FP4FontiUii(void*,int,unsigned,int);
void fontColor__FUiii(unsigned,int,int);
void fontPrintCenteredu__FiiPUsi(int,int,unsigned short*,int);
int fontPrintWrapu__FiiPUsiiii(int,int,unsigned short*,int,int,int,int);
int fontStringSizeu__FP4FontPUsi(void*,unsigned short*,int);
void fontPrint__FiiPci(int,int,char*,int);
void fontPrintu__FiiPUsi(int,int,unsigned short*,int);
void fontEnd__Fv();
void textureDrawBorder__FiiiiiUiPcii(int,int,int,int,int,unsigned,char*,int,int);
void rectangleDraw__FiiiiiUi(int,int,int,int,int,unsigned);
int sprintf(char*,const char*,...);
unsigned short* textGetString__FPc(char*);
void scriptActivatePostAction__Fv();
int netPlayerIsLocal__Fi(int);
void msgHudSetPlayerBusy__Fcc(int,int);
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | unsigned(p[1])<<8 | unsigned(p[2])<<16 | unsigned(p[3])<<24;
}
extern "C" void helpMessageFrame__Fv() {
    if (controlPaused > 0) --controlPaused;
    if (helpMessageOn) {
        if (!netNetworkMode) { particlesPaused=1; gamePause=1; }
        int used=static_cast<int>(glistp-gstart)>>3;
        if (static_cast<int>(unsigned(GLISTSIZE)-unsigned(used)) >= 3000) {
            fontStart__FP4FontiUii(mediumFont,6,colorScale__FUif(0x8028444e,coreInterfaceBrightness[0]),-1);
            if (title) fontPrintCenteredu__FiiPUsi(324,100,title,1000000);
            fontColor__FUiii(colorScale__FUif(0x80464646,coreInterfaceBrightness[0]),-1,0);
            int height=body ? fontPrintWrapu__FiiPUsiiii(132,130,body,404,0,448,21) : 0;
            char glyph[16]; sprintf(glyph,"%c",padAcceptButtonFontChar);
            int width=fontStringSizeu__FP4FontPUsi(mediumFont,textGetString__FPc(const_cast<char*>("{")),1000000);
            int alpha=static_cast<int>(128u-unsigned(controlPaused)*10u);
            if(alpha<0) alpha=0;
            if(alpha>128) alpha=128;
            fontColor__FUiii(colorScale__FUif((unsigned(alpha)<<24)|0x464646,coreInterfaceBrightness[0]),-1,0);
            int x=324-width/2;
            fontPrint__FiiPci(x-25,height+139,glyph,100000);
            fontPrintu__FiiPUsi(x,height+139,textGetString__FPc(const_cast<char*>("{")),1000000);
            fontEnd__Fv();
            textureDrawBorder__FiiiiiUiPcii(116,94,416,height+76,6,colorScale__FUif(0x80808080,coreInterfaceBrightness[0]),0,0,0);
            rectangleDraw__FiiiiiUi(124,102,540,height+185,6,0x3f000000);
        }
        if(controlPaused) return;
        // Do not break after the first dismissal: the original checks every pad.
        for(int i=0;i<nmPlayers;++i) if(read_word(netLocalPad+i*88+12)&padAcceptButton) {
            helpMessageOn=0; gameControlPaused=60;
            if(!netNetworkMode) scriptActivatePostAction__Fv();
            if(!interfaceOn) gamePause=0;
            controlPaused=60; particlesPaused=0;
            for(int j=0;j<nmPlayersAlive;++j)
                if(netPlayerIsLocal__Fi(playersAlive[j]))
                    msgHudSetPlayerBusy__Fcc(static_cast<signed char>(playersAlive[j]),0);
        }
    } else {
        for(int i=0;i<nmPlayers;++i) if(multiHelpMessageOn[i]) {
            int side=multiSide[i]*320;
            fontStart__FP4FontiUii(mediumFont,6,colorScale__FUif(0x8028444e,coreInterfaceBrightness[i]),-1);
            fontPrintCenteredu__FiiPUsi(side+160,100,multiTitle[i],1000000);
            fontColor__FUiii(colorScale__FUif(0x80464646,coreInterfaceBrightness[i]),-1,0);
            int height=fontPrintWrapu__FiiPUsiiii(side+36,130,multiBody[i],256,0,448,21);
            char glyph[16]; sprintf(glyph,"%c",padAcceptButtonFontChar);
            fontPrint__FiiPci(side+135,height+135,glyph,100000);
            fontPrintu__FiiPUsi(side+160,height+135,textGetString__FPc(const_cast<char*>("{")),1000000);
            fontEnd__Fv();
            textureDrawBorder__FiiiiiUiPcii(side+26,94,268,height+76,6,colorScale__FUif(0x80808080,coreInterfaceBrightness[i]),0,0,0);
            rectangleDraw__FiiiiiUi(side+34,102,side+302,height+178,6,0x3f000000);
            if(!(read_word(pad+i*88+8)&padAcceptButton)) multiRelease[i]=1;
            if(multiRelease[i] && (read_word(pad+i*88+12)&padAcceptButton)) {
                gameControlPaused=60; multiHelpMessageOn[i]=0; scriptActivatePostAction__Fv();
            }
        }
    }
}

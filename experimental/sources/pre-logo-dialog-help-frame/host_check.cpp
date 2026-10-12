#include "help_frame.cpp"
#include <cassert>
#include <vector>
#include <cstring>
extern "C" {
int controlPaused,gamePause,gameControlPaused,GLISTSIZE=4000;
unsigned glistp,gstart,padAcceptButton=1;
signed char helpMessageOn,netNetworkMode,particlesPaused,interfaceOn;
int nmPlayers=2,nmPlayersAlive=2,playersAlive[]={0,128};
unsigned char netLocalPad[176],pad[176];
signed char multiHelpMessageOn[2],multiRelease[2];
unsigned short words[]={1,0};
unsigned short *title=words,*body=words,*multiTitle[]={words,words},*multiBody[]={words,words};
int multiSide[]={0,1},padAcceptButtonFontChar=65;
float coreInterfaceBrightness[]={1,0.5f};
void* mediumFont;
}
static int starts,posts,busy,wraps; static std::vector<unsigned> colors;
static std::vector<int> centers,rects,printx;
extern "C" unsigned colorScale__FUif(unsigned c,float){colors.push_back(c);return c;}
extern "C" void fontStart__FP4FontiUii(void*,int p,unsigned,int z){assert(p==6&&z==-1);++starts;}
extern "C" void fontColor__FUiii(unsigned,int,int){}
extern "C" void fontPrintCenteredu__FiiPUsi(int x,int y,unsigned short*,int n){assert(y==100&&n==1000000);centers.push_back(x);}
extern "C" int fontPrintWrapu__FiiPUsiiii(int,int,unsigned short*,int,int,int,int){++wraps;return 42;}
extern "C" int fontStringSizeu__FP4FontPUsi(void*,unsigned short*,int){return 41;}
extern "C" void fontPrint__FiiPci(int x,int,char* p,int n){assert(p[0]=='A'&&n==100000);printx.push_back(x);}
extern "C" void fontPrintu__FiiPUsi(int,int,unsigned short*,int){}
extern "C" void fontEnd__Fv(){}
extern "C" void textureDrawBorder__FiiiiiUiPcii(int,int,int,int,int,unsigned,char*,int,int){}
extern "C" void rectangleDraw__FiiiiiUi(int x,int,int right,int,int,unsigned){rects.push_back(x);rects.push_back(right);}
extern "C" unsigned short* textGetString__FPc(char* key){assert(!strcmp(key,"{"));return words;}
extern "C" void scriptActivatePostAction__Fv(){++posts;}
extern "C" int netPlayerIsLocal__Fi(int){return 1;}
extern "C" void msgHudSetPlayerBusy__Fcc(int p,int b){assert((p==0||p==-128)&&b==0);++busy;}
int main(){
 helpMessageOn=1;controlPaused=2;netLocalPad[12]=1;
 helpMessageFrame__Fv();assert(controlPaused==1&&helpMessageOn==1&&starts==1&&gamePause==1&&particlesPaused==1);assert(colors[2]==0x76464646&&printx[0]==279);
 helpMessageFrame__Fv();assert(!helpMessageOn&&controlPaused==60&&gameControlPaused==60&&!gamePause&&!particlesPaused&&posts==1&&busy==2);
 helpMessageOn=1;controlPaused=0;netLocalPad[100]=1;interfaceOn=1;netNetworkMode=1;gamePause=7;GLISTSIZE=2999;
 int old=starts;helpMessageFrame__Fv();assert(starts==old&&gamePause==7&&posts==1&&busy==6);
 helpMessageOn=0;multiHelpMessageOn[0]=multiHelpMessageOn[1]=1;pad[8]=1;pad[12]=1;pad[100]=1;
 helpMessageFrame__Fv();assert(multiHelpMessageOn[0]==1&&multiHelpMessageOn[1]==0&&posts==2);assert(centers[centers.size()-2]==160&&centers.back()==480);assert(rects[rects.size()-2]==354&&rects.back()==622);
 pad[8]=0;pad[12]=0;helpMessageFrame__Fv();assert(multiRelease[0]==1&&multiHelpMessageOn[0]);pad[8]=pad[12]=1;helpMessageFrame__Fv();assert(!multiHelpMessageOn[0]&&posts==3);
}

#define NORRATH_HOST_CONTRACT
#include "run_dialog.cpp"
#include <cassert>
#include <cstring>
#include <vector>
extern "C" {
int swooping,optionsShowing,shopRunning,DLG_StartWithSwoopDone;
int dialogMsgHandle=17,dlgPos,realDlgPos,nmPlayersAlive=2,playersAlive[]={1,0},activePlayer;
int flaredOut,sentEndMsg,DLG_SwoopMode,DLG_ShowDialogText,cancelStreamWait;
int scrollY,scrollTarget,showMore,showOptions,scrollEnd,letterAtATime;
int textBodyChars,textPageDone,fontLastPrintedPosition,nextIndex,optionIndex;
unsigned curDlg,padAcceptButton=1;
signed char netNetworkMode,waitForSound,animLoadingComplete,animStarted;
int lastNmQ;
float correction,cursorTween,scrollSpan,coreInterfaceFadeUpVal;
unsigned char cam[4],*gameCam=cam,player[95936],netLocalPad[352];
char levelName[128];
unsigned short dlgName[4],textBody[8];void* mediumFont;
}
static int advances,doneafter=1,delta,task,queued,preload,played,inits,runs,fonts,restrictions;
static float progress=1,clip,duck;static std::vector<int> sends;
extern "C" int AdvanceDialogSwoop__6CameraiPf(unsigned char* p,int d,float* a){assert(p==cam);++advances;delta=d;*a=progress;return advances>=doneafter;}
extern "C" void worldSetFarClip__Ff(float f){clip=f;}
extern "C" void netSendMessage__FiPUci(int h,unsigned char* p,int n){assert(h==17);sends.push_back(n==2?int(p[0])*256+p[1]:p[0]);}
extern "C" void soundSetMusicDuckAmount__Ff(float f){duck=f;}
extern "C" int engineTaskActive__FPc(char* n){assert(!strcmp(n,"pauseMenu"));return task;}
extern "C" int netPlayerIsLocal__Fi(int n){return n==0;}
extern "C" int soundGetStreamPreloadDone__Fi(int n){assert(n==2);return preload;}
extern "C" int soundGetNmQueued__Fi(int){return queued;}
extern "C" void soundContinueStream__Fi(int){assert(waitForSound==0);++queued;}
extern "C" int soundGetStreamAmountPlayed__Fi(int){return played;}
extern "C" void initAnimation__Fv(){++inits;}
extern "C" void runAnimation__Fv(){++runs;}
extern "C" int soundGetStreamPlayStatus__Fi(int){return 1;}
extern "C" void fontStart__FP4FontiUii(void*,int p,unsigned c,int z){assert(p==6&&c==0x80808080&&z==-1);++fonts;}
extern "C" void fontRestrict__Fi(int n){restrictions=n;}
extern "C" int lineToY__Fi(int n){return n*18;}
extern "C" void fontScissor__Fiiii(int x,int y,int r,int b){assert(x==0&&y==24&&r==0&&b==108);}
extern "C" int fontPrintWrapu__FiiPUsiiii(int x,int y,unsigned short* p,int w,int low,int high,int step){assert(x==56&&y==18-scrollY&&p==textBody&&w==524&&low==295&&high==(DLG_ShowDialogText?385:295)&&step==18);return 0;}
extern "C" void fontEnd__Fv(){}
int main(){
 optionsShowing=7;runDialog__Fv();assert(optionsShowing==0&&fonts==0);
 swooping=1;DLG_StartWithSwoopDone=1;doneafter=3;runDialog__Fv();assert(advances==3&&!swooping&&clip==720&&fonts==0);
 swooping=-1;shopRunning=1;runDialog__Fv();assert(swooping==-1&&advances==3);shopRunning=0;runDialog__Fv();assert(!swooping&&delta==-45&&clip==1800&&sends.back()==3);
 swooping=1;strcpy(levelName+81,"kelethin");dlgName[0]=84;clip=0;runDialog__Fv();assert(clip==0);
 curDlg=1;dlgPos=9;scrollEnd=100;runDialog__Fv();assert(realDlgPos==9&&activePlayer==1&&scrollTarget==72&&scrollSpan==72&&duck==0x1.333332p-2f);
 netNetworkMode=1;DLG_ShowDialogText=1;cursorTween=1;netLocalPad[12]=1;netLocalPad[13]=0x50;netLocalPad[14]=0x30;sends.clear();
 waitForSound=1;animLoadingComplete=1;preload=1;played=1;correction=9;letterAtATime=1;textBodyChars=8;fontLastPrintedPosition=7;nextIndex=2;
 runDialog__Fv();assert((sends==std::vector<int>{767,513,1}));assert(!waitForSound&&inits==1&&runs==1&&correction==0&&animStarted);assert(scrollY==5&&textPageDone==1&&restrictions==8&&optionIndex==2);
 flaredOut=1;coreInterfaceFadeUpVal=1;DLG_SwoopMode=1;sends.clear();runDialog__Fv();assert((sends==std::vector<int>{4})&&sentEndMsg);runDialog__Fv();assert(sends.size()==1);
}

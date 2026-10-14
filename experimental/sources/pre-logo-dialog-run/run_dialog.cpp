// Observed runDialog__Fv at 0x1d2940, 1508 bytes, pinned SLUS_205.65.
// Swoop completion returns before ordinary dialog work. Network sends may
// synchronously mutate dialog state, so subsequent global reads are retained.
// Inferred byte views cover only observed accesses; hardware/EE floats untested.
extern "C" {
extern int swooping,optionsShowing,shopRunning,DLG_StartWithSwoopDone;
extern int dialogMsgHandle,dlgPos,realDlgPos,nmPlayersAlive,playersAlive[],activePlayer;
extern int flaredOut,sentEndMsg,DLG_SwoopMode,DLG_ShowDialogText,cancelStreamWait;
extern int scrollY,scrollTarget,showMore,showOptions,scrollEnd,letterAtATime;
extern int textBodyChars,textPageDone,fontLastPrintedPosition,nextIndex,optionIndex;
extern unsigned curDlg,padAcceptButton;
extern signed char netNetworkMode,waitForSound,animLoadingComplete,animStarted;
extern int lastNmQ asm("lastNmQ.1088");
extern float correction,cursorTween,scrollSpan,coreInterfaceFadeUpVal;
extern unsigned char *gameCam,player[],netLocalPad[];
extern char levelName[];
extern unsigned short dlgName[],textBody[];
extern void* mediumFont;
int AdvanceDialogSwoop__6CameraiPf(unsigned char*,int,float*);
int strcmp(const char*,const char*);
void worldSetFarClip__Ff(float);
void netSendMessage__FiPUci(int,unsigned char*,int);
void soundSetMusicDuckAmount__Ff(float);
int engineTaskActive__FPc(char*);
int netPlayerIsLocal__Fi(int);
int soundGetStreamPreloadDone__Fi(int);
int soundGetNmQueued__Fi(int);
void soundContinueStream__Fi(int);
int soundGetStreamAmountPlayed__Fi(int);
void initAnimation__Fv();
void runAnimation__Fv();
int soundGetStreamPlayStatus__Fi(int);
void fontStart__FP4FontiUii(void*,int,unsigned,int);
void fontRestrict__Fi(int);
int lineToY__Fi(int);
void fontScissor__Fiiii(int,int,int,int);
int fontPrintWrapu__FiiPUsiiii(int,int,unsigned short*,int,int,int,int);
void fontEnd__Fv();
}
static unsigned word(const unsigned char* p) {
 return unsigned(p[0])|unsigned(p[1])<<8|unsigned(p[2])<<16|unsigned(p[3])<<24;
}
static unsigned char* pointer(const unsigned char* p) {
#ifdef NORRATH_HOST_CONTRACT
 unsigned char* v; for(unsigned i=0;i<sizeof(v);++i) reinterpret_cast<unsigned char*>(&v)[i]=p[i];return v;
#else
 return reinterpret_cast<unsigned char*>(word(p));
#endif
}
extern "C" void runDialog__Fv() {
 optionsShowing=0;
 if(swooping) {
  int done=0;
  if(swooping==1) {
   do {
    float amount; done=AdvanceDialogSwoop__6CameraiPf(gameCam,45,&amount);
    if(amount>0.75f && (strcmp(levelName+81,"kelethin") || dlgName[0]!=84)) worldSetFarClip__Ff(720.0f);
   } while(DLG_StartWithSwoopDone && !done);
  } else if(!shopRunning) {
   float amount; done=AdvanceDialogSwoop__6CameraiPf(gameCam,-45,&amount);
   if(amount>0.25f) worldSetFarClip__Ff(1800.0f);
  }
  if(done==1) {
   if(swooping==-1) {unsigned char msg=3;netSendMessage__FiPUci(dialogMsgHandle,&msg,1);}
   swooping=0;
  }
  return;
 }
 if(!curDlg) return;
 soundSetMusicDuckAmount__Ff(0x1.333332p-2f);
 realDlgPos=dlgPos;
 unsigned buttons=0;
 if(!engineTaskActive__FPc(const_cast<char*>("pauseMenu"))) {
  if(netNetworkMode) {
   for(int i=0;i<nmPlayersAlive;++i) if(netPlayerIsLocal__Fi(playersAlive[i])) {
    buttons=word(netLocalPad+playersAlive[i]*88+12);break;
   }
  } else {
   int index=activePlayer;
   auto* entity=pointer(player+index*23984+21872);
   buttons=word(netLocalPad+index*88+12);
   if(!entity || (word(entity+12)&8)) activePlayer=index^1;
  }
 }
 if(waitForSound==1 && soundGetStreamPreloadDone__Fi(2) && animLoadingComplete) {
  waitForSound=0;lastNmQ=soundGetNmQueued__Fi(2);soundContinueStream__Fi(2);
 }
 if(animLoadingComplete && !animStarted && !waitForSound &&
    soundGetNmQueued__Fi(2)!=lastNmQ && soundGetStreamAmountPlayed__Fi(2)>0) {
  initAnimation__Fv();correction=0;animStarted=1;
 }
 if(animStarted) runAnimation__Fv();
 if(flaredOut && coreInterfaceFadeUpVal==1.0f) {
  if(!sentEndMsg) {
   sentEndMsg=1;unsigned char msg=DLG_SwoopMode ? 4 : 3;
   netSendMessage__FiPUci(dialogMsgHandle,&msg,1);
  }
 } else if(DLG_ShowDialogText || (!waitForSound && !soundGetStreamPlayStatus__Fi(2)) || cancelStreamWait) {
  if(cursorTween==1.0f) {
   if(buttons&0x101000) {unsigned char msg[]={2,255};netSendMessage__FiPUci(dialogMsgHandle,msg,2);}
   if(buttons&0x204000) {unsigned char msg[]={2,1};netSendMessage__FiPUci(dialogMsgHandle,msg,2);}
  }
  if(buttons&padAcceptButton) {unsigned char msg=1;netSendMessage__FiPUci(dialogMsgHandle,&msg,1);}
 }
 if(scrollY==scrollTarget && !showMore && !showOptions) {
  if(scrollY!=scrollEnd) {
   if(!DLG_ShowDialogText) {
    int end=scrollY+72; if(end>=scrollEnd) end=scrollEnd;
    scrollTarget=end;scrollSpan=static_cast<float>(end-scrollY);
   } else showMore=1;
  } else {showMore=0;showOptions=1;}
 } else {
  int y=scrollY+5;scrollY=scrollTarget<y ? scrollTarget : y;
 }
 fontStart__FP4FontiUii(mediumFont,6,0x80808080,-1);
 if(letterAtATime) fontRestrict__Fi(textBodyChars);
 int top=lineToY__Fi(1)+6;int bottom=lineToY__Fi(6);
 fontScissor__Fiiii(0,top,0,bottom);
 int y=lineToY__Fi(1);
 fontPrintWrapu__FiiPUsiiii(56,y-scrollY,textBody,524,295,DLG_ShowDialogText?385:295,18);
 fontEnd__Fv();
 if(letterAtATime && !textPageDone) {
  if(fontLastPrintedPosition<textBodyChars) textPageDone=1;
  else ++textBodyChars;
 }
 float tween=cursorTween+0x1.eb851cp-4f;
 if(tween>1.0f) {cursorTween=1.0f;if(nextIndex!=optionIndex) optionIndex=nextIndex;}
 else cursorTween=tween;
}

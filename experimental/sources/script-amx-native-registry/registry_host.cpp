#include <cstdio>
#include <cstring>
extern "C" {
struct NorrathAmxNativeEntry {
    const char* name;
    void (*function)();
};
extern NorrathAmxNativeEntry norrath_amx_game_native_registry_view[];
void amxCallScript__FP5__amxPi() {}
void amxPrint__FP5__amxPi() {}
void amxPrintVal__FP5__amxPi() {}
void amxSound__FP5__amxPi() {}
void amxSoundSequence__FP5__amxPi() {}
void amxLightEffect__FP5__amxPi() {}
void amxSimpleNewWorld__FP5__amxPi() {}
void amxNewWorld__FP5__amxPi() {}
void amxNewWorldPost__FP5__amxPi() {}
void amxActivateShop__FP5__amxPi() {}
void amxShopRunning__FP5__amxPi() {}
void amxGetScaledTime__FP5__amxPi() {}
void amxSetFarClip__FP5__amxPi() {}
void amxStartDialog__FP5__amxPi() {}
void amxChangeSwoop__FP5__amxPi() {}
void amxStartWithSwoopDone__FP5__amxPi() {}
void amxDialogRunning__FP5__amxPi() {}
void amxDeleteMonster__FP5__amxPi() {}
void amxHideMonster__FP5__amxPi() {}
void amxShowMonster__FP5__amxPi() {}
void amxMoveMonster__FP5__amxPi() {}
void amxFollowPlayer__FP5__amxPi() {}
void amxStopFollowingPlayer__FP5__amxPi() {}
void amxFollowPath__FP5__amxPi() {}
void amxStopFollowingPath__FP5__amxPi() {}
void amxFriendly__FP5__amxPi() {}
void amxUnFriendly__FP5__amxPi() {}
void amxStartPropAnim__FP5__amxPi() {}
void amxStopPropAnim__FP5__amxPi() {}
void amxPropAnimAtEnd__FP5__amxPi() {}
void amxStartNPCAnim__FP5__amxPi() {}
void amxNPCAnimAtEnd__FP5__amxPi() {}
void amxAddExpandingPoolUnder__FP5__amxPi() {}
void amxPlayerHasItem__FP5__amxPi() {}
void amxRemovePlayerItem__FP5__amxPi() {}
void amxGivePlayerItem__FP5__amxPi() {}
void amxGivePlayerRandomItem__FP5__amxPi() {}
void amxGivePlayerExp__FP5__amxPi() {}
void amxGivePlayerGold__FP5__amxPi() {}
void amxTakePlayerGold__FP5__amxPi() {}
void amxCheckPlayerGold__FP5__amxPi() {}
void amxPlayGotItemSound__FP5__amxPi() {}
void amxSetInfo__FP5__amxPi() {}
void amxAddHelpMessage__FP5__amxPi() {}
void amxAddQuest__FP5__amxPi() {}
void amxRemoveQuest__FP5__amxPi() {}
void amxHornOmen__FP5__amxPi() {}
void amxPlanarSteel__FP5__amxPi() {}
void amxParticleMorph__FP5__amxPi() {}
void amxHealPlayer__FP5__amxPi() {}
void amxLeaveGame__FP5__amxPi() {}
void amxEndGame__FP5__amxPi() {}
void amxEndGauntlet__FP5__amxPi() {}
void amxClearRecall__FP5__amxPi() {}
void amxStartMusic__FP5__amxPi() {}
void amxStartAudio__FP5__amxPi() {}
void amxEndMusic__FP5__amxPi() {}
void amxSaveGame__FP5__amxPi() {}
void amxSaveEndConfig__FP5__amxPi() {}
void amxSaveDone__FP5__amxPi() {}
void amxExitToMainMenu__FP5__amxPi() {}
void amxReapEnemies__FP5__amxPi() {}
void amxGetv__FP5__amxPi() {}
void amxGetNumPlayers__FP5__amxPi() {}
void amxSetv__FP5__amxPi() {}
void amxAddv__FP5__amxPi() {}
void amxGetRand__FP5__amxPi() {}
void amxActivePlayerIsDwarf__FP5__amxPi() {}
void amxActivePlayerIsElf__FP5__amxPi() {}
void amxGamePaused__FP5__amxPi() {}
void amxPauseGameControl__FP5__amxPi() {}
void amxLevelLoading__FP5__amxPi() {}
void amxSetParticle__FP5__amxPi() {}
void amxAddParticleOnObject__FP5__amxPi() {}
void amxSetNoCollide__FP5__amxPi() {}
void amxShakeCamera__FP5__amxPi() {}
void amxDeleteObjects__FP5__amxPi() {}
void amxHidePlayers__FP5__amxPi() {}
void amxCheckPlayerAlive__FP5__amxPi() {}
void amxSetBestActivePlayer__FP5__amxPi() {}
void amxDecayPlayerLights__FP5__amxPi() {}
void amxCheckIfAnyMonstersRemain__FP5__amxPi() {}
void amxForceMutexRelease__FP5__amxPi() {}
void amxAddDeferedCall__FP5__amxPi() {}
void amxCutscene__FP5__amxPi() {}
void amxSetPostAction__FP5__amxPi() {}
void amxNewWorldParameter__FP5__amxPi() {}
void amxPutNPCInVehicle__FP5__amxPi() {}
void amxPutPlayersInVehicle__FP5__amxPi() {}
void amxEntityRideVehicle__FP5__amxPi() {}
void amxControlVehicle__FP5__amxPi() {}
void amxPlayersLoaded__FP5__amxPi() {}
void amxHoldVehicle__FP5__amxPi() {}
void amxGetPlayerVPosition__FP5__amxPi() {}
void amxGetVPosition__FP5__amxPi() {}
void amxGetPlayerVDir__FP5__amxPi() {}
void amxSetupPlayerSel__FP5__amxPi() {}
void amxStartPlayerSel__FP5__amxPi() {}
void amxPlayerSelRunning__FP5__amxPi() {}
void amxObjectInPlayerTrigger__FP5__amxPi() {}
void amxSetEntrance__FP5__amxPi() {}
void amxHudResetFadeScreen__FP5__amxPi() {}
void amxHudFadeScreen__FP5__amxPi() {}
void amxClosestPlayerDist__FP5__amxPi() {}
void amxHitPlayers__FP5__amxPi() {}
void amxShowCompass__FP5__amxPi() {}
void amxShowBust__FP5__amxPi() {}
void amxGetHitPoints__FP5__amxPi() {}
void amxSetHitPoints__FP5__amxPi() {}
void amxSaveCheckPoint__FP5__amxPi() {}
void amxLoadCheckPoint__FP5__amxPi() {}
void amxTrigger__FP5__amxPi() {}
void amxAutoPropSetState__FP5__amxPi() {}
void amxPreloadResource__FP5__amxPi() {}
void amxSetSaveInhibit__FP5__amxPi() {}
void amxGetSaveInhibit__FP5__amxPi() {}
void amxCountQuestItems__FP5__amxPi() {}
void amxPlayersAlive__FP5__amxPi() {}
void amxCountLiveObjects__FP5__amxPi() {}
void amxNetworkMode__FP5__amxPi() {}
}
int main() {
    const NorrathAmxNativeEntry* entry = norrath_amx_game_native_registry_view;
    int count = 0;
    while (entry[count].name != nullptr) {
        if (entry[count].name[0] == 0 || entry[count].function == nullptr) return 1;
        ++count;
    }
    if (count != 121 || entry[121].function != nullptr) return 2;
    if (std::strcmp(entry[0].name, "callScript") != 0) return 3;
    if (std::strcmp(entry[1].name, "print") != 0) return 4;
    if (std::strcmp(entry[7].name, "newWorld") != 0) return 5;
    if (std::strcmp(entry[8].name, "newWorldAlways") != 0) return 6;
    if (entry[7].function != entry[8].function) return 7;
    if (entry[6].function == entry[7].function) return 8;
    std::puts("native registry passed");
}

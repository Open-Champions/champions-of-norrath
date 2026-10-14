// Host contract for _vt$16VIRealmInterface; not an EE input. Build with the unit beside it:
//   c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp vt.cpp
// Each slot must hold a zero adjustment and call the expected observed name; the last slot is null.
#include <cstdio>
#include <cstring>
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
extern "C" VtSlot vt_16VIRealmInterface[75] asm("_vt$16VIRealmInterface");
static const char* hit = "";
extern "C" void vt_fn_0() asm("__tf16VIRealmInterface");
extern "C" void vt_fn_0() { hit = "__tf16VIRealmInterface"; }
extern "C" void vt_fn_1() asm("_$_16VIRealmInterface");
extern "C" void vt_fn_1() { hit = "_$_16VIRealmInterface"; }
extern "C" void vt_fn_2() asm("onConnect__16VIRealmInterface");
extern "C" void vt_fn_2() { hit = "onConnect__16VIRealmInterface"; }
extern "C" void vt_fn_3() asm("onDisconnect__16VIRealmInterface");
extern "C" void vt_fn_3() { hit = "onDisconnect__16VIRealmInterface"; }
extern "C" void vt_fn_4() asm("onCreateAccount__16VIRealmInterfaceUiiPCUsPv");
extern "C" void vt_fn_4() { hit = "onCreateAccount__16VIRealmInterfaceUiiPCUsPv"; }
extern "C" void vt_fn_5() asm("onConsumeKey__Q212RealmService15RealmGatewayAPIUiiPCUsPv");
extern "C" void vt_fn_5() { hit = "onConsumeKey__Q212RealmService15RealmGatewayAPIUiiPCUsPv"; }
extern "C" void vt_fn_6() asm("onLogin__16VIRealmInterfaceUiiPCUsPv");
extern "C" void vt_fn_6() { hit = "onLogin__16VIRealmInterfaceUiiPCUsPv"; }
extern "C" void vt_fn_7() asm("onLogout__Q212RealmService15RealmGatewayAPIUiiPv");
extern "C" void vt_fn_7() { hit = "onLogout__Q212RealmService15RealmGatewayAPIUiiPv"; }
extern "C" void vt_fn_8() asm("onTouchSession__16VIRealmInterfaceUiiPv");
extern "C" void vt_fn_8() { hit = "onTouchSession__16VIRealmInterfaceUiiPv"; }
extern "C" void vt_fn_9() asm("onCreateAvatar__Q212RealmService15RealmGatewayAPIUiiPCPCUsUiT3UiPv");
extern "C" void vt_fn_9() { hit = "onCreateAvatar__Q212RealmService15RealmGatewayAPIUiiPCPCUsUiT3UiPv"; }
extern "C" void vt_fn_10() asm("onDestroyAvatar__Q212RealmService15RealmGatewayAPIUiiPv");
extern "C" void vt_fn_10() { hit = "onDestroyAvatar__Q212RealmService15RealmGatewayAPIUiiPv"; }
extern "C" void vt_fn_11() asm("onGetPublicRooms__Q212RealmService15RealmGatewayAPIUiiPCPCUsT3PCUiT5UiPv");
extern "C" void vt_fn_11() { hit = "onGetPublicRooms__Q212RealmService15RealmGatewayAPIUiiPCPCUsT3PCUiT5UiPv"; }
extern "C" void vt_fn_12() asm("onGetRoom__Q212RealmService15RealmGatewayAPIUiiPCUsT3PCPCUsUiT5UiPv");
extern "C" void vt_fn_12() { hit = "onGetRoom__Q212RealmService15RealmGatewayAPIUiiPCUsT3PCPCUsUiT5UiPv"; }
extern "C" void vt_fn_13() asm("onCreatePrivateRoom__Q212RealmService15RealmGatewayAPIUiiPCUsT3PCPCUsUiT5UiPv");
extern "C" void vt_fn_13() { hit = "onCreatePrivateRoom__Q212RealmService15RealmGatewayAPIUiiPCUsT3PCPCUsUiT5UiPv"; }
extern "C" void vt_fn_14() asm("onEnterRoom__Q212RealmService15RealmGatewayAPIUiiPCUsT3PCPCUsUiT5UiPv");
extern "C" void vt_fn_14() { hit = "onEnterRoom__Q212RealmService15RealmGatewayAPIUiiPCUsT3PCPCUsUiT5UiPv"; }
extern "C" void vt_fn_15() asm("onLeaveRoom__Q212RealmService15RealmGatewayAPIUiiPv");
extern "C" void vt_fn_15() { hit = "onLeaveRoom__Q212RealmService15RealmGatewayAPIUiiPv"; }
extern "C" void vt_fn_16() asm("onSendRoomMessage__Q212RealmService15RealmGatewayAPIUiiPv");
extern "C" void vt_fn_16() { hit = "onSendRoomMessage__Q212RealmService15RealmGatewayAPIUiiPv"; }
extern "C" void vt_fn_17() asm("onSendInstantMessage__Q212RealmService15RealmGatewayAPIUiiPv");
extern "C" void vt_fn_17() { hit = "onSendInstantMessage__Q212RealmService15RealmGatewayAPIUiiPv"; }
extern "C" void vt_fn_18() asm("onAddFriend__Q212RealmService15RealmGatewayAPIUiiPv");
extern "C" void vt_fn_18() { hit = "onAddFriend__Q212RealmService15RealmGatewayAPIUiiPv"; }
extern "C" void vt_fn_19() asm("onRemoveFriend__Q212RealmService15RealmGatewayAPIUiiPv");
extern "C" void vt_fn_19() { hit = "onRemoveFriend__Q212RealmService15RealmGatewayAPIUiiPv"; }
extern "C" void vt_fn_20() asm("onAddIgnore__Q212RealmService15RealmGatewayAPIUiiPv");
extern "C" void vt_fn_20() { hit = "onAddIgnore__Q212RealmService15RealmGatewayAPIUiiPv"; }
extern "C" void vt_fn_21() asm("onRemoveIgnore__Q212RealmService15RealmGatewayAPIUiiPv");
extern "C" void vt_fn_21() { hit = "onRemoveIgnore__Q212RealmService15RealmGatewayAPIUiiPv"; }
extern "C" void vt_fn_22() asm("onAddRoomModerator__Q212RealmService15RealmGatewayAPIUiiPv");
extern "C" void vt_fn_22() { hit = "onAddRoomModerator__Q212RealmService15RealmGatewayAPIUiiPv"; }
extern "C" void vt_fn_23() asm("onRemoveRoomModerator__Q212RealmService15RealmGatewayAPIUiiPv");
extern "C" void vt_fn_23() { hit = "onRemoveRoomModerator__Q212RealmService15RealmGatewayAPIUiiPv"; }
extern "C" void vt_fn_24() asm("onAddBan__Q212RealmService15RealmGatewayAPIUiiPv");
extern "C" void vt_fn_24() { hit = "onAddBan__Q212RealmService15RealmGatewayAPIUiiPv"; }
extern "C" void vt_fn_25() asm("onRemoveBan__Q212RealmService15RealmGatewayAPIUiiPv");
extern "C" void vt_fn_25() { hit = "onRemoveBan__Q212RealmService15RealmGatewayAPIUiiPv"; }
extern "C" void vt_fn_26() asm("onAddInvite__Q212RealmService15RealmGatewayAPIUiiPv");
extern "C" void vt_fn_26() { hit = "onAddInvite__Q212RealmService15RealmGatewayAPIUiiPv"; }
extern "C" void vt_fn_27() asm("onRemoveInvite__Q212RealmService15RealmGatewayAPIUiiPv");
extern "C" void vt_fn_27() { hit = "onRemoveInvite__Q212RealmService15RealmGatewayAPIUiiPv"; }
extern "C" void vt_fn_28() asm("onKickAvatar__Q212RealmService15RealmGatewayAPIUiiPv");
extern "C" void vt_fn_28() { hit = "onKickAvatar__Q212RealmService15RealmGatewayAPIUiiPv"; }
extern "C" void vt_fn_29() asm("onModifyRoomBanner__Q212RealmService15RealmGatewayAPIUiiPv");
extern "C" void vt_fn_29() { hit = "onModifyRoomBanner__Q212RealmService15RealmGatewayAPIUiiPv"; }
extern "C" void vt_fn_30() asm("onGetUserInfo__Q212RealmService15RealmGatewayAPIUiibT3PCUsPv");
extern "C" void vt_fn_30() { hit = "onGetUserInfo__Q212RealmService15RealmGatewayAPIUiibT3PCUsPv"; }
extern "C" void vt_fn_31() asm("onGetUserStats__Q212RealmService15RealmGatewayAPIUiiRCQ212RealmService9UserStatsPCPCQ212RealmService9UserStatsUiPv");
extern "C" void vt_fn_31() { hit = "onGetUserStats__Q212RealmService15RealmGatewayAPIUiiRCQ212RealmService9UserStatsPCPCQ212RealmService9UserStatsUiPv"; }
extern "C" void vt_fn_32() asm("onGetRealmTime__Q212RealmService15RealmGatewayAPIUiiUiPv");
extern "C" void vt_fn_32() { hit = "onGetRealmTime__Q212RealmService15RealmGatewayAPIUiiUiPv"; }
extern "C" void vt_fn_33() asm("onGetRealmStats__16VIRealmInterfaceUiiUiUiUiUiUiUiUiUiPv");
extern "C" void vt_fn_33() { hit = "onGetRealmStats__16VIRealmInterfaceUiiUiUiUiUiUiUiUiUiPv"; }
extern "C" void vt_fn_34() asm("onCreatePrivateGame__16VIRealmInterfaceUiiPCciPv");
extern "C" void vt_fn_34() { hit = "onCreatePrivateGame__16VIRealmInterfaceUiiPCciPv"; }
extern "C" void vt_fn_35() asm("onCreatePublicGame__16VIRealmInterfaceUiiPCciPv");
extern "C" void vt_fn_35() { hit = "onCreatePublicGame__16VIRealmInterfaceUiiPCciPv"; }
extern "C" void vt_fn_36() asm("onMatchGame__16VIRealmInterfaceiiUiPCPCUsN24PCiPCQ212RealmService4BlobPv");
extern "C" void vt_fn_36() { hit = "onMatchGame__16VIRealmInterfaceiiUiPCPCUsN24PCiPCQ212RealmService4BlobPv"; }
extern "C" void vt_fn_37() asm("onCancelGame__16VIRealmInterfaceUiiPv");
extern "C" void vt_fn_37() { hit = "onCancelGame__16VIRealmInterfaceUiiPv"; }
extern "C" void vt_fn_38() asm("onGetGame__16VIRealmInterfaceiiPCUsT3iPv");
extern "C" void vt_fn_38() { hit = "onGetGame__16VIRealmInterfaceiiPCUsT3iPv"; }
extern "C" void vt_fn_39() asm("onStartGame__Q212RealmService15RealmGatewayAPIUiiUiPv");
extern "C" void vt_fn_39() { hit = "onStartGame__Q212RealmService15RealmGatewayAPIUiiUiPv"; }
extern "C" void vt_fn_40() asm("onRecordGameOutcome__Q212RealmService15RealmGatewayAPIUiiPv");
extern "C" void vt_fn_40() { hit = "onRecordGameOutcome__Q212RealmService15RealmGatewayAPIUiiPv"; }
extern "C" void vt_fn_41() asm("onDoClientDiscovery__16VIRealmInterfaceUiiPCciPv");
extern "C" void vt_fn_41() { hit = "onDoClientDiscovery__16VIRealmInterfaceUiiPCciPv"; }
extern "C" void vt_fn_42() asm("onChangePassword__Q212RealmService15RealmGatewayAPIUiiPv");
extern "C" void vt_fn_42() { hit = "onChangePassword__Q212RealmService15RealmGatewayAPIUiiPv"; }
extern "C" void vt_fn_43() asm("onChangeEmail__Q212RealmService15RealmGatewayAPIUiiPv");
extern "C" void vt_fn_43() { hit = "onChangeEmail__Q212RealmService15RealmGatewayAPIUiiPv"; }
extern "C" void vt_fn_44() asm("onGetRules__16VIRealmInterfaceUiiPCUsUiPv");
extern "C" void vt_fn_44() { hit = "onGetRules__16VIRealmInterfaceUiiPCUsUiPv"; }
extern "C" void vt_fn_45() asm("onUpdateGameData__16VIRealmInterfaceUiiPv");
extern "C" void vt_fn_45() { hit = "onUpdateGameData__16VIRealmInterfaceUiiPv"; }
extern "C" void vt_fn_46() asm("onRoundTrip__Q212RealmService15RealmGatewayAPIUiiPv");
extern "C" void vt_fn_46() { hit = "onRoundTrip__Q212RealmService15RealmGatewayAPIUiiPv"; }
extern "C" void vt_fn_47() asm("notifyInstantMessage__Q212RealmService15RealmGatewayAPIPCUsT1");
extern "C" void vt_fn_47() { hit = "notifyInstantMessage__Q212RealmService15RealmGatewayAPIPCUsT1"; }
extern "C" void vt_fn_48() asm("notifyRoomMessage__Q212RealmService15RealmGatewayAPIPCUsN21");
extern "C" void vt_fn_48() { hit = "notifyRoomMessage__Q212RealmService15RealmGatewayAPIPCUsN21"; }
extern "C" void vt_fn_49() asm("notifyFriendStatus__Q212RealmService15RealmGatewayAPIPCUsb");
extern "C" void vt_fn_49() { hit = "notifyFriendStatus__Q212RealmService15RealmGatewayAPIPCUsb"; }
extern "C" void vt_fn_50() asm("notifyEnterRoom__Q212RealmService15RealmGatewayAPIPCUsT1");
extern "C" void vt_fn_50() { hit = "notifyEnterRoom__Q212RealmService15RealmGatewayAPIPCUsT1"; }
extern "C" void vt_fn_51() asm("notifyLeaveRoom__Q212RealmService15RealmGatewayAPIPCUsT1");
extern "C" void vt_fn_51() { hit = "notifyLeaveRoom__Q212RealmService15RealmGatewayAPIPCUsT1"; }
extern "C" void vt_fn_52() asm("notifyKickRoom__Q212RealmService15RealmGatewayAPIPCUsN21");
extern "C" void vt_fn_52() { hit = "notifyKickRoom__Q212RealmService15RealmGatewayAPIPCUsN21"; }
extern "C" void vt_fn_53() asm("notifyKickAvatar__Q212RealmService15RealmGatewayAPIPCUsT1");
extern "C" void vt_fn_53() { hit = "notifyKickAvatar__Q212RealmService15RealmGatewayAPIPCUsT1"; }
extern "C" void vt_fn_54() asm("notifyModeratorAddedRoom__Q212RealmService15RealmGatewayAPIPCUsN21");
extern "C" void vt_fn_54() { hit = "notifyModeratorAddedRoom__Q212RealmService15RealmGatewayAPIPCUsN21"; }
extern "C" void vt_fn_55() asm("notifyModeratorAddedAvatar__Q212RealmService15RealmGatewayAPIPCUsT1");
extern "C" void vt_fn_55() { hit = "notifyModeratorAddedAvatar__Q212RealmService15RealmGatewayAPIPCUsT1"; }
extern "C" void vt_fn_56() asm("notifyModeratorRemovedRoom__Q212RealmService15RealmGatewayAPIPCUsN21");
extern "C" void vt_fn_56() { hit = "notifyModeratorRemovedRoom__Q212RealmService15RealmGatewayAPIPCUsN21"; }
extern "C" void vt_fn_57() asm("notifyModeratorRemovedAvatar__Q212RealmService15RealmGatewayAPIPCUsT1");
extern "C" void vt_fn_57() { hit = "notifyModeratorRemovedAvatar__Q212RealmService15RealmGatewayAPIPCUsT1"; }
extern "C" void vt_fn_58() asm("notifyAddBanRoom__Q212RealmService15RealmGatewayAPIPCUsN21");
extern "C" void vt_fn_58() { hit = "notifyAddBanRoom__Q212RealmService15RealmGatewayAPIPCUsN21"; }
extern "C" void vt_fn_59() asm("notifyAddBanAvatar__Q212RealmService15RealmGatewayAPIPCUsT1");
extern "C" void vt_fn_59() { hit = "notifyAddBanAvatar__Q212RealmService15RealmGatewayAPIPCUsT1"; }
extern "C" void vt_fn_60() asm("notifyRemoveBanRoom__Q212RealmService15RealmGatewayAPIPCUsN21");
extern "C" void vt_fn_60() { hit = "notifyRemoveBanRoom__Q212RealmService15RealmGatewayAPIPCUsN21"; }
extern "C" void vt_fn_61() asm("notifyRemoveBanAvatar__Q212RealmService15RealmGatewayAPIPCUsT1");
extern "C" void vt_fn_61() { hit = "notifyRemoveBanAvatar__Q212RealmService15RealmGatewayAPIPCUsT1"; }
extern "C" void vt_fn_62() asm("notifyAddInviteRoom__Q212RealmService15RealmGatewayAPIPCUsN21");
extern "C" void vt_fn_62() { hit = "notifyAddInviteRoom__Q212RealmService15RealmGatewayAPIPCUsN21"; }
extern "C" void vt_fn_63() asm("notifyAddInviteAvatar__Q212RealmService15RealmGatewayAPIPCUsT1");
extern "C" void vt_fn_63() { hit = "notifyAddInviteAvatar__Q212RealmService15RealmGatewayAPIPCUsT1"; }
extern "C" void vt_fn_64() asm("notifyRemoveInviteRoom__Q212RealmService15RealmGatewayAPIPCUsN21");
extern "C" void vt_fn_64() { hit = "notifyRemoveInviteRoom__Q212RealmService15RealmGatewayAPIPCUsN21"; }
extern "C" void vt_fn_65() asm("notifyRemoveInviteAvatar__Q212RealmService15RealmGatewayAPIPCUsT1");
extern "C" void vt_fn_65() { hit = "notifyRemoveInviteAvatar__Q212RealmService15RealmGatewayAPIPCUsT1"; }
extern "C" void vt_fn_66() asm("notifyRoomBanner__Q212RealmService15RealmGatewayAPIPCUsT1");
extern "C" void vt_fn_66() { hit = "notifyRoomBanner__Q212RealmService15RealmGatewayAPIPCUsT1"; }
extern "C" void vt_fn_67() asm("notifyGameDiscovered__16VIRealmInterfacePCci");
extern "C" void vt_fn_67() { hit = "notifyGameDiscovered__16VIRealmInterfacePCci"; }
extern "C" void vt_fn_68() asm("notifyClientRequestsConnect__16VIRealmInterfacePCci");
extern "C" void vt_fn_68() { hit = "notifyClientRequestsConnect__16VIRealmInterfacePCci"; }
extern "C" void vt_fn_69() asm("notifyClientDiscovered__16VIRealmInterfacePCci");
extern "C" void vt_fn_69() { hit = "notifyClientDiscovered__16VIRealmInterfacePCci"; }
extern "C" void vt_fn_70() asm("notifyForcedLogout__Q212RealmService15RealmGatewayAPIs");
extern "C" void vt_fn_70() { hit = "notifyForcedLogout__Q212RealmService15RealmGatewayAPIs"; }
extern "C" void vt_fn_71() asm("process__16VIRealmInterface");
extern "C" void vt_fn_71() { hit = "process__16VIRealmInterface"; }
extern "C" void vt_fn_72() asm("createPublicGame__16VIRealmInterfaceiPCUssUiUiUiT2RCQ212RealmService14MatchGameAttrsT2Pv");
extern "C" void vt_fn_72() { hit = "createPublicGame__16VIRealmInterfaceiPCUssUiUiUiT2RCQ212RealmService14MatchGameAttrsT2Pv"; }
extern "C" void vt_fn_73() asm("createPrivateGame__16VIRealmInterfaceiPCUsN22Pv");
extern "C" void vt_fn_73() { hit = "createPrivateGame__16VIRealmInterfaceiPCUsN22Pv"; }
int main() {
    const char* want[] = {
        "__tf16VIRealmInterface",
        "_$_16VIRealmInterface",
        "onConnect__16VIRealmInterface",
        "onDisconnect__16VIRealmInterface",
        "onCreateAccount__16VIRealmInterfaceUiiPCUsPv",
        "onConsumeKey__Q212RealmService15RealmGatewayAPIUiiPCUsPv",
        "onLogin__16VIRealmInterfaceUiiPCUsPv",
        "onLogout__Q212RealmService15RealmGatewayAPIUiiPv",
        "onTouchSession__16VIRealmInterfaceUiiPv",
        "onCreateAvatar__Q212RealmService15RealmGatewayAPIUiiPCPCUsUiT3UiPv",
        "onDestroyAvatar__Q212RealmService15RealmGatewayAPIUiiPv",
        "onGetPublicRooms__Q212RealmService15RealmGatewayAPIUiiPCPCUsT3PCUiT5UiPv",
        "onGetRoom__Q212RealmService15RealmGatewayAPIUiiPCUsT3PCPCUsUiT5UiPv",
        "onCreatePrivateRoom__Q212RealmService15RealmGatewayAPIUiiPCUsT3PCPCUsUiT5UiPv",
        "onEnterRoom__Q212RealmService15RealmGatewayAPIUiiPCUsT3PCPCUsUiT5UiPv",
        "onLeaveRoom__Q212RealmService15RealmGatewayAPIUiiPv",
        "onSendRoomMessage__Q212RealmService15RealmGatewayAPIUiiPv",
        "onSendInstantMessage__Q212RealmService15RealmGatewayAPIUiiPv",
        "onAddFriend__Q212RealmService15RealmGatewayAPIUiiPv",
        "onRemoveFriend__Q212RealmService15RealmGatewayAPIUiiPv",
        "onAddIgnore__Q212RealmService15RealmGatewayAPIUiiPv",
        "onRemoveIgnore__Q212RealmService15RealmGatewayAPIUiiPv",
        "onAddRoomModerator__Q212RealmService15RealmGatewayAPIUiiPv",
        "onRemoveRoomModerator__Q212RealmService15RealmGatewayAPIUiiPv",
        "onAddBan__Q212RealmService15RealmGatewayAPIUiiPv",
        "onRemoveBan__Q212RealmService15RealmGatewayAPIUiiPv",
        "onAddInvite__Q212RealmService15RealmGatewayAPIUiiPv",
        "onRemoveInvite__Q212RealmService15RealmGatewayAPIUiiPv",
        "onKickAvatar__Q212RealmService15RealmGatewayAPIUiiPv",
        "onModifyRoomBanner__Q212RealmService15RealmGatewayAPIUiiPv",
        "onGetUserInfo__Q212RealmService15RealmGatewayAPIUiibT3PCUsPv",
        "onGetUserStats__Q212RealmService15RealmGatewayAPIUiiRCQ212RealmService9UserStatsPCPCQ212RealmService9UserStatsUiPv",
        "onGetRealmTime__Q212RealmService15RealmGatewayAPIUiiUiPv",
        "onGetRealmStats__16VIRealmInterfaceUiiUiUiUiUiUiUiUiUiPv",
        "onCreatePrivateGame__16VIRealmInterfaceUiiPCciPv",
        "onCreatePublicGame__16VIRealmInterfaceUiiPCciPv",
        "onMatchGame__16VIRealmInterfaceiiUiPCPCUsN24PCiPCQ212RealmService4BlobPv",
        "onCancelGame__16VIRealmInterfaceUiiPv",
        "onGetGame__16VIRealmInterfaceiiPCUsT3iPv",
        "onStartGame__Q212RealmService15RealmGatewayAPIUiiUiPv",
        "onRecordGameOutcome__Q212RealmService15RealmGatewayAPIUiiPv",
        "onDoClientDiscovery__16VIRealmInterfaceUiiPCciPv",
        "onChangePassword__Q212RealmService15RealmGatewayAPIUiiPv",
        "onChangeEmail__Q212RealmService15RealmGatewayAPIUiiPv",
        "onGetRules__16VIRealmInterfaceUiiPCUsUiPv",
        "onUpdateGameData__16VIRealmInterfaceUiiPv",
        "onRoundTrip__Q212RealmService15RealmGatewayAPIUiiPv",
        "notifyInstantMessage__Q212RealmService15RealmGatewayAPIPCUsT1",
        "notifyRoomMessage__Q212RealmService15RealmGatewayAPIPCUsN21",
        "notifyFriendStatus__Q212RealmService15RealmGatewayAPIPCUsb",
        "notifyEnterRoom__Q212RealmService15RealmGatewayAPIPCUsT1",
        "notifyLeaveRoom__Q212RealmService15RealmGatewayAPIPCUsT1",
        "notifyKickRoom__Q212RealmService15RealmGatewayAPIPCUsN21",
        "notifyKickAvatar__Q212RealmService15RealmGatewayAPIPCUsT1",
        "notifyModeratorAddedRoom__Q212RealmService15RealmGatewayAPIPCUsN21",
        "notifyModeratorAddedAvatar__Q212RealmService15RealmGatewayAPIPCUsT1",
        "notifyModeratorRemovedRoom__Q212RealmService15RealmGatewayAPIPCUsN21",
        "notifyModeratorRemovedAvatar__Q212RealmService15RealmGatewayAPIPCUsT1",
        "notifyAddBanRoom__Q212RealmService15RealmGatewayAPIPCUsN21",
        "notifyAddBanAvatar__Q212RealmService15RealmGatewayAPIPCUsT1",
        "notifyRemoveBanRoom__Q212RealmService15RealmGatewayAPIPCUsN21",
        "notifyRemoveBanAvatar__Q212RealmService15RealmGatewayAPIPCUsT1",
        "notifyAddInviteRoom__Q212RealmService15RealmGatewayAPIPCUsN21",
        "notifyAddInviteAvatar__Q212RealmService15RealmGatewayAPIPCUsT1",
        "notifyRemoveInviteRoom__Q212RealmService15RealmGatewayAPIPCUsN21",
        "notifyRemoveInviteAvatar__Q212RealmService15RealmGatewayAPIPCUsT1",
        "notifyRoomBanner__Q212RealmService15RealmGatewayAPIPCUsT1",
        "notifyGameDiscovered__16VIRealmInterfacePCci",
        "notifyClientRequestsConnect__16VIRealmInterfacePCci",
        "notifyClientDiscovered__16VIRealmInterfacePCci",
        "notifyForcedLogout__Q212RealmService15RealmGatewayAPIs",
        "process__16VIRealmInterface",
        "createPublicGame__16VIRealmInterfaceiPCUssUiUiUiT2RCQ212RealmService14MatchGameAttrsT2Pv",
        "createPrivateGame__16VIRealmInterfaceiPCUsN22Pv",
        nullptr,
    };
    int bad = 0;
    if (sizeof vt_16VIRealmInterface / sizeof vt_16VIRealmInterface[0] != 75) { std::printf("count\n"); ++bad; }
    for (int i = 0; i < 75; ++i) {
        if (vt_16VIRealmInterface[i].adjustment != 0u) { std::printf("adjust %d\n", i); ++bad; }
        if (want[i] == nullptr) { if (vt_16VIRealmInterface[i].function != nullptr) { std::printf("last %d\n", i); ++bad; } continue; }
        hit = "";
        vt_16VIRealmInterface[i].function();
        if (std::strcmp(hit, want[i]) != 0) { std::printf("slot %d\n", i); ++bad; }
    }
    std::printf(bad ? "failed\n" : "_vt$16VIRealmInterface slots ok\n");
    return bad != 0;
}

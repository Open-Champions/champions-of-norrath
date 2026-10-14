// Observed _vt$16VIRealmInterface at 0x01210298, 600 bytes (75 slots of 8 bytes) in the image data.
// Each slot is a zero adjustment word and a function word. The function words, in slot order, are the
// image symbols named below; a null function word is written as nullptr. The bodies of these functions
// are not in this unit, so every slot refers to the observed name and this unit adds no definitions.
// Inferred: the first slot is the type-info function of the class and the second its destructor, as in
// the other tables of this kind. Unknown: the owner of each named function beyond the name itself.
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
#if defined(__mips__)
static_assert(sizeof(VtSlot) == 8, "slot is an adjustment word plus a pointer");
#endif
extern "C" {
void vt_fn_0() asm("__tf16VIRealmInterface");
void vt_fn_1() asm("_$_16VIRealmInterface");
void vt_fn_2() asm("onConnect__16VIRealmInterface");
void vt_fn_3() asm("onDisconnect__16VIRealmInterface");
void vt_fn_4() asm("onCreateAccount__16VIRealmInterfaceUiiPCUsPv");
void vt_fn_5() asm("onConsumeKey__Q212RealmService15RealmGatewayAPIUiiPCUsPv");
void vt_fn_6() asm("onLogin__16VIRealmInterfaceUiiPCUsPv");
void vt_fn_7() asm("onLogout__Q212RealmService15RealmGatewayAPIUiiPv");
void vt_fn_8() asm("onTouchSession__16VIRealmInterfaceUiiPv");
void vt_fn_9() asm("onCreateAvatar__Q212RealmService15RealmGatewayAPIUiiPCPCUsUiT3UiPv");
void vt_fn_10() asm("onDestroyAvatar__Q212RealmService15RealmGatewayAPIUiiPv");
void vt_fn_11() asm("onGetPublicRooms__Q212RealmService15RealmGatewayAPIUiiPCPCUsT3PCUiT5UiPv");
void vt_fn_12() asm("onGetRoom__Q212RealmService15RealmGatewayAPIUiiPCUsT3PCPCUsUiT5UiPv");
void vt_fn_13() asm("onCreatePrivateRoom__Q212RealmService15RealmGatewayAPIUiiPCUsT3PCPCUsUiT5UiPv");
void vt_fn_14() asm("onEnterRoom__Q212RealmService15RealmGatewayAPIUiiPCUsT3PCPCUsUiT5UiPv");
void vt_fn_15() asm("onLeaveRoom__Q212RealmService15RealmGatewayAPIUiiPv");
void vt_fn_16() asm("onSendRoomMessage__Q212RealmService15RealmGatewayAPIUiiPv");
void vt_fn_17() asm("onSendInstantMessage__Q212RealmService15RealmGatewayAPIUiiPv");
void vt_fn_18() asm("onAddFriend__Q212RealmService15RealmGatewayAPIUiiPv");
void vt_fn_19() asm("onRemoveFriend__Q212RealmService15RealmGatewayAPIUiiPv");
void vt_fn_20() asm("onAddIgnore__Q212RealmService15RealmGatewayAPIUiiPv");
void vt_fn_21() asm("onRemoveIgnore__Q212RealmService15RealmGatewayAPIUiiPv");
void vt_fn_22() asm("onAddRoomModerator__Q212RealmService15RealmGatewayAPIUiiPv");
void vt_fn_23() asm("onRemoveRoomModerator__Q212RealmService15RealmGatewayAPIUiiPv");
void vt_fn_24() asm("onAddBan__Q212RealmService15RealmGatewayAPIUiiPv");
void vt_fn_25() asm("onRemoveBan__Q212RealmService15RealmGatewayAPIUiiPv");
void vt_fn_26() asm("onAddInvite__Q212RealmService15RealmGatewayAPIUiiPv");
void vt_fn_27() asm("onRemoveInvite__Q212RealmService15RealmGatewayAPIUiiPv");
void vt_fn_28() asm("onKickAvatar__Q212RealmService15RealmGatewayAPIUiiPv");
void vt_fn_29() asm("onModifyRoomBanner__Q212RealmService15RealmGatewayAPIUiiPv");
void vt_fn_30() asm("onGetUserInfo__Q212RealmService15RealmGatewayAPIUiibT3PCUsPv");
void vt_fn_31() asm("onGetUserStats__Q212RealmService15RealmGatewayAPIUiiRCQ212RealmService9UserStatsPCPCQ212RealmService9UserStatsUiPv");
void vt_fn_32() asm("onGetRealmTime__Q212RealmService15RealmGatewayAPIUiiUiPv");
void vt_fn_33() asm("onGetRealmStats__16VIRealmInterfaceUiiUiUiUiUiUiUiUiUiPv");
void vt_fn_34() asm("onCreatePrivateGame__16VIRealmInterfaceUiiPCciPv");
void vt_fn_35() asm("onCreatePublicGame__16VIRealmInterfaceUiiPCciPv");
void vt_fn_36() asm("onMatchGame__16VIRealmInterfaceiiUiPCPCUsN24PCiPCQ212RealmService4BlobPv");
void vt_fn_37() asm("onCancelGame__16VIRealmInterfaceUiiPv");
void vt_fn_38() asm("onGetGame__16VIRealmInterfaceiiPCUsT3iPv");
void vt_fn_39() asm("onStartGame__Q212RealmService15RealmGatewayAPIUiiUiPv");
void vt_fn_40() asm("onRecordGameOutcome__Q212RealmService15RealmGatewayAPIUiiPv");
void vt_fn_41() asm("onDoClientDiscovery__16VIRealmInterfaceUiiPCciPv");
void vt_fn_42() asm("onChangePassword__Q212RealmService15RealmGatewayAPIUiiPv");
void vt_fn_43() asm("onChangeEmail__Q212RealmService15RealmGatewayAPIUiiPv");
void vt_fn_44() asm("onGetRules__16VIRealmInterfaceUiiPCUsUiPv");
void vt_fn_45() asm("onUpdateGameData__16VIRealmInterfaceUiiPv");
void vt_fn_46() asm("onRoundTrip__Q212RealmService15RealmGatewayAPIUiiPv");
void vt_fn_47() asm("notifyInstantMessage__Q212RealmService15RealmGatewayAPIPCUsT1");
void vt_fn_48() asm("notifyRoomMessage__Q212RealmService15RealmGatewayAPIPCUsN21");
void vt_fn_49() asm("notifyFriendStatus__Q212RealmService15RealmGatewayAPIPCUsb");
void vt_fn_50() asm("notifyEnterRoom__Q212RealmService15RealmGatewayAPIPCUsT1");
void vt_fn_51() asm("notifyLeaveRoom__Q212RealmService15RealmGatewayAPIPCUsT1");
void vt_fn_52() asm("notifyKickRoom__Q212RealmService15RealmGatewayAPIPCUsN21");
void vt_fn_53() asm("notifyKickAvatar__Q212RealmService15RealmGatewayAPIPCUsT1");
void vt_fn_54() asm("notifyModeratorAddedRoom__Q212RealmService15RealmGatewayAPIPCUsN21");
void vt_fn_55() asm("notifyModeratorAddedAvatar__Q212RealmService15RealmGatewayAPIPCUsT1");
void vt_fn_56() asm("notifyModeratorRemovedRoom__Q212RealmService15RealmGatewayAPIPCUsN21");
void vt_fn_57() asm("notifyModeratorRemovedAvatar__Q212RealmService15RealmGatewayAPIPCUsT1");
void vt_fn_58() asm("notifyAddBanRoom__Q212RealmService15RealmGatewayAPIPCUsN21");
void vt_fn_59() asm("notifyAddBanAvatar__Q212RealmService15RealmGatewayAPIPCUsT1");
void vt_fn_60() asm("notifyRemoveBanRoom__Q212RealmService15RealmGatewayAPIPCUsN21");
void vt_fn_61() asm("notifyRemoveBanAvatar__Q212RealmService15RealmGatewayAPIPCUsT1");
void vt_fn_62() asm("notifyAddInviteRoom__Q212RealmService15RealmGatewayAPIPCUsN21");
void vt_fn_63() asm("notifyAddInviteAvatar__Q212RealmService15RealmGatewayAPIPCUsT1");
void vt_fn_64() asm("notifyRemoveInviteRoom__Q212RealmService15RealmGatewayAPIPCUsN21");
void vt_fn_65() asm("notifyRemoveInviteAvatar__Q212RealmService15RealmGatewayAPIPCUsT1");
void vt_fn_66() asm("notifyRoomBanner__Q212RealmService15RealmGatewayAPIPCUsT1");
void vt_fn_67() asm("notifyGameDiscovered__16VIRealmInterfacePCci");
void vt_fn_68() asm("notifyClientRequestsConnect__16VIRealmInterfacePCci");
void vt_fn_69() asm("notifyClientDiscovered__16VIRealmInterfacePCci");
void vt_fn_70() asm("notifyForcedLogout__Q212RealmService15RealmGatewayAPIs");
void vt_fn_71() asm("process__16VIRealmInterface");
void vt_fn_72() asm("createPublicGame__16VIRealmInterfaceiPCUssUiUiUiT2RCQ212RealmService14MatchGameAttrsT2Pv");
void vt_fn_73() asm("createPrivateGame__16VIRealmInterfaceiPCUsN22Pv");
VtSlot vt_16VIRealmInterface[75] asm("_vt$16VIRealmInterface") = {
    {0x0u, vt_fn_0},
    {0x0u, vt_fn_1},
    {0x0u, vt_fn_2},
    {0x0u, vt_fn_3},
    {0x0u, vt_fn_4},
    {0x0u, vt_fn_5},
    {0x0u, vt_fn_6},
    {0x0u, vt_fn_7},
    {0x0u, vt_fn_8},
    {0x0u, vt_fn_9},
    {0x0u, vt_fn_10},
    {0x0u, vt_fn_11},
    {0x0u, vt_fn_12},
    {0x0u, vt_fn_13},
    {0x0u, vt_fn_14},
    {0x0u, vt_fn_15},
    {0x0u, vt_fn_16},
    {0x0u, vt_fn_17},
    {0x0u, vt_fn_18},
    {0x0u, vt_fn_19},
    {0x0u, vt_fn_20},
    {0x0u, vt_fn_21},
    {0x0u, vt_fn_22},
    {0x0u, vt_fn_23},
    {0x0u, vt_fn_24},
    {0x0u, vt_fn_25},
    {0x0u, vt_fn_26},
    {0x0u, vt_fn_27},
    {0x0u, vt_fn_28},
    {0x0u, vt_fn_29},
    {0x0u, vt_fn_30},
    {0x0u, vt_fn_31},
    {0x0u, vt_fn_32},
    {0x0u, vt_fn_33},
    {0x0u, vt_fn_34},
    {0x0u, vt_fn_35},
    {0x0u, vt_fn_36},
    {0x0u, vt_fn_37},
    {0x0u, vt_fn_38},
    {0x0u, vt_fn_39},
    {0x0u, vt_fn_40},
    {0x0u, vt_fn_41},
    {0x0u, vt_fn_42},
    {0x0u, vt_fn_43},
    {0x0u, vt_fn_44},
    {0x0u, vt_fn_45},
    {0x0u, vt_fn_46},
    {0x0u, vt_fn_47},
    {0x0u, vt_fn_48},
    {0x0u, vt_fn_49},
    {0x0u, vt_fn_50},
    {0x0u, vt_fn_51},
    {0x0u, vt_fn_52},
    {0x0u, vt_fn_53},
    {0x0u, vt_fn_54},
    {0x0u, vt_fn_55},
    {0x0u, vt_fn_56},
    {0x0u, vt_fn_57},
    {0x0u, vt_fn_58},
    {0x0u, vt_fn_59},
    {0x0u, vt_fn_60},
    {0x0u, vt_fn_61},
    {0x0u, vt_fn_62},
    {0x0u, vt_fn_63},
    {0x0u, vt_fn_64},
    {0x0u, vt_fn_65},
    {0x0u, vt_fn_66},
    {0x0u, vt_fn_67},
    {0x0u, vt_fn_68},
    {0x0u, vt_fn_69},
    {0x0u, vt_fn_70},
    {0x0u, vt_fn_71},
    {0x0u, vt_fn_72},
    {0x0u, vt_fn_73},
    {0x0u, nullptr},
};
}

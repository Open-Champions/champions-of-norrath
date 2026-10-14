# w16-realm-a: online realm service interface (part 1)

Branch `agents/w16-realm-a`. Reference image: the private `SLUS_205.65` (SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`). Addresses and sizes are that image's own symbol
values; nothing from it (bytes, disassembly, decompiler output) is committed. Nothing here has run on an EE or in the game.

## What was done

36 functions, 36 units, 36 commits, one function per commit, under `experimental/sources/w16-realm-a-*`:

- 29 do-nothing defaults of `Q212RealmService15RealmGatewayAPI` (`on*` requests and `notify*` callbacks). Each is 8 bytes in the
  image (a return and its delay slot) and a weak symbol. They are `w16-realm-a-gateway-*`.
- 7 members of `VIRealmInterface` (`w16-realm-a-vi-realm-interface-*`):

| Symbol | Address | Image bytes | Compiled bytes |
|---|---|---|---|
| `notifyGameDiscovered__16VIRealmInterfacePCci` | `0x011093a0` | 152 | 144 |
| `notifyClientDiscovered__16VIRealmInterfacePCci` | `0x01109000` | 196 | 184 |
| `notifyClientRequestsConnect__16VIRealmInterfacePCci` | `0x011090c8` | 304 | 296 |
| `onCancelGame__16VIRealmInterfaceUiiPv` | `0x01108980` | 372 | 308 |
| `createPrivateGame__16VIRealmInterfaceiPCUsN22Pv` | `0x01103c50` | 848 | 616 (+104 local helper) |
| `createPublicGame__16VIRealmInterfaceiPCUssUiUiUiT2RCQ212RealmService14MatchGameAttrsT2Pv` | `0x011038d0` | 896 | 664 (+104 local helper) |
| `onConnect__16VIRealmInterface` | `0x01103fa0` | 2056 | 136 (+608 and +104 local helpers) |

No byte match is claimed; the compiler repacks the inlined string-library steps.

## Observed

- The 29 defaults are `jr ra; nop` in the image (checked word by word for every one).
- `notifyGameDiscovered` / `notifyClientDiscovered`: print a line, copy the address into the global `g_localIp` and the port into
  `g_localPort`, set the state word at object+3468 (2 for a game, 11 for a client), and call `GetInetAddress__11VIUDPSocket...(this+3396,
  this+38012, g_localIp, 1000)`. The game version also copies the address to this+2348 and the port to this+3372; the client version
  stamps `GetTime__7VITimer()` at this+9720 and puts string 1856 into the page behind `(this+12)->+52` under the key `ServerStatus`.
- `notifyClientRequestsConnect`: calls `strcmp(name, entry)` for the first `count-1` entries of a table of 1144-byte entries at
  this+5088 and discards the result; when `count < limit` (words at +3476 and +3472) it initialises entry `count-1`: memset 1144, `snprintf(entry, 1023, name)`
  (the name is the format), port at entry+1024, 7 at entry+1048, time at entry+1056, memset of 52 bytes at entry+1092, then
  `UpdateNumPlayers__16VIRealmInterfacei(this, count+1)`.
- `onCancelGame`: clears the state word at +3468, calls `Reset__16VIRealmInterface`, prints the two arguments, and updates the dialog
  behind `(this+12)->+56`: status 0 unlocks, shows string 1840 and sets text 1854; status 1 sets text 1861; other statuses show 1835
  and set text 1855.
- `createPrivateGame` / `createPublicGame`: store the count at +3472, copy a 16-bit string into an old reference-counted string,
  narrow it to its low bytes, `snprintf` it (as a format) into the buffer at +276, call the gateway base routine of the same name, and
  release the strings. `createPublicGame` sign-extends its 16-bit argument itself and takes three more arguments from the stack area.
- `onConnect`: sets the global `connected` to 1, prints a line, builds two 16-bit strings from the literals at `0x011d8e60` and
  `0x011d8e68` (three ASCII letters each) with sign-extending widening and a trailing zero character, calls
  `login__Q212RealmService15RealmGatewayAPIPCUsT1Pv(this, user, password, 0)` and releases the strings in the original order.
- The string layout used by the three string routines is the one already read in the realm-interface constructor/destructor
  sources: a string is one word (the data pointer); 16 bytes before it are a length, a capacity, a reference count and a flag word;
  a block of `capacity * character size + 16` bytes below 129 returns to `_t24__default_alloc_template2b0i0$_S_free_list` at index
  `((bytes + 23) >> 3) - 1`, otherwise to `__wrap_free`. The store loops set the flag word to 1 before each store, so a later copy
  is a `clone`, not a shared count.

## Inferred (labelled so, not proven)

- The callbacks belong to LAN discovery and the realm connection state of `VIRealmInterface`; the state word at +3468 is a
  state number (2, 11 and 0 are stored). The meaning of the numbers is unknown.
- The `createPublicGame` buffer at +276 receives a narrowed name; the "public" and "private" routines share the same text step.
- The 29 defaults are inline virtual defaults of the base class; derived classes that override them are not part of this package.

## Unknown

- The meaning of every numeric argument of `createPublicGame`, of `first`/`last`/`context` and of the words at +3472, +3468,
  +3476, +9720; what string ids 1835, 1840, 1854, 1855, 1856 and 1861 say; the structure of the 1144-byte entry beyond the offsets
  above; the size of the address record at +38012; what `1000` passed to `GetInetAddress` means; why the strcmp result is dropped.
- Whether the format-string uses of caller-supplied text (`snprintf(buf, 1023, text)` in three routines) are intended; they are what
  the image does and the sources keep them.
- Whether `foo`/`bar` in `onConnect` are placeholders. The sign extension of the widening step cannot be observed with those literals.

## Checks performed

- Every unit compiles alone with the reviewed `authored_cpp` flags (EE GCC 15.2.0).
- Every unit has a `host_check.cpp` (not a build input) built with the host compiler, with stubs that record call order and arguments.
  The host checks of the three string routines use `w16-realm-a-common/host_string_model.hpp` (host-only model of the string library).
  Mutating an offset, constant, release size or call order makes the host check fail (details in the commit messages).
- Every unit was run against the original in a private R5900 interpreter (outside git): identical memory snapshots, identical Python
  models of the callees, equal call logs and equal changed bytes outside the stack; `HALT` is counted as a failure. Scenario counts,
  coverage and the mutations that fail are in each commit message. Covered original words: 2/2 for each default, 38/38, 49/49,
  76/76, 92/93, 209/212, 219/224 and 496/514; the uncovered words are padding words and copy-by-count paths that the store loops make
  unreachable.
- A private partial build (plan copy kept outside git, made with `docs/agents/w16-realm-a-plan-entries.py --private-root`) compiled
  and linked the shared selection plus these 36 units: 7363 compilations, one link, unresolved imports 841 to 827, with exactly the
  36 assigned names closed and 22 names opened.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.

## Imports closed (36)

`createPrivateGame__16VIRealmInterfaceiPCUsN22Pv`, `createPublicGame__16VIRealmInterfaceiPCUssUiUiUiT2RCQ212RealmService14MatchGameAttrsT2Pv`, `notifyAddBanAvatar__Q212RealmService15RealmGatewayAPIPCUsT1`, `notifyAddBanRoom__Q212RealmService15RealmGatewayAPIPCUsN21`, `notifyAddInviteAvatar__Q212RealmService15RealmGatewayAPIPCUsT1`, `notifyAddInviteRoom__Q212RealmService15RealmGatewayAPIPCUsN21`, `notifyClientDiscovered__16VIRealmInterfacePCci`, `notifyClientRequestsConnect__16VIRealmInterfacePCci`, `notifyEnterRoom__Q212RealmService15RealmGatewayAPIPCUsT1`, `notifyForcedLogout__Q212RealmService15RealmGatewayAPIs`, `notifyFriendStatus__Q212RealmService15RealmGatewayAPIPCUsb`, `notifyGameDiscovered__16VIRealmInterfacePCci`, `notifyInstantMessage__Q212RealmService15RealmGatewayAPIPCUsT1`, `notifyKickAvatar__Q212RealmService15RealmGatewayAPIPCUsT1`, `notifyKickRoom__Q212RealmService15RealmGatewayAPIPCUsN21`, `notifyLeaveRoom__Q212RealmService15RealmGatewayAPIPCUsT1`, `notifyModeratorAddedAvatar__Q212RealmService15RealmGatewayAPIPCUsT1`, `notifyModeratorAddedRoom__Q212RealmService15RealmGatewayAPIPCUsN21`, `notifyModeratorRemovedAvatar__Q212RealmService15RealmGatewayAPIPCUsT1`, `notifyModeratorRemovedRoom__Q212RealmService15RealmGatewayAPIPCUsN21`, `notifyRemoveBanAvatar__Q212RealmService15RealmGatewayAPIPCUsT1`, `notifyRemoveBanRoom__Q212RealmService15RealmGatewayAPIPCUsN21`, `notifyRemoveInviteAvatar__Q212RealmService15RealmGatewayAPIPCUsT1`, `notifyRemoveInviteRoom__Q212RealmService15RealmGatewayAPIPCUsN21`, `notifyRoomBanner__Q212RealmService15RealmGatewayAPIPCUsT1`, `notifyRoomMessage__Q212RealmService15RealmGatewayAPIPCUsN21`, `onAddBan__Q212RealmService15RealmGatewayAPIUiiPv`, `onAddFriend__Q212RealmService15RealmGatewayAPIUiiPv`, `onAddIgnore__Q212RealmService15RealmGatewayAPIUiiPv`, `onAddInvite__Q212RealmService15RealmGatewayAPIUiiPv`, `onAddRoomModerator__Q212RealmService15RealmGatewayAPIUiiPv`, `onCancelGame__16VIRealmInterfaceUiiPv`, `onChangeEmail__Q212RealmService15RealmGatewayAPIUiiPv`, `onChangePassword__Q212RealmService15RealmGatewayAPIUiiPv`, `onConnect__16VIRealmInterface`, `onConsumeKey__Q212RealmService15RealmGatewayAPIUiiPCUsPv`

## Imports opened (22)

`GetInetAddress__11VIUDPSocketP13VIInetAddressPCci`, `Reset__16VIRealmInterface`, `SetStringValue__9VIGUIPagePCcPCUsb`, `SetText__11VIGUIDialogPCUsb`, `Show__11VIGUIDialogPCUsPCc`, `UnLock__11VIGUIDialog`, `UpdateNumPlayers__16VIRealmInterfacei`, `_t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0$nilRep`, `alloc__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0Uib`, `alloc__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uib`, `clone__Q2t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0_3Rep`, `connected`, `createPrivateGame__Q212RealmService15RealmGatewayAPIPCUsN21Pv`, `createPublicGame__Q212RealmService15RealmGatewayAPIPCUssUiUiUiT1RCQ212RealmService14MatchGameAttrsT1Pv`, `g_localPort`, `login__Q212RealmService15RealmGatewayAPIPCUsT1Pv`, `replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiPCUsUi`, `replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiUiUs`, `replace__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0UiUiPCcUi`, `resize__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUs`, `resize__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uic`, `snprintf`

Of these, `Reset__16VIRealmInterface`, `UpdateNumPlayers__16VIRealmInterfacei`, `createPrivateGame__Q212RealmService15RealmGatewayAPIPCUsN21Pv`,
`createPublicGame__Q212RealmService15RealmGatewayAPIPCUssUiUiUiT1RCQ212RealmService14MatchGameAttrsT1Pv` and `login__Q212RealmService15RealmGatewayAPIPCUsT1Pv`
are realm-service functions that belong to the neighbouring realm packages; `GetInetAddress__11VIUDPSocket...` is network library code;
`SetText`/`Show`/`UnLock` are `VIGUIDialog` members; `connected` and `g_localPort` are plain globals (4 bytes each in the image,
`0x011b017c` and `0x011b20e0`, whose owners are not part of this package).

## Build-plan entries needed

Profile `authored_cpp`; the integrator assigns `input_order` and adds the sources to the snapshot. Do not add `host_check.cpp`.
`w16-realm-a-common` is a host-check header only. `docs/agents/w16-realm-a-plan-entries.py --toolchain <ee>` prints the entries
(and with `--private-root DIR` writes a private plan/snapshot copy). The entries as printed at the last commit:

```json
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "notifyAddBanAvatar__Q212RealmService15RealmGatewayAPIPCUsT1"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_notify_add_ban_avatar.o", "profile": "authored_cpp", "sha256": "534a9c873e4c7e3dd12d0a53ccc14d55877ea6bba1374a6ddb9a3b7b216695e1", "source": "sources/w16-realm-a-gateway-notify-add-ban-avatar/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "notifyAddBanRoom__Q212RealmService15RealmGatewayAPIPCUsN21"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_notify_add_ban_room.o", "profile": "authored_cpp", "sha256": "ae475a92ef772c352fa58df8946c3711edb9cbd4d664ea8574626a5d409faabc", "source": "sources/w16-realm-a-gateway-notify-add-ban-room/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "notifyAddInviteAvatar__Q212RealmService15RealmGatewayAPIPCUsT1"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_notify_add_invite_avatar.o", "profile": "authored_cpp", "sha256": "9ffab6a3930423f8f4e1dce0104b445d9b7073a6f65ce0992ea79bdd3c82bf5c", "source": "sources/w16-realm-a-gateway-notify-add-invite-avatar/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "notifyAddInviteRoom__Q212RealmService15RealmGatewayAPIPCUsN21"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_notify_add_invite_room.o", "profile": "authored_cpp", "sha256": "3c39a6d53d5dab81734bf26da5bb9f2384f4614dbef4bc4155f3811f7376565b", "source": "sources/w16-realm-a-gateway-notify-add-invite-room/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "notifyEnterRoom__Q212RealmService15RealmGatewayAPIPCUsT1"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_notify_enter_room.o", "profile": "authored_cpp", "sha256": "131b31d8361e7a013286daf064902e802cac04ea2be8a769e21d2754e610e4c6", "source": "sources/w16-realm-a-gateway-notify-enter-room/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "notifyForcedLogout__Q212RealmService15RealmGatewayAPIs"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_notify_forced_logout.o", "profile": "authored_cpp", "sha256": "b7c6801abf9e934649270182e366168109b236ddd1a4e5264ae9cd39f6a3ba7c", "source": "sources/w16-realm-a-gateway-notify-forced-logout/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "notifyFriendStatus__Q212RealmService15RealmGatewayAPIPCUsb"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_notify_friend_status.o", "profile": "authored_cpp", "sha256": "105b32563c89c4527a9b081ab5db8412da7b178a0788faa7933801a3febfabdb", "source": "sources/w16-realm-a-gateway-notify-friend-status/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "notifyInstantMessage__Q212RealmService15RealmGatewayAPIPCUsT1"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_notify_instant_message.o", "profile": "authored_cpp", "sha256": "7a0fef57571b5fb987c897a68c755768653642cee04ce89aaf094cdb6fc2f170", "source": "sources/w16-realm-a-gateway-notify-instant-message/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "notifyKickAvatar__Q212RealmService15RealmGatewayAPIPCUsT1"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_notify_kick_avatar.o", "profile": "authored_cpp", "sha256": "5e97e7c8c314e8b1b525c5af7d587457477d382b9219aace87c9fcd26b1f13c4", "source": "sources/w16-realm-a-gateway-notify-kick-avatar/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "notifyKickRoom__Q212RealmService15RealmGatewayAPIPCUsN21"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_notify_kick_room.o", "profile": "authored_cpp", "sha256": "437a924e5ef9c6250b9c6e15601874f44547113ae40fea96b56c093cc4cc3697", "source": "sources/w16-realm-a-gateway-notify-kick-room/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "notifyLeaveRoom__Q212RealmService15RealmGatewayAPIPCUsT1"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_notify_leave_room.o", "profile": "authored_cpp", "sha256": "4f8d59839a3bfe2246c1313e2c8b033e8709f36f779b0b201fd6b0c0406c051d", "source": "sources/w16-realm-a-gateway-notify-leave-room/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "notifyModeratorAddedAvatar__Q212RealmService15RealmGatewayAPIPCUsT1"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_notify_moderator_added_avatar.o", "profile": "authored_cpp", "sha256": "c61fc6b8ac77d9d48c689c3cae1e01e6a88429264bbfdd0bd6d5604c0bc24da8", "source": "sources/w16-realm-a-gateway-notify-moderator-added-avatar/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "notifyModeratorAddedRoom__Q212RealmService15RealmGatewayAPIPCUsN21"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_notify_moderator_added_room.o", "profile": "authored_cpp", "sha256": "3cc12da71cc1069dd38a1714a52950a92141452bf812f1aed4dceee98aa0f0f4", "source": "sources/w16-realm-a-gateway-notify-moderator-added-room/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "notifyModeratorRemovedAvatar__Q212RealmService15RealmGatewayAPIPCUsT1"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_notify_moderator_removed_avatar.o", "profile": "authored_cpp", "sha256": "762e69e8fff263922ac22869a1f1a4318e05259ae000e09c56114d7da8084065", "source": "sources/w16-realm-a-gateway-notify-moderator-removed-avatar/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "notifyModeratorRemovedRoom__Q212RealmService15RealmGatewayAPIPCUsN21"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_notify_moderator_removed_room.o", "profile": "authored_cpp", "sha256": "e4723813028ade81278d2fcfd718b08bd274171847f1960c667d16faea838479", "source": "sources/w16-realm-a-gateway-notify-moderator-removed-room/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "notifyRemoveBanAvatar__Q212RealmService15RealmGatewayAPIPCUsT1"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_notify_remove_ban_avatar.o", "profile": "authored_cpp", "sha256": "ec426cd295053d5d4cbe1150f84a9f1daa99985e7bd850d728498ef7f9794a04", "source": "sources/w16-realm-a-gateway-notify-remove-ban-avatar/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "notifyRemoveBanRoom__Q212RealmService15RealmGatewayAPIPCUsN21"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_notify_remove_ban_room.o", "profile": "authored_cpp", "sha256": "f38f039af13e2658f548eb5c90e9f7f6a6e3b2ff0f7623d75e06f04ffae769d3", "source": "sources/w16-realm-a-gateway-notify-remove-ban-room/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "notifyRemoveInviteAvatar__Q212RealmService15RealmGatewayAPIPCUsT1"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_notify_remove_invite_avatar.o", "profile": "authored_cpp", "sha256": "85f469959c6f01574101fe63d3e842835a9de6b86de9b2952ab3042545268799", "source": "sources/w16-realm-a-gateway-notify-remove-invite-avatar/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "notifyRemoveInviteRoom__Q212RealmService15RealmGatewayAPIPCUsN21"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_notify_remove_invite_room.o", "profile": "authored_cpp", "sha256": "ef59433ecabe1bd72529d33a43487cab367bf95f5de956614ff2d0cee7802816", "source": "sources/w16-realm-a-gateway-notify-remove-invite-room/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "notifyRoomBanner__Q212RealmService15RealmGatewayAPIPCUsT1"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_notify_room_banner.o", "profile": "authored_cpp", "sha256": "d989ba7295a0e7af9123dc67f57dfdb323a9795a27906385fdada97c9bfbedcd", "source": "sources/w16-realm-a-gateway-notify-room-banner/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "notifyRoomMessage__Q212RealmService15RealmGatewayAPIPCUsN21"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_notify_room_message.o", "profile": "authored_cpp", "sha256": "431e8ec679dfedd896ac0eb36496950e2379839cf07dd6b5b1d34b3a07bb1388", "source": "sources/w16-realm-a-gateway-notify-room-message/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onAddBan__Q212RealmService15RealmGatewayAPIUiiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_on_add_ban.o", "profile": "authored_cpp", "sha256": "1339867ca16c33104bfceaef16e9090004b1249bb77575413dbf0cf0150fd33a", "source": "sources/w16-realm-a-gateway-on-add-ban/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onAddFriend__Q212RealmService15RealmGatewayAPIUiiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_on_add_friend.o", "profile": "authored_cpp", "sha256": "33b529cea80e0e57bce2649b09c32f34eb8bb9b34e6581b0a265b1a78ef44769", "source": "sources/w16-realm-a-gateway-on-add-friend/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onAddIgnore__Q212RealmService15RealmGatewayAPIUiiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_on_add_ignore.o", "profile": "authored_cpp", "sha256": "d25344a00766487b381cb755e0fb5516875e4f924e183db5c65eee2bc66ba070", "source": "sources/w16-realm-a-gateway-on-add-ignore/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onAddInvite__Q212RealmService15RealmGatewayAPIUiiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_on_add_invite.o", "profile": "authored_cpp", "sha256": "ad6d592f3ebfaca02c0c9ddc1bd78227795a35d6fd063fbd68952440f2f159a8", "source": "sources/w16-realm-a-gateway-on-add-invite/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onAddRoomModerator__Q212RealmService15RealmGatewayAPIUiiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_on_add_room_moderator.o", "profile": "authored_cpp", "sha256": "e74c0af0e946a56ab23ec15142b26bcd3a85867870f48a2364f29f9a369d8ec5", "source": "sources/w16-realm-a-gateway-on-add-room-moderator/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onChangeEmail__Q212RealmService15RealmGatewayAPIUiiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_on_change_email.o", "profile": "authored_cpp", "sha256": "feef8332eabb76ab915fb3e2dffcd9e07b5e78910272ef65d63aa4a305ba260c", "source": "sources/w16-realm-a-gateway-on-change-email/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onChangePassword__Q212RealmService15RealmGatewayAPIUiiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_on_change_password.o", "profile": "authored_cpp", "sha256": "528e8ba4fb1ecffd4405c806441d1831f1cf592fabc649dc321139fb2da0079f", "source": "sources/w16-realm-a-gateway-on-change-password/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onConsumeKey__Q212RealmService15RealmGatewayAPIUiiPCUsPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_gateway_on_consume_key.o", "profile": "authored_cpp", "sha256": "8a0ca2ee5a92f436f1d4e93740deda8495dfe5d6a965877ded10e8c0cb799438", "source": "sources/w16-realm-a-gateway-on-consume-key/stub.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "_ZN12_GLOBAL__N_1L7releaseEjj"}, {"compiled_body_bytes": 616, "name": "createPrivateGame__16VIRealmInterfaceiPCUsN22Pv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_vi_realm_interface_create_private_game.o", "profile": "authored_cpp", "sha256": "1a824366f981d7d2700f99a207067dcf6e3c89afa7fbc1ec44e64f370fa6153a", "source": "sources/w16-realm-a-vi-realm-interface-create-private-game/create_private_game.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "_ZN12_GLOBAL__N_1L7releaseEjj"}, {"compiled_body_bytes": 664, "name": "createPublicGame__16VIRealmInterfaceiPCUssUiUiUiT2RCQ212RealmService14MatchGameAttrsT2Pv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_vi_realm_interface_create_public_game.o", "profile": "authored_cpp", "sha256": "8c4fe7c3c26d74ca3644420a314b19795b169fb6d41f4d30bbe5590ff81aa62b", "source": "sources/w16-realm-a-vi-realm-interface-create-public-game/create_public_game.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 184, "name": "notifyClientDiscovered__16VIRealmInterfacePCci"}], "header_closure": ["sources/w9-animation-object-c-common/mem.hpp"], "kind": "independently_authored_source", "object_name": "w16ra_vi_realm_interface_notify_client_discovered.o", "profile": "authored_cpp", "sha256": "ecf6409653f94b3e071b530268d3abe0a817a424841bf1a155f13b61275f012b", "source": "sources/w16-realm-a-vi-realm-interface-notify-client-discovered/notify.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 296, "name": "notifyClientRequestsConnect__16VIRealmInterfacePCci"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_vi_realm_interface_notify_client_requests_connect.o", "profile": "authored_cpp", "sha256": "c7c27c8a345c8d7426b1a40c74b692e26beca71e47e8127fb11ce0bad1bcf2f4", "source": "sources/w16-realm-a-vi-realm-interface-notify-client-requests-connect/notify.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 144, "name": "notifyGameDiscovered__16VIRealmInterfacePCci"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_vi_realm_interface_notify_game_discovered.o", "profile": "authored_cpp", "sha256": "f0fc7ef5acceedfc0b1b1ea21aa8460dca1d033afe2948d39d5c5b21caa86998", "source": "sources/w16-realm-a-vi-realm-interface-notify-game-discovered/notify.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 308, "name": "onCancelGame__16VIRealmInterfaceUiiPv"}], "header_closure": ["sources/w9-animation-object-c-common/mem.hpp"], "kind": "independently_authored_source", "object_name": "w16ra_vi_realm_interface_on_cancel_game.o", "profile": "authored_cpp", "sha256": "252c6cd4939bd8d2eb6411427ada7d00a86f15bc4fa38c95929dbd26e1950885", "source": "sources/w16-realm-a-vi-realm-interface-on-cancel-game/on_cancel_game.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "_ZN12_GLOBAL__N_1L7releaseEjj"}, {"compiled_body_bytes": 608, "name": "_ZN12_GLOBAL__N_1L24wide_string_from_literalEPKc"}, {"compiled_body_bytes": 136, "name": "onConnect__16VIRealmInterface"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16ra_vi_realm_interface_on_connect.o", "profile": "authored_cpp", "sha256": "51621c950d7240bc38030f6a8cdab15790d30d6354786be084f18233080aa0c9", "source": "sources/w16-realm-a-vi-realm-interface-on-connect/on_connect.cpp"}
```

## Authored but never executed

Nothing in this package has run on an EE, in the game or against a real realm server. The interpreter runs used Python models of the
callees, not the original string-library or network code. The strcmp loop, the format-string `snprintf` calls and the narrowing
(low bytes only) are reproduced as observed, not as intended.

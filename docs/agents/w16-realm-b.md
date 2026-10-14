# w16-realm-b: online realm service interface (part 2)

Branch `agents/w16-realm-b`. Private reference executable `work/private/SLUS_205.65` (SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`). Addresses and sizes are that image's own symbol
values. Nothing from the image (bytes, disassembly, decompiler output) is committed, and nothing here ran on an EE or in the game.

## What was done

36 units are authored under `experimental/sources/w16-realm-b-*`, one verified function per commit: the 36 reply callbacks and the
per-frame pump of the realm interface that the front-end online windows call. Twenty-four are 8-byte empty callbacks (23 are the
default `RealmGatewayAPI` replies, one is `VIRealmInterface::onUpdateGameData`); the other twelve carry behaviour:

- Small replies: `onDisconnect__16VIRealmInterface`, `onTouchSession__16VIRealmInterfaceUiiPv`, `onGetRealmStats__...`.
- Dialog/page replies: `onDoClientDiscovery`, `onCreateAccount`, `onCreatePublicGame`, `onCreatePrivateGame`.
- Replies that use the image's inlined reference-counted string library: `onGetRules`, `onGetGame`, `onLogin`, `onMatchGame`.
- `process__16VIRealmInterface`: the per-frame lobby pump (session keep-alive, UDP discovery/ping/start-game exchange, datagram dispatch).

## Functions

| Symbol | Address | Image bytes | Compiled bytes | Unit |
|---|---|---|---|---|
| `process__16VIRealmInterface` | `0x01104ae8` | 7652 | 7664 | `w16-realm-b-process` |
| `onLogin__16VIRealmInterfaceUiiPCUsPv` | `0x011068d0` | 2124 | 512 | `w16-realm-b-on-login` |
| `onCreateAccount__16VIRealmInterfaceUiiPCUsPv` | `0x01107120` | 548 | 540 | `w16-realm-b-on-create-account` |
| `onCreatePublicGame__16VIRealmInterfaceUiiPCciPv` | `0x01107348` | 564 | 604 | `w16-realm-b-on-create-public-game` |
| `onCreatePrivateGame__16VIRealmInterfaceUiiPCciPv` | `0x01107580` | 524 | 564 | `w16-realm-b-on-create-private-game` |
| `onMatchGame__16VIRealmInterfaceiiUiPCPCUsN24PCiPCQ212RealmService4BlobPv` | `0x01107790` | 3444 | 1976 | `w16-realm-b-on-match-game` |
| `onGetGame__16VIRealmInterfaceiiPCUsT3iPv` | `0x01108508` | 1140 | 1192 | `w16-realm-b-on-get-game` |
| `onDoClientDiscovery__16VIRealmInterfaceUiiPCciPv` | `0x01108af8` | 356 | 384 | `w16-realm-b-on-do-client-discovery` |
| `onGetRules__16VIRealmInterfaceUiiPCUsUiPv` | `0x01108c60` | 928 | 952 | `w16-realm-b-on-get-rules` |
| `onDisconnect__16VIRealmInterface` | `0x01109258` | 40 | 20 | `w16-realm-b-on-disconnect` |
| `onTouchSession__16VIRealmInterfaceUiiPv` | `0x011092e0` | 52 | 60 | `w16-realm-b-on-touch-session` |
| `onGetRealmStats__16VIRealmInterfaceUiiUiUiUiUiUiUiUiUiPv` | `0x01109318` | 124 | 140 | `w16-realm-b-on-get-realm-stats` |
| `onUpdateGameData__16VIRealmInterfaceUiiPv` | `0x01109398` | 8 | 8 | `w16-realm-b-on-update-game-data` |
| `onLogout__Q212RealmService15RealmGatewayAPIUiiPv` | `0x01214a00` | 8 | 8 | `w16-realm-b-on-logout` |
| `onCreateAvatar__Q212RealmService15RealmGatewayAPIUiiPCPCUsUiT3UiPv` | `0x01214a10` | 8 | 8 | `w16-realm-b-on-create-avatar` |
| `onDestroyAvatar__Q212RealmService15RealmGatewayAPIUiiPv` | `0x01214a18` | 8 | 8 | `w16-realm-b-on-destroy-avatar` |
| `onGetPublicRooms__Q212RealmService15RealmGatewayAPIUiiPCPCUsT3PCUiT5UiPv` | `0x01214a20` | 8 | 8 | `w16-realm-b-on-get-public-rooms` |
| `onGetRoom__Q212RealmService15RealmGatewayAPIUiiPCUsT3PCPCUsUiT5UiPv` | `0x01214a28` | 8 | 8 | `w16-realm-b-on-get-room` |
| `onCreatePrivateRoom__Q212RealmService15RealmGatewayAPIUiiPCUsT3PCPCUsUiT5UiPv` | `0x01214a30` | 8 | 8 | `w16-realm-b-on-create-private-room` |
| `onEnterRoom__Q212RealmService15RealmGatewayAPIUiiPCUsT3PCPCUsUiT5UiPv` | `0x01214a38` | 8 | 8 | `w16-realm-b-on-enter-room` |
| `onLeaveRoom__Q212RealmService15RealmGatewayAPIUiiPv` | `0x01214a40` | 8 | 8 | `w16-realm-b-on-leave-room` |
| `onSendRoomMessage__Q212RealmService15RealmGatewayAPIUiiPv` | `0x01214a48` | 8 | 8 | `w16-realm-b-on-send-room-message` |
| `onSendInstantMessage__Q212RealmService15RealmGatewayAPIUiiPv` | `0x01214a50` | 8 | 8 | `w16-realm-b-on-send-instant-message` |
| `onRemoveFriend__Q212RealmService15RealmGatewayAPIUiiPv` | `0x01214a60` | 8 | 8 | `w16-realm-b-on-remove-friend` |
| `onRemoveIgnore__Q212RealmService15RealmGatewayAPIUiiPv` | `0x01214a70` | 8 | 8 | `w16-realm-b-on-remove-ignore` |
| `onRemoveRoomModerator__Q212RealmService15RealmGatewayAPIUiiPv` | `0x01214a80` | 8 | 8 | `w16-realm-b-on-remove-room-moderator` |
| `onRemoveBan__Q212RealmService15RealmGatewayAPIUiiPv` | `0x01214a90` | 8 | 8 | `w16-realm-b-on-remove-ban` |
| `onRemoveInvite__Q212RealmService15RealmGatewayAPIUiiPv` | `0x01214aa0` | 8 | 8 | `w16-realm-b-on-remove-invite` |
| `onKickAvatar__Q212RealmService15RealmGatewayAPIUiiPv` | `0x01214aa8` | 8 | 8 | `w16-realm-b-on-kick-avatar` |
| `onModifyRoomBanner__Q212RealmService15RealmGatewayAPIUiiPv` | `0x01214ab0` | 8 | 8 | `w16-realm-b-on-modify-room-banner` |
| `onGetUserInfo__Q212RealmService15RealmGatewayAPIUiibT3PCUsPv` | `0x01214ab8` | 8 | 8 | `w16-realm-b-on-get-user-info` |
| `onGetUserStats__Q212RealmService15RealmGatewayAPIUiiRCQ212RealmService9UserStatsPCPCQ212RealmService9UserStatsUiPv` | `0x01214ac0` | 8 | 8 | `w16-realm-b-on-get-user-stats` |
| `onGetRealmTime__Q212RealmService15RealmGatewayAPIUiiUiPv` | `0x01214ac8` | 8 | 8 | `w16-realm-b-on-get-realm-time` |
| `onStartGame__Q212RealmService15RealmGatewayAPIUiiUiPv` | `0x01214b00` | 8 | 8 | `w16-realm-b-on-start-game` |
| `onRecordGameOutcome__Q212RealmService15RealmGatewayAPIUiiPv` | `0x01214b08` | 8 | 8 | `w16-realm-b-on-record-game-outcome` |
| `onRoundTrip__Q212RealmService15RealmGatewayAPIUiiPv` | `0x01214b30` | 8 | 8 | `w16-realm-b-on-round-trip` |

The compiled size differs from the image size because the compiler repacks, tail-calls and schedules differently; no byte match is claimed.
Four static helpers are emitted as local functions next to their callers (not original symbols): `wide_login_text` (onLogin), and `find_combo`,
`wide_copy`, `narrow_copy` (onMatchGame). The 24 empty callbacks are weak (`W`) symbols in the image; the authored definitions are normal globals so that
any table taking their address can link.

## Method and checks

- Each function was read privately, rewritten in my own words and compiled alone with the reviewed `authored_cpp` flags
  (`work/toolchains/ee`). Offsets, constants and call order are in the header comment of each source file, under the headings
  observed / inferred / unknown.
- Every unit has a `host_check.cpp` (not a build input) built with the host compiler. They use an address arena
  (`NORRATH_HOST_ARENA`) for 32-bit pointer words and stub every callee with a recorder; the expected traces are written out by hand from
  the notes at the top of each source file, not generated from the code under test. The twenty-four empty callbacks are checked to leave all
  argument memory untouched. A check was kept only when a hand mutation of the source made it fail.
- Every function was also run against the original in a private R5900 interpreter (outside git): identical memory snapshots, identical Python
  stand-ins for every callee, equal call logs (arguments, string contents) and equal changed bytes. Runs were checked for `HALT`. Scenario
  counts, covered original words and the hand mutations that fail are in each commit message. Coverage gaps are padding words after branches
  and, in `process`, divide-by-zero guard traps.
- The harness passes only eight register arguments unless `Env.stack_words` is set; `onMatchGame` was first run without its two stack arguments (a
  vacuous run, spotted from the coverage figure) and redone. `process` compares and stores 64-bit clock values with `long long` arithmetic as the image does.

## Observed, inferred, unknown (summary)

**Observed.** The offsets, constants, string literals, call order and branch conditions in each source header. The image's reply functions take
`(this, a, status, ...)`; most print a trace line first (`VIRealmInterface::onX(%i, %i)`); `onCreatePrivateGame` prints the public routine's text.
Several replies set string-table messages (ids 1371 to 1871) on the dialog (`GetString__13VIStringTableii(mUI, id, mUI+48)`).
The string-heavy replies use the image's own inlined reference-counted `basic_string` code (shared empty rep, clone when the rep's flag word is
nonzero, unshare through `alloc` when the count is at least 2, release to a size-indexed pool or `__wrap_free`).

**Inferred.** That these are the callbacks of a realm gateway whose reply carries `(request id, result, ...)`; that status 0 is success in the
replies that branch on it; that `this+16` is a 256-unit account name with its length at `+272`; that the word at `this+12` holds the renderer and
has page (`+52`) and dialog (`+56`) pointers; that the word at `+3468` of `process` is a connection phase; that the three 20-byte regions of the
object (`+3376`, `+0x8b08`, `+0x947c`) and the 1144-byte row array starting at `+5088` are addresses and client rows. All of these are only
supported by how the code uses them.

**Unknown.** The meaning of every status value and packet type number; the names of the phase values; the layouts of the packets beyond the
offsets used; the class of `mUI` and of the application object returned by `GetAppPtr__5VIWnd`; what the large unused parts of some frames
hold; the roles of the `Blob` records in `onMatchGame` beyond their pointer and size words; whether the placeholder login credentials in
`onLogin` ("foo"/"bar") are a test path.

## Imports closed (36)

All thirty-six assigned names are now defined by the units listed above:

`onCreateAccount__16VIRealmInterfaceUiiPCUsPv`, `onCreateAvatar__Q212RealmService15RealmGatewayAPIUiiPCPCUsUiT3UiPv`, `onCreatePrivateGame__16VIRealmInterfaceUiiPCciPv`, `onCreatePrivateRoom__Q212RealmService15RealmGatewayAPIUiiPCUsT3PCPCUsUiT5UiPv`, `onCreatePublicGame__16VIRealmInterfaceUiiPCciPv`, `onDestroyAvatar__Q212RealmService15RealmGatewayAPIUiiPv`, `onDisconnect__16VIRealmInterface`, `onDoClientDiscovery__16VIRealmInterfaceUiiPCciPv`, `onEnterRoom__Q212RealmService15RealmGatewayAPIUiiPCUsT3PCPCUsUiT5UiPv`, `onGetGame__16VIRealmInterfaceiiPCUsT3iPv`, `onGetPublicRooms__Q212RealmService15RealmGatewayAPIUiiPCPCUsT3PCUiT5UiPv`, `onGetRealmStats__16VIRealmInterfaceUiiUiUiUiUiUiUiUiUiPv`, `onGetRealmTime__Q212RealmService15RealmGatewayAPIUiiUiPv`, `onGetRoom__Q212RealmService15RealmGatewayAPIUiiPCUsT3PCPCUsUiT5UiPv`, `onGetRules__16VIRealmInterfaceUiiPCUsUiPv`, `onGetUserInfo__Q212RealmService15RealmGatewayAPIUiibT3PCUsPv`, `onGetUserStats__Q212RealmService15RealmGatewayAPIUiiRCQ212RealmService9UserStatsPCPCQ212RealmService9UserStatsUiPv`, `onKickAvatar__Q212RealmService15RealmGatewayAPIUiiPv`, `onLeaveRoom__Q212RealmService15RealmGatewayAPIUiiPv`, `onLogin__16VIRealmInterfaceUiiPCUsPv`, `onLogout__Q212RealmService15RealmGatewayAPIUiiPv`, `onMatchGame__16VIRealmInterfaceiiUiPCPCUsN24PCiPCQ212RealmService4BlobPv`, `onModifyRoomBanner__Q212RealmService15RealmGatewayAPIUiiPv`, `onRecordGameOutcome__Q212RealmService15RealmGatewayAPIUiiPv`, `onRemoveBan__Q212RealmService15RealmGatewayAPIUiiPv`, `onRemoveFriend__Q212RealmService15RealmGatewayAPIUiiPv`, `onRemoveIgnore__Q212RealmService15RealmGatewayAPIUiiPv`, `onRemoveInvite__Q212RealmService15RealmGatewayAPIUiiPv`, `onRemoveRoomModerator__Q212RealmService15RealmGatewayAPIUiiPv`, `onRoundTrip__Q212RealmService15RealmGatewayAPIUiiPv`, `onSendInstantMessage__Q212RealmService15RealmGatewayAPIUiiPv`, `onSendRoomMessage__Q212RealmService15RealmGatewayAPIUiiPv`, `onStartGame__Q212RealmService15RealmGatewayAPIUiiUiPv`, `onTouchSession__16VIRealmInterfaceUiiPv`, `onUpdateGameData__16VIRealmInterfaceUiiPv`, `process__16VIRealmInterface`.

## Imports opened (57)

Computed by symbol diff, not by a link (see Build). Imports of my units that no unit in the last completed integrator base build referenced (they would appear as new unresolved names unless another package defines them):

`AddOption__13VIGUIComboBoxPc`, `AddPage__20VIWndGenericRendererPCc`, `Clear__13VIGUIComboBox`, `Finish__20VIWndGenericRenderer`, `ForceDisconnect__20VIWndGenericRenderer`, `GetAppPtr__5VIWnd`, `GetInetAddress__11VIUDPSocketP13VIInetAddressPCci`, `GetLocalAddress__11VIUDPSocketP13VIInetAddress`, `GetName__9VIGUIPage`, `GetObjectByName__9VIGUIPagePCc`, `GetStringAddress__11VIUDPSocketP13VIInetAddressPc`, `GoBack__20VIWndGenericRenderer`, `Hide__11VIGUIDialog`, `IsConnected__11VIUDPSocket`, `Open__11VIUDPSocket`, `ReceiveEula__9VIStationiPCUsi`, `RecvFrom__11VIUDPSocketP13VIInetAddressPiPciT2i`, `Reset__16VIRealmInterface`, `SendTo__11VIUDPSocketG13VIInetAddressiPCciPii`, `SendTo__11VIUDPSocketG13VIInetAddressiT1PCciT4iPii`, `SendTo__11VIUDPSocketG13VIInetAddressiT1T1iPCciPii`, `SetStringValue__9VIGUIPagePCcPCUsb`, `SetStringValue__9VIGUIPagePCcT1b`, `SetText__11VIGUIDialogPCUsb`, `SetText__11VIGUIDialogib`, `SetValue__13VIGUIComboBoxi`, `Show__11VIGUIDialogPCUsPCc`, `UnLock__11VIGUIDialog`, `UpdateNumPlayers__16VIRealmInterfacei`, `_$_Q212RealmService14MatchGameAttrs`, `__Q212RealmService14MatchGameAttrs`, `__tf11VIGUIObject`, `__tf13VIGUIComboBox`, `_t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0$nilRep`, `alloc__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0Uib`, `alloc__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uib`, `cancelGame__Q212RealmService15RealmGatewayAPIPCUsPv`, `clone__Q2t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0_3Rep`, `connected`, `doClientDiscovery__Q212RealmService15RealmGatewayAPIPCUsiT1Pv`, `frame.3503`, `g_lastSessionTouch`, `g_localPort`, `getRealmStats__Q212RealmService15RealmGatewayAPIPv`, `login__Q212RealmService15RealmGatewayAPIPCUsT1Pv`, `matchGame__Q212RealmService15RealmGatewayAPIPCUssUiUiiRCQ212RealmService14MatchGameAttrsbUiUiPv`, `packDiscoveryPacket__Q212RealmService15RealmGatewayAPIPUcPCUs`, `process__Q212RealmService15RealmGatewayAPI`, `replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiPCUsUi`, `replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiUiUs`, `replace__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0UiUiPCcUi`, `resize__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUs`, `resize__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uic`, `snprintf`, `strlenWide__FPCUs`, `strncpyWide__FPUsPCUsUi`, `touchSession__Q212RealmService15RealmGatewayAPIPCUsPv`.

Already unresolved in that base build and still imports here: `__dynamic_cast_2`, `clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep`.

Defined by units already in that base build (resolved at link, nothing opened): `GetString__13VIStringTableii`, `GetTime__7VITimer`, `_5VIWnd$mUI`, `__wrap_free`, `__wrap_strdup`, `_t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0$nilRep`, `_t24__default_alloc_template2b0i0$_S_free_list`, `atoi`, `g_ips`, `g_localClientIps`, `g_localClientPorts`, `g_localIp`, `g_mode`, `g_myIndex`, `g_numClients`, `g_ports`, `memcmp`, `memset`, `printf`, `snprintf16__8VIStringPUsiPCUse`, `snprintf16__8VIStringPUsiPCce`, `sprintf`, `strcmp`, `strcpy`, `strlen`, `strstr`.

Data symbols among the opened names: `connected` (word, 0x011b017c), `g_lastSessionTouch` (8 bytes, 0x011b0180), `g_localPort` (word, 0x011b20e0) and the file-local `frame.3503` (word, 0x011b0188); `_5VIWnd$mUI` is defined by an existing unit. `_t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0$nilRep` (the wide empty-string rep, 16 bytes) is the only opened string-library data name. Nothing here defines storage; this package only declares it.


## Build-plan entries needed

Profile `authored_cpp`; the integrator assigns `input_order`, adds the sources to the snapshot and adds the header `sources/w9-animation-object-c-common/mem.hpp` hash if it
is not yet listed. Do not add `host_check.cpp`. The entries are produced by `docs/agents/w16-realm-b-plan-entries.py --toolchain <ee> [--private-root DIR]`;
the shared `build-plan.json` and `snapshot.json` were not edited.

```json
{"emitted_functions": [{"compiled_body_bytes": 540, "name": "onCreateAccount__16VIRealmInterfaceUiiPCUsPv"}], "header_closure": ["sources/w9-animation-object-c-common/mem.hpp"], "kind": "independently_authored_source", "object_name": "w16rb_on_create_account.o", "profile": "authored_cpp", "sha256": "cbee86d80087c7bbf476f13711aed9ba14907eb8b27bbba9666a73a5702f9f55", "source": "sources/w16-realm-b-on-create-account/on_create_account.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onCreateAvatar__Q212RealmService15RealmGatewayAPIUiiPCPCUsUiT3UiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_create_avatar.o", "profile": "authored_cpp", "sha256": "29befcaa9047ddd8a2e112a26e6d033c992fd901696f67f4fb394773ebd410ea", "source": "sources/w16-realm-b-on-create-avatar/on_create_avatar.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 564, "name": "onCreatePrivateGame__16VIRealmInterfaceUiiPCciPv"}], "header_closure": ["sources/w9-animation-object-c-common/mem.hpp"], "kind": "independently_authored_source", "object_name": "w16rb_on_create_private_game.o", "profile": "authored_cpp", "sha256": "203a593c76c58e0b050783b1dccc2442a1ba10dcff24ffc692f4a3ab4e1a08b8", "source": "sources/w16-realm-b-on-create-private-game/on_create_private_game.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onCreatePrivateRoom__Q212RealmService15RealmGatewayAPIUiiPCUsT3PCPCUsUiT5UiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_create_private_room.o", "profile": "authored_cpp", "sha256": "7f68f3ad4131332fbb261975d3b9ec4d2179e60029392bdde6db3963cd17f4e2", "source": "sources/w16-realm-b-on-create-private-room/on_create_private_room.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 604, "name": "onCreatePublicGame__16VIRealmInterfaceUiiPCciPv"}], "header_closure": ["sources/w9-animation-object-c-common/mem.hpp"], "kind": "independently_authored_source", "object_name": "w16rb_on_create_public_game.o", "profile": "authored_cpp", "sha256": "a51d9e472da8df0477c88e2b120f53a154f4d5152590c2a5642876e81fa39c07", "source": "sources/w16-realm-b-on-create-public-game/on_create_public_game.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onDestroyAvatar__Q212RealmService15RealmGatewayAPIUiiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_destroy_avatar.o", "profile": "authored_cpp", "sha256": "f148e3cdab6740ce4c0f3d5063b48b6ffb38944b93bc2827d607c617085f68f9", "source": "sources/w16-realm-b-on-destroy-avatar/on_destroy_avatar.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 20, "name": "onDisconnect__16VIRealmInterface"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_disconnect.o", "profile": "authored_cpp", "sha256": "6e7eb443237902df6ae92b86d694fd75ef58f95f96d9741b9488c4bd61806bcf", "source": "sources/w16-realm-b-on-disconnect/on_disconnect.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 384, "name": "onDoClientDiscovery__16VIRealmInterfaceUiiPCciPv"}], "header_closure": ["sources/w9-animation-object-c-common/mem.hpp"], "kind": "independently_authored_source", "object_name": "w16rb_on_do_client_discovery.o", "profile": "authored_cpp", "sha256": "5c26d08f6e0f8a4e77c956187388150dc7316e895e8e74a6bbfd0bfd444080b4", "source": "sources/w16-realm-b-on-do-client-discovery/on_do_client_discovery.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onEnterRoom__Q212RealmService15RealmGatewayAPIUiiPCUsT3PCPCUsUiT5UiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_enter_room.o", "profile": "authored_cpp", "sha256": "17bfe9f6874a54d4b4f8f377ce8656cd9360e44f923875de533b1f98a05869a0", "source": "sources/w16-realm-b-on-enter-room/on_enter_room.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1192, "name": "onGetGame__16VIRealmInterfaceiiPCUsT3iPv"}], "header_closure": ["sources/w9-animation-object-c-common/mem.hpp"], "kind": "independently_authored_source", "object_name": "w16rb_on_get_game.o", "profile": "authored_cpp", "sha256": "6ef641eec364dacdc4792c66226268e63be5d2f784b3bb6dda23300b65e6afd9", "source": "sources/w16-realm-b-on-get-game/on_get_game.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onGetPublicRooms__Q212RealmService15RealmGatewayAPIUiiPCPCUsT3PCUiT5UiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_get_public_rooms.o", "profile": "authored_cpp", "sha256": "3edc298184c9b4c15d682db1da55633dba651cd42c85e601c6a8263953994389", "source": "sources/w16-realm-b-on-get-public-rooms/on_get_public_rooms.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 140, "name": "onGetRealmStats__16VIRealmInterfaceUiiUiUiUiUiUiUiUiUiPv"}], "header_closure": ["sources/w9-animation-object-c-common/mem.hpp"], "kind": "independently_authored_source", "object_name": "w16rb_on_get_realm_stats.o", "profile": "authored_cpp", "sha256": "27309a4f59bd776336459addcb26443fe4298ac7bbabd2675871b9ad6f3d53e3", "source": "sources/w16-realm-b-on-get-realm-stats/on_get_realm_stats.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onGetRealmTime__Q212RealmService15RealmGatewayAPIUiiUiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_get_realm_time.o", "profile": "authored_cpp", "sha256": "1661ed1200389004cefa082e74178ba677253c8019fd093292e68aa7753fd265", "source": "sources/w16-realm-b-on-get-realm-time/on_get_realm_time.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onGetRoom__Q212RealmService15RealmGatewayAPIUiiPCUsT3PCPCUsUiT5UiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_get_room.o", "profile": "authored_cpp", "sha256": "5be814b2642ec64ba3b3d66a2b7244bae41b59e22494443feb2063ac7d5d6bf8", "source": "sources/w16-realm-b-on-get-room/on_get_room.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 952, "name": "onGetRules__16VIRealmInterfaceUiiPCUsUiPv"}], "header_closure": ["sources/w9-animation-object-c-common/mem.hpp"], "kind": "independently_authored_source", "object_name": "w16rb_on_get_rules.o", "profile": "authored_cpp", "sha256": "75720cea74764dce012c38ea620e2cad1e4dc967bf2802d8eef2c3c329012f92", "source": "sources/w16-realm-b-on-get-rules/on_get_rules.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onGetUserInfo__Q212RealmService15RealmGatewayAPIUiibT3PCUsPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_get_user_info.o", "profile": "authored_cpp", "sha256": "f802e8925973466af177b2b66b49a9c0ea6bf939357bb54dd2b1adec8e80eeb6", "source": "sources/w16-realm-b-on-get-user-info/on_get_user_info.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onGetUserStats__Q212RealmService15RealmGatewayAPIUiiRCQ212RealmService9UserStatsPCPCQ212RealmService9UserStatsUiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_get_user_stats.o", "profile": "authored_cpp", "sha256": "2ac5b3d8266b4e36a7cc2ff39e5dd549b1262eaf4d096ff82df7de7fb0a05823", "source": "sources/w16-realm-b-on-get-user-stats/on_get_user_stats.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onKickAvatar__Q212RealmService15RealmGatewayAPIUiiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_kick_avatar.o", "profile": "authored_cpp", "sha256": "45aca7a74891923017d475d2d262bd870f56498e67e5b3b90f073df04f913753", "source": "sources/w16-realm-b-on-kick-avatar/on_kick_avatar.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onLeaveRoom__Q212RealmService15RealmGatewayAPIUiiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_leave_room.o", "profile": "authored_cpp", "sha256": "0f26e7cf10de6387ad0380f5959377e9e9f567cd0622c8487a04838f239282d4", "source": "sources/w16-realm-b-on-leave-room/on_leave_room.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 984, "name": "wide_login_text"}, {"compiled_body_bytes": 512, "name": "onLogin__16VIRealmInterfaceUiiPCUsPv"}], "header_closure": ["sources/w9-animation-object-c-common/mem.hpp"], "kind": "independently_authored_source", "object_name": "w16rb_on_login.o", "profile": "authored_cpp", "sha256": "5ac454d3377788530312375ff3f8cb00178d4f8f36481b61ed1b70e93a10bf9b", "source": "sources/w16-realm-b-on-login/on_login.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onLogout__Q212RealmService15RealmGatewayAPIUiiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_logout.o", "profile": "authored_cpp", "sha256": "1ad3cf8cf4bf35af86b93c325d39710bc24732d4e1fdff993bb29d25b9e02170", "source": "sources/w16-realm-b-on-logout/on_logout.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "find_combo"}, {"compiled_body_bytes": 192, "name": "wide_copy"}, {"compiled_body_bytes": 400, "name": "narrow_copy"}, {"compiled_body_bytes": 1976, "name": "onMatchGame__16VIRealmInterfaceiiUiPCPCUsN24PCiPCQ212RealmService4BlobPv"}], "header_closure": ["sources/w9-animation-object-c-common/mem.hpp"], "kind": "independently_authored_source", "object_name": "w16rb_on_match_game.o", "profile": "authored_cpp", "sha256": "66f210bea5465fa6ae554f02cfdedd91bc94d179e47a37cd45b9bd2dbb0d63ae", "source": "sources/w16-realm-b-on-match-game/on_match_game.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onModifyRoomBanner__Q212RealmService15RealmGatewayAPIUiiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_modify_room_banner.o", "profile": "authored_cpp", "sha256": "3a106f370d6762421770ff6ae4391796b7048a5f10ff3c090a908be2062db2aa", "source": "sources/w16-realm-b-on-modify-room-banner/on_modify_room_banner.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onRecordGameOutcome__Q212RealmService15RealmGatewayAPIUiiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_record_game_outcome.o", "profile": "authored_cpp", "sha256": "a381e3fc61f8e9fe3b153090795b844d976a9a7329f7f21c606a5b316030c08b", "source": "sources/w16-realm-b-on-record-game-outcome/on_record_game_outcome.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onRemoveBan__Q212RealmService15RealmGatewayAPIUiiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_remove_ban.o", "profile": "authored_cpp", "sha256": "f3ac0c7b7e7a64c781ed7820a9c85d94dbb852e96a26a4e975a54d8253a6f884", "source": "sources/w16-realm-b-on-remove-ban/on_remove_ban.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onRemoveFriend__Q212RealmService15RealmGatewayAPIUiiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_remove_friend.o", "profile": "authored_cpp", "sha256": "f9998acbd1d0c3eb0d00ff7243134e89d358a4bfa76fb34f957baf68b6457053", "source": "sources/w16-realm-b-on-remove-friend/on_remove_friend.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onRemoveIgnore__Q212RealmService15RealmGatewayAPIUiiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_remove_ignore.o", "profile": "authored_cpp", "sha256": "e0734b9edd56c0b499af76927de8c3cd54c7d5fa9f2f5b6bba8f6de41a5d91b3", "source": "sources/w16-realm-b-on-remove-ignore/on_remove_ignore.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onRemoveInvite__Q212RealmService15RealmGatewayAPIUiiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_remove_invite.o", "profile": "authored_cpp", "sha256": "f0c963f00d006fdde02868699208b9a161294150ab007abb7afd6fc917a8c1a9", "source": "sources/w16-realm-b-on-remove-invite/on_remove_invite.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onRemoveRoomModerator__Q212RealmService15RealmGatewayAPIUiiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_remove_room_moderator.o", "profile": "authored_cpp", "sha256": "462b94d90077258df3359de393f245c945e1c00e76bdc7dcdc26d08678c89a44", "source": "sources/w16-realm-b-on-remove-room-moderator/on_remove_room_moderator.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onRoundTrip__Q212RealmService15RealmGatewayAPIUiiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_round_trip.o", "profile": "authored_cpp", "sha256": "ba7c03ddfe6bd92c5f916d4c9f78097bb120db7e26769cb4ad2dc3a24f08b1e8", "source": "sources/w16-realm-b-on-round-trip/on_round_trip.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onSendInstantMessage__Q212RealmService15RealmGatewayAPIUiiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_send_instant_message.o", "profile": "authored_cpp", "sha256": "4e4ebdfb7247e07711bdc9f381a9347111409e2305c2091405dd06f95df553e8", "source": "sources/w16-realm-b-on-send-instant-message/on_send_instant_message.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onSendRoomMessage__Q212RealmService15RealmGatewayAPIUiiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_send_room_message.o", "profile": "authored_cpp", "sha256": "e242aa1d03840427de810d2c87fe1c3c9c13674c9b2df33b1c0a5c01579a983d", "source": "sources/w16-realm-b-on-send-room-message/on_send_room_message.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onStartGame__Q212RealmService15RealmGatewayAPIUiiUiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_start_game.o", "profile": "authored_cpp", "sha256": "5c905d8db61d81c32e42ec5d2400e1c3a38238523c9ff90ae5d6c79744ce044b", "source": "sources/w16-realm-b-on-start-game/on_start_game.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 60, "name": "onTouchSession__16VIRealmInterfaceUiiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_touch_session.o", "profile": "authored_cpp", "sha256": "7a9b543a16b48f59f4ca205083e603727f2d385a7437f2c5be32450c4b31fd9a", "source": "sources/w16-realm-b-on-touch-session/on_touch_session.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "onUpdateGameData__16VIRealmInterfaceUiiPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16rb_on_update_game_data.o", "profile": "authored_cpp", "sha256": "9400c840c9df2d61bd4edf53405f653eca3765def340a7867bb7024c3a009567", "source": "sources/w16-realm-b-on-update-game-data/on_update_game_data.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 7664, "name": "process__16VIRealmInterface"}], "header_closure": ["sources/w9-animation-object-c-common/mem.hpp"], "kind": "independently_authored_source", "object_name": "w16rb_process.o", "profile": "authored_cpp", "sha256": "e175771e2ba43114228162cea5f00c0b6458e3ae9b01c78b227643ef668c842e", "source": "sources/w16-realm-b-process/process.cpp"}
```

## Build

A full private partial build (shared plan + my 36 entries, 7,363 selected inputs) was started and stopped after 891 compilations in about 25 minutes because the machine was saturated by other agents; no link was produced, so no link-level numbers are claimed. Instead: (1) every unit compiles alone with the reviewed `authored_cpp` flags to the sizes in the table (the plan-entries script above does exactly this); (2) the symbol sets of my objects were diffed against the last completed integrator base build (`work/int-base-1791963852`: 841 unresolved imports, 10,400 defined symbols). All 36 assigned names were in its unresolved list and none was defined there, so no duplicate definitions arise; closing them and opening the names listed above would move that count from 841 to about 862 before other packages' work. `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.

## Authored but never executed

Everything. No unit has run on the EE or inside the game; the paired runs use an R5900 interpreter with scripted callees, which does not model
interrupts, the TLB, sockets or hardware timing, and the host checks run on the build machine's CPU. Specific limits:

- The inlined string-library code was exercised only against Python/host models of `replace`, `resize`, `alloc`, `clone` and `__wrap_free`;
  the real library routines were not run, and the shared empty-string reps were given synthetic counts and flags.
- `process` was run with scripted socket, clock, GUI and packet callees; real datagrams, real `VIInetAddress` contents and the real GUI classes were not
  involved. The `UpdateNumPlayers__16VIRealmInterfacei` stand-in stores the count into `this+3476`; whether the real routine does so is not known.
- By-value `VIInetAddress` and player-record arguments are passed as pointers to explicit copies; the image's ABI for these was read from the
  generated code, not from any header.
- Hand mutation survivors that are equivalent code: widening the `state - 2 < 3u` range in `process` (state 5 is handled by an earlier branch); the
  sign extension of the narrow literal bytes in `onLogin` (ASCII only); order-insensitive loads in the string helpers.

## Unknown

- What status 0, 1, 36 to 40 and the other reply codes stand for, beyond the branches they select; the text behind the string ids (1371,
  1384, 1839 to 1871).
- Who owns `connected`, `g_lastSessionTouch`, `g_mode`, `g_myIndex`, `g_numClients`, `g_ips`, `g_ports`, `g_localClientIps`, `g_localClientPorts`,
  `g_localIp`, `g_localPort` and the file-local `frame.3503`; they are imports here.
- The layout and size of the object behind `this` beyond the offsets used (it extends past `+0x947c + 20`), of the 1144-byte client rows, and of
  the packet records (types 7 to 14 and the 236-, 256-, 108-byte records).
- Why `onCreatePrivateGame` prints the public routine's name, and why `onLogin` has a placeholder login.
- The 614,400-count passed to `snprintf16` in `onGetRules` and the 4095-count in the create-game replies (the buffers are 614,400 and 4,096
  bytes in the image, so the counts look like byte counts).

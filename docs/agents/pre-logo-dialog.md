# pre-logo-dialog

Takeover uses the existing `agents/pre-logo-dialog` worktree. It was clean at
`00dcc88` except one uncommitted unit (`pre-logo-dialog-run`, committed as
`9072971` after review). The predecessor had published fourteen package
commits but no handoff note. All nineteen units freshly compiled with the
plan's reviewed EE `authored_cpp` flags; all nineteen host contracts passed
with ASan/UBSan. `python3 -m unittest discover -s tools -p 'test_*.py'`
passed, 214 tests. The private executable SHA-256 was checked against
`targets/SLUS-20565/baseline.json`.

Observed evidence is from that pinned image; addresses below are provenance,
not absolute-address bindings in the authored link. Inferred C++ interfaces
cover accessed fields only. Host contracts test call arguments and state
changes; they are not original-machine execution or exact-byte comparisons.

## Takeover additions

`runDialog__Fv` (inherited uncommitted), `dialogMsgCallback__FPUci`,
`drawDialog__Fv`, and `pauseMenuFrameRun__Fv` are reconstructed, each in its
own commit with a host contract. Evidence notes:

- `dialogMsgCallback__FPUci` at `0x001d1c80`, 2292 original bytes: the
  length is never read. The signed first byte selects five cases through the
  table at `0x0046cdc0`. Case 0 stores a dialog offer after `gameNetKick(0)`
  unless `recallOn` is set: name bytes with an alignment quirk (no terminator
  is stored when the cursor is already aligned; the host contract covers both
  sides back-to-back), packet word, player/check bytes, camera floats, and
  yaw/pitch halves drive `Push`/`SetYaw`/`SetPitch`/`BuildMatrix` on
  `gameCam` in that order. Packet records have stride 1096 with the flag at
  byte 1; the first nonzero `scriptStart` index is kept for `startPacket`. A
  valid player byte reads the entity word at that player record plus 21104;
  otherwise the single-object search runs. Up to 100 listed objects are
  scanned for the nearest by float distance with the first entry forced.
  Bit 0x20 of the entity word at offset 12 selects the creature block; the
  slot at `(entity+72)+4` is called (second argument: pitch-extended leftover
  on the single path, end of list on the multi path, list base when the list
  is empty and the original leaves call-clobbered bytes in the first
  argument), and its result goes through the NPC type check into
  `setExtraParams`. Idle animation comes from "esc" with an "idle" fallback.
  Sizes round to 2048 for two buffers tagged with the eight unnamed zero
  bytes at `0x00485040`; no ELF symbol covers them, so the unit keeps its own
  zero block. Case 1 advances text, shares one scroll tail for the
  showOptions-clear and tween-away paths (`scrollSpan` is always 0 there
  because the tail requires `scrollY == scrollEnd`), resets stream 2, and
  compares option names at `curDlg+84*opt+136` against the tag; a mismatch
  starts `tag+84*opt+136` (the original adds to the post-`strcmp` first
  argument, which is unadvanced exactly on the path that reaches the call).
  A record word at +168 other than -1 resets the body and starts that
  packet. Case 2 moves the option by the signed second byte; offline play
  only restarts the tween. Case 4 stores -1 in `swooping` and falls into
  case 3, which waits on `animLoadingComplete`, frees both buffers, restores
  the camera, clears the dialog words, restores a low creature flag through
  `animAdd`, wakes monsters, sets far clip 1800, adopts `DLG_NewWorld` into
  `levelName` with 0xff at byte 80 on mismatch, runs the post action, shops
  the first local player when flagged, and switches scenes when told.
  `curDlg`/`dlgArrayPtr` are `unsigned long`, which is 32 bits under the
  reviewed EE `mlong32` profile and carries full pointers in host contracts.
  The flags load was moved after the null check so the host contract stays
  within sanitizer rules; observable behavior matches on every path where
  the original does not fault.
- `drawDialog__Fv` at `0x001d2f28`, 1448 original bytes: returns while
  `curDlg` is zero or a swoop runs. The fade word rises once through
  `flaredIn`; the alpha byte is the fade (or one minus it while flared out)
  scaled by 128 and tints five text colors. The body prints with the
  restrict count gated by `letterAtATime`. Options print only while
  `textPageDone` is set and the show-text/stream-idle/cancel gate passes; an
  empty list prints "Affirmation", otherwise each option prints its string
  at `curDlg+84*opt+88` with its line position, highlighting the selection.
  A nonzero word at `curDlg+84` Hermite-interpolates the cursor row, else row
  398. The "%c" glyph, the "dialogMore" prompt, the revealed speaker name
  (decremented by the 19.6-scaled count, clamped at zero), and the "(%s)"
  remote-player line follow. Plain C++ casts reproduce the observed
  `cvt.w.s` conversions under the reviewed compiler. `loadSampleRate.784`
  is a static owned elsewhere, referenced by asm label like the package's
  `lastNmQ.1088` precedent.
- `pauseMenuFrameRun__Fv` at `0x0017e738`, 4292 original bytes: the opening
  dead scan for an empty table word is omitted (pure reads, result
  discarded). The `packetBuffer+0xF7C0` table swaps exit-menu names by
  network mode and gates the kick-players entry by local/connected counts.
  Then come the coordinates/config gates, the async-mode return, the exit
  countdown (message while nonzero, multiplayer exit plus game-loop bail at
  zero), the exit-to-menu yes/no (1 clears and returns, non-2 returns, 2
  arms the count and exits the player HUD), and the state-51 save flow (fail
  and back-button pop the config, accept resets to the first table and saves
  again, neither returns). The menu walk enables load/controls entries only
  when networked and checks hurt, dialog, jumpgate, camera, and entity state
  for recall; change-players always ends the walk through the draw. The draw
  shows the menu plus "menulong.tex", the footer buttons, and "helpbar.tex",
  then copies player names into the pre-configuration menu when current.
  Music reseeds from entry snapshots, the hot timer freezes at 241, and the
  back button gates the state machine. Five menu states dispatch through the
  table at `0x00463dc0` (return/save/load/level/kick/recall/shop/cleanup/
  exit/players/options/race/more, warp names, controls/comm, controller
  init, kicks), all rejoining at a tail that restores the menu position
  unless the menu moved tables. Unnamed bytes are literals ("%c", "(%s)",
  "hud.lmp", "fx.lmp", "lava3", ".tex", menu/text keys); the fixed tables
  are same-object offsets into `packetBuffer`, and `usprintf` lands in the
  named `hudFooterUni` view. The walk flag is at +4 on EE; host pointers are
  eight bytes wide there, so host builds use the otherwise unobserved +8
  slot. `menu` is `unsigned char*` exactly as game-boot declares it.

## Working environment

`git fetch origin` failed on shared malformed ref
`refs/heads/agents/iop-module-boot 2`. That unrelated shared ref was not changed.
Private references, build plans, compiled objects, and logs stay outside git.

## Imports closed

These package symbols are defined here (pending selection in the shared
build plan; see the private trial below):

`DLG_FireUpShopOnExit`, `DLG_ForceMutexRelease__Fv`,
`DLG_StartDialog__FPcP9DlgPacketii`, `DLG_StartWithSwoopDone`,
`DLG_SwoopMode`, `animSetSpecialSoundHandler__FPFiG6Point3_v`,
`dialogMsgCallback__FPUci`, `dialogMsgHandle`, `drawDialog__Fv`,
`gameFootstepSound__FG6Point3iii`, `gameobjInit__Fv`,
`helpMessageFrame__Fv`, `hudAddHelpMessage__FiPUsT1`,
`hudMsgCallback__FPUci`, `hudMsgHandle`, `hudSetInfoMessage__FPUsiic`,
`hudShowBust__FPcPUsii`, `hudShowCompass__FG6Point3PUs`,
`lightHaloTexture`, `netSendMessage__FiPUci`, `pauseMenuFrameRun__Fv`,
`runDialog__Fv`.

`player` and `c.1089` are not defined here (see Unknown). `specialHandler`
storage is not defined here either; only the setter is.

## Imports opened

No new names beyond what the referenced owners already leave open: every
other symbol these units call or read stays an unresolved import owned
elsewhere. Notable cross-package references: Camera methods (`Push`,
`SetYaw`, `SetPitch`, `BuildMatrix`, `SetMode`, `StartDialogSwoop`,
`AdvanceDialogSwoop`, `Update`, `GetYaw`, `GetPitch`), creature methods
(`sleepMonsters`, `wakeMonsters`, `setExtraParams`, `playerDamageHandler`),
`MC_GameConfig`, dialog state words (`scrollY`, `optTbl`, `dlgName`,
`textBody`, `dlgArrayPtr`, `optionLinePos`, `anim`, `dialogCreature`,
`dialogIdleAnimation`, `waitForSound`, `animLoadingComplete`, `dialogMutex`,
`sceneTransitionOK.1041`, `loadSampleRate.784`, `lastNmQ.1088`), menu state
(`menu`, `menuNm`, `menuPlayerNm`, `kickMenu`, `kickMenuList`,
`preConfigure`, `lastWarpPos`, `hudFooterUni`, `endTimer.1059`,
`packetBuffer`, `netLocalPad`, `player`, `levelName`, `gameCam`), sound,
font, lump, shop, script, and net functions, and the C++ runtime names
(`__tf3NPC`, `__eq__C9type_infoRC9type_info`, `__throw_bad_typeid`).
`strcmp`/`strcpy`/`sprintf` resolve to the libc-format package;
`anim` is also claimed at `0x00790f60` by a same-named local symbol, so the
two `anim` objects need an integrator decision like the menu/Malloc second
objects.

## Build-plan entries needed

Append these authored inputs and assign fresh `input_order` values.
`header_closure` is empty. `profile` is `authored_cpp`. `kind` is
`independently_authored_source`. Do not add any `host_check.cpp` file. Body
sizes are the reviewed EE g++ sizes, not the original extents. Data units
emit no functions. The hud-msg unit also emits its file-local helper.

```json
{"source":"sources/pre-logo-dialog-footstep/footstep.cpp","object_name":"footstep.o","sha256":"e89c24850ecf3ef3673515028b65a57b2d34b3785bf933e91d9fa9c5a0eb12ef","emitted_functions":[{"name":"gameFootstepSound__FG6Point3iii","compiled_body_bytes":460}]}
{"source":"sources/pre-logo-dialog-force-mutex/force_mutex.cpp","object_name":"force_mutex.o","sha256":"01dcc2bab704d25e9c743aa2de695bd2f3b9a2778c11b41b286c6b1a2fff63a0","emitted_functions":[{"name":"DLG_ForceMutexRelease__Fv","compiled_body_bytes":12}]}
{"source":"sources/pre-logo-dialog-gameobj-init/gameobj_init.cpp","object_name":"gameobj_init.o","sha256":"8f3ef9e97f8f0667e990a136e4a95d0dc8933603523b960b730735dde3c8b71f","emitted_functions":[{"name":"gameobjInit__Fv","compiled_body_bytes":40}]}
{"source":"sources/pre-logo-dialog-halo/light_halo.cpp","object_name":"light_halo.o","sha256":"27260c4795362ebdca7cceb3a46b563a099cbcc035ce8f31882247858b9ed51a"}
{"source":"sources/pre-logo-dialog-handles/msg_handles.cpp","object_name":"msg_handles.o","sha256":"7c7288512f484e7d8b758b1c78f777516b49e2f700d43f1d3f324ed69e666136"}
{"source":"sources/pre-logo-dialog-help-frame/help_frame.cpp","object_name":"help_frame.o","sha256":"19e8af5cd16e66ee92d2d4a9f13a4673d09262049d5ae5ac830a69303ed5954a","emitted_functions":[{"name":"helpMessageFrame__Fv","compiled_body_bytes":1708}]}
{"source":"sources/pre-logo-dialog-hud-add-help/hud_add_help.cpp","object_name":"hud_add_help.o","sha256":"cd51f3a9ac7e190125b526b69f68a03849f16fde8062fb878d45bdfd10f8ad39","emitted_functions":[{"name":"hudAddHelpMessage__FiPUsT1","compiled_body_bytes":360}]}
{"source":"sources/pre-logo-dialog-hud-msg/hud_msg.cpp","object_name":"hud_msg.o","sha256":"a8ba9a3274c7f474d921499a18dce591f1aba2836b55658c25c1313bef5d384a","emitted_functions":[{"name":"_ZL17disconnect_playeri","compiled_body_bytes":112},{"name":"hudMsgCallback__FPUci","compiled_body_bytes":964}]}
{"source":"sources/pre-logo-dialog-hud-set-info/hud_set_info.cpp","object_name":"hud_set_info.o","sha256":"c95e164089ae47e9f7cc141961ec911d456990778d8af1196123cfa2bf7c60c2","emitted_functions":[{"name":"hudSetInfoMessage__FPUsiic","compiled_body_bytes":168}]}
{"source":"sources/pre-logo-dialog-msg/dialog_msg.cpp","object_name":"dialog_msg.o","sha256":"1b76de24cbb694400023c55c3fbc99bf78285315e12f8f334a70775fcf1bfa00","emitted_functions":[{"name":"dialogMsgCallback__FPUci","compiled_body_bytes":3192}]}
{"source":"sources/pre-logo-dialog-net-send/net_send.cpp","object_name":"net_send.o","sha256":"9e881d1cf96b08bd6fdeb7046e068e4008e340e5fcdbc13de8334cf0308105ac","emitted_functions":[{"name":"netSendMessage__FiPUci","compiled_body_bytes":344}]}
{"source":"sources/pre-logo-dialog-pause/pause_menu.cpp","object_name":"pause_menu.o","sha256":"555267983223f766bf7ce7894ded1183ee2689af6d6b3c07697ed7c05685e1e8","emitted_functions":[{"name":"pauseMenuFrameRun__Fv","compiled_body_bytes":4888}]}
{"source":"sources/pre-logo-dialog-run/run_dialog.cpp","object_name":"run_dialog.o","sha256":"21db4ef831f53e092676d49c9694775583800e53efeaf07c7c3598f8664c6c44","emitted_functions":[{"name":"runDialog__Fv","compiled_body_bytes":2100}]}
{"source":"sources/pre-logo-dialog-show-bust/show_bust.cpp","object_name":"show_bust.o","sha256":"74d65e21626dd7071bc639e4ad5c76830050920d8a0be708bc9fcfef05b76661","emitted_functions":[{"name":"hudShowBust__FPcPUsii","compiled_body_bytes":212}]}
{"source":"sources/pre-logo-dialog-show-compass/show_compass.cpp","object_name":"show_compass.o","sha256":"f7149285e45a40b3a55fdc149f1d1ec912d5acb3dbaf32a66123ed21097f9d1b","emitted_functions":[{"name":"hudShowCompass__FG6Point3PUs","compiled_body_bytes":248}]}
{"source":"sources/pre-logo-dialog-special-handler/special_handler.cpp","object_name":"special_handler.o","sha256":"1ce0d928db3baa80ee2e7293de961bde47594e3a43786acc0e6965e8c455d420","emitted_functions":[{"name":"animSetSpecialSoundHandler__FPFiG6Point3_v","compiled_body_bytes":12}]}
{"source":"sources/pre-logo-dialog-start-dialog/start_dialog.cpp","object_name":"start_dialog.o","sha256":"57e27d1e1366687fa9e3581564905c8667678dc4e893b907e9636804b4216af1","emitted_functions":[{"name":"DLG_StartDialog__FPcP9DlgPacketii","compiled_body_bytes":448}]}
{"source":"sources/pre-logo-dialog-words/dialog_words.cpp","object_name":"dialog_words.o","sha256":"f925ff0cef307dd3d9d8493439c3ace476e8b7fb5c2c63d7faaa0dd4a062aac7"}
{"source":"sources/pre-logo-dialog-draw/draw_dialog.cpp","object_name":"draw_dialog.o","sha256":"04f9c5314cdb42e66ad8f957357586201cc00342daa2aede4a5f963be2d48688","emitted_functions":[{"name":"drawDialog__Fv","compiled_body_bytes":1632}]}
```

## Private trial

A private copy of the plan outside git appended all nineteen package units
(orders 847-865) and ran the reviewed driver against the reviewed EE
toolchain: 864 source compilations with implicit assembly and one
relocatable link, no retries, EXIT=0. Result: 740 function bodies (723 on
the shared base + 17 new here: the sixteen package functions plus the
hud-msg file-local helper), 1014 unresolved imports, 0 COMMON. A later
two-instruction bound fix in the pause unit (`p < 4` to `p <= 4` in both
connected-player loops, plus host coverage) changed its body from 4884 to
4888 bytes with byte-identical defined/undefined symbol sets, so the
accounting below is unaffected. All 22
defined package names left the unresolved list (verified against the trial
`unresolved-imports.json`); `player` and `c.1089` remain open by design
(see Unknown). A matching baseline trial on the same tree (845 inputs, no
package units) linked 723 bodies and 879 unresolved, exactly the
shared-branch count. Diff: 22 closed, 157 opened, all of them symbols these
units reference that no selected unit defines yet:

`AdvanceDialogSwoop__6CameraiPf`, `BuildMatrix__6CamerafffP6Point3`,
`DLG_NewWorld`, `GetPitch__6Camera`, `GetYaw__6Camera`,
`MC_ChangePlayers__FiPUsiii`, `Push__6Camera`, `SetMode__6Camerai`,
`SetPitch__6Camerai`, `SetYaw__6Camerai`,
`StartDialogSwoop__6CameraP10GameObjecti`, `Update__6Camera`, `__tf3NPC`,
`__throw_bad_typeid`, `anim`, `animLoadingComplete`, `animStarted`,
`body`, `cancelStreamWait`, `checkBlock`, `cleanUp__Fv`,
`colorLevelNames__FP4Menu`, `colorScale__FUif`, `controlPaused`,
`coreInterfaceBrightness`, `coreInterfaceFadeUpVal`,
`coreInterfaceFadeUp__Fi`, `coreStartSwitchScene__Fi`, `correction`,
`createByName__FPcG6Point3sPPc`, `cursorTween`,
`debugClearStartMenu__Fv`, `debugRecall__Fi`, `dialogCreature`,
`dialogIdleAnimation`, `dialogMutex`, `dlgArrayPtr`, `dlgName`, `dlgPos`,
`drawBustCur`, `drawBustMax`, `drawBustOn`, `drawBustTexName`,
`drawBustTitle`, `drawCompassOn`, `drawCompassPoint`, `drawCompassTitle`,
`endTimer.1059`, `evalHermite__Ffffff`, `flaredIn`, `flaredOut`,
`fontColor__FUiii`, `fontEnd__Fv`, `fontLastPrintedPosition`,
`fontPrintCentered__FiiPci`, `fontPrintCenteredu__FiiPUsi`,
`fontPrintWrapu__FiiPUsiiii`, `fontPrint__FiiPci`, `fontPrintu__FiiPUsi`,
`fontRestrict__Fi`, `fontScissor__Fiiii`, `fontStart__FP4FontiUii`,
`fontStringSizeu__FP4FontPUsi`, `frontendInitControllerConfig__Fi`,
`frontendRunControllerConfig__Fv`, `gameCheckNoSaveZone__Fv`,
`gamePopConfig__Fv`, `gamePushConfig__Fv`, `helpMessageOn`,
`hudAssignQuickSpell__Fiii`, `hudAssignSpellBookSpell__Fiiil`,
`hudFooterUni`, `hudNetLevelWarp__FPc`, `hudRunYesNo__FPUsiiii`,
`hudShowMsg__FPUsN20iii`, `infoMessage`, `infoMessageAlways`,
`infoMessageTimer`, `initAnimation__Fv`, `interfaceOn`, `kickMenu`,
`kickMenuList`, `lastNmQ.1088`, `lastWarpPos`, `letterAtATime`,
`lineToY__Fi`, `loadSampleRate.784`,
`menuDraw__FiiP4MenuUiUiP4FontP8_texturei`, `menuRun__FP4MenuP3padPi`,
`msgHandler`, `msgHudPlayerExit__Fv`, `msgHudRecall__Fc`, `multiBody`,
`multiHelpMessageOn`, `multiRelease`, `multiSide`, `multiTitle`,
`nameChars`, `netBlankControllerButtons__Fiii`,
`netDisconnectPlayer__Fi`, `netKickPlayer__Fi`, `netLocalPad`,
`netPlayerIsConnected__Fi`, `netRestoreButtonConfigs__Fv`,
`neverSaveAlwaysLoadFromLump__FPc`, `nextIndex`,
`objectFindAnimation__FP10GameObjectPc`,
`objectSetCreateByNameFunction__FPFPcG6Point3sPPc_P10GameObject`,
`objectSetFilterFunction__FPFPc_i`, `optTbl`, `optionCount`,
`optionIndex`, `optionLinePos`, `optionsShowing`,
`playerDamageHandler__6Player`, `preConfigure`, `realDlgPos`,
`rectangleDraw__FiiiiiUi`, `resetTextBody__Fv`, `runAnimation__Fv`,
`saveActivate__9Savepoint`, `sceneTransitionOK.1041`,
`scriptActivatePostAction__Fv`, `scrollEnd`, `scrollSpan`,
`scrollTarget`, `scrollY`, `sentEndMsg`,
`setExtraParams__FP8CreaturePc`, `showCoords__Fv`, `showMore`,
`showOptions`, `sleepMonsters__8creaturev`, `soundContinueStream__Fi`,
`soundGetDialogSize__FPc`, `soundGetNmQueued__Fi`,
`soundGetStreamAmountPlayed__Fi`, `soundGetStreamPreloadDone__Fi`,
`soundPlaySequence__FP15AnimationHeaderG6Point3f`,
`soundSetMusicDuckAmount__Ff`, `soundStartGlobalMusic__FPci`,
`soundUpdateAdjustments__Fv`, `specialHandler`, `startPacket__Fi`,
`swooping`, `textBody`, `textBodyChars`, `textPageDone`,
`textureDrawBorder__FiiiiiUiPcii`, `title`,
`usprintf__FPUsT0ffffffff`, `ustrcat__FPUsT0`, `ustrcpy__FPUsPc`,
`ustrcpy__FPUsT0`, `ustrlen__FPUs`, `waitForSound`,
`wakeMonsters__8creaturev`.

(`__eq__C9type_infoRC9type_info` and several others these units also
reference were already unresolved on the base, so they are not new.)

## Authored but never executed

All package EE implementations. Host contracts ran; no EE execution, startup,
rendered dialog, pause menu, or gameplay has been demonstrated.

## Unknown

- `player`: items-player already defines the 95,936 zero bytes at
  `0x00408600` as `norrath_player_progression_records_view`. This package
  only imports the ELF name. Whether the view should gain that alias is an
  integrator decision.
- `c.1089` at `0x007d0988` has ELF size 0; `worldBoundingBox` begins 384
  bytes later and the 100-byte `__6Camera` constructor writes halfwords
  through offset 136. That does not establish a size, so this package does
  not define it. The Camera-owning package should settle the extent.
- `preConfigure` exists twice (80 bytes at `0x0037f150`, used here, and at
  `0x004289d0`); `anim` exists twice (`0x007d0dc8`, used here, and
  `0x00790f60`). Each pair needs an integrator decision like the earlier
  menu/Malloc second objects.
- The pause-menu `hudShowMsg`/`hudRunYesNo` extra register arguments and the
  dialog-callback empty-list first argument are call-clobbered leftovers in
  the original; only the consumed values are modeled.
- The `menu==preConfigure` name-copy block and the kick setup write through
  menu tables whose full record layouts are not established beyond the
  accessed words.

# natives-c

Script natives 81 through 120, in plain byte order of the `amx*__FP5__amxPi` names, plus the camera and dust imports opened by `norrath_custom_variables_process`. Each symbol is its own translation unit under `experimental/sources/natives-c-*`. Game callees stay unresolved. This package does not define `_start`, `_InitSys`, `main`, `launchSRS`, or the `worldWater*` words.

Each unit was compiled with the reviewed `authored_cpp` flags. Each host test failed when the behavior was wrong and passed after the reconstruction. Host tests include the host C library and are not EE plan inputs. No EE object from this package was linked into `experimental/build-plan.json` or executed. Compiled sizes below are `nm -S` body sizes from that compile. They are not original-byte matches.

`python3 -m unittest discover -s tools -p 'test_*.py'` passed, 214 tests. `tools/check_source_snapshot.py` is not part of that run. It will reject this tree until the integrator adds every new file to `experimental/snapshot.json`. This branch does not edit the snapshot, the shared plan, or `tools/`.

## Observed

Retail anchors below are local reference only. The authored comments in each `.cpp` record the cell layout and the callee. String natives copy through `amx_GetAddr` and `amx_GetString` into a 160-byte buffer unless a shorter original slot is named in that file. `amx_GetAddr`'s physical pointer is zeroed before the call. The original stack slot was left uninitialized when that call failed.

Camera float table stores start at byte 176. `SetMaxHeight` stores `f12` at byte 164. `SetPitchAtLevel` stores the truncated halfword at byte `108 + level * 2`, and also at byte 106 when `level` equals the int at byte 84. `DisableZooming` latches the level at byte 84, writes the old level at byte 88, and blends the distance words. `SetDistanceAtLevel` always stores the argument at byte `176 + level * 4`, and blends only when `level` matches the int at byte 84. `fxUpdateDust` forwards eight integers to `P_UpdateParticleType` with `fxDust` and the `.lit4` words `0x41a4cccc`, `0x41180000`, and `0xc0a00000`, then the words 2, 0, 2, and a null callback. `gameCam` at `0x00484f50` and `levelFlags` at `0x004844c0` are 4-byte zeros.

Player records are 23984 bytes. Gold is the word at byte 232. The header pointer used by the best-player and vehicle natives is at byte 21872. The simple-new-world halfword is at byte `23968 + activePlayer * 23984 + 2`. `nmPlayers` and `activePlayer` are the existing words. `scriptInPostAction` and `netNetworkMode` are bytes. `levelName` byte 80 receives `0xFF` on a zone kick, and `levelName` byte 81 is the lump stem. Dialog formatting uses the literals `%s.dbin` and `%s.lmp`. The prop tag is the literal `lump`. The local-player refusal uses the literal `noZone3`. The reap box is the `.lit4` words `0xc76a6000` and `0x476a6000` on all three axes. NPC and prop transitions use the `.lit4` word `0x3e999999`.

| Symbol | Retail address | Retail bytes | Compiled bytes |
|---|---:|---:|---:|
| `SetMaxHeight__6Cameraf` | `0x0026fb78` | 8 | 40 |
| `SetPitchAtLevel__6Cameraii` | `0x0026fb80` | 32 | 100 |
| `DisableZooming__6Camera` | `0x0026c100` | 140 | 644 |
| `SetDistanceAtLevel__6Cameraif` | `0x0026fba0` | 132 | 592 |
| `fxUpdateDust__Fiiiiiiii` | `0x001e6410` | 128 | 112 |
| `gameCam` | `0x00484f50` | 4 | 4 |
| `levelFlags` | `0x004844c0` | 4 | 4 |
| `amxPutPlayersInVehicle__FP5__amxPi` | `0x0013abc0` | 236 | 324 |
| `amxReapEnemies__FP5__amxPi` | `0x00139928` | 424 | 672 |
| `amxRemovePlayerItem__FP5__amxPi` | `0x00139ad0` | 248 | 276 |
| `amxRemoveQuest__FP5__amxPi` | `0x0013afc0` | 56 | 60 |
| `amxSaveCheckPoint__FP5__amxPi` | `0x0013a9d0` | 52 | 52 |
| `amxSaveDone__FP5__amxPi` | `0x0013c0d0` | 36 | 36 |
| `amxSaveEndConfig__FP5__amxPi` | `0x0013c058` | 116 | 284 |
| `amxSaveGame__FP5__amxPi` | `0x0013c030` | 40 | 40 |
| `amxSetBestActivePlayer__FP5__amxPi` | `0x0013c308` | 60 | 128 |
| `amxSetEntrance__FP5__amxPi` | `0x0013aa28` | 64 | 68 |
| `amxSetFarClip__FP5__amxPi` | `0x0013b6d0` | 36 | 40 |
| `amxSetHitPoints__FP5__amxPi` | `0x0013a970` | 96 | 140 |
| `amxSetInfo__FP5__amxPi` | `0x0013ae58` | 76 | 80 |
| `amxSetNoCollide__FP5__amxPi` | `0x0013c198` | 112 | 224 |
| `amxSetParticle__FP5__amxPi` | `0x0013c108` | 140 | 140 |
| `amxSetPostAction__FP5__amxPi` | `0x0013b578` | 56 | 60 |
| `amxSetSaveInhibit__FP5__amxPi` | `0x0013c4c8` | 16 | 20 |
| `amxSetupPlayerSel__FP5__amxPi` | `0x0013aa68` | 32 | 32 |
| `amxSetv__FP5__amxPi` | `0x0013b038` | 72 | 76 |
| `amxShakeCamera__FP5__amxPi` | `0x0013c208` | 40 | 44 |
| `amxShopRunning__FP5__amxPi` | `0x0013b6c8` | 8 | 12 |
| `amxShowBust__FP5__amxPi` | `0x0013a880` | 152 | 132 |
| `amxShowCompass__FP5__amxPi` | `0x0013a7d0` | 176 | 176 |
| `amxShowMonster__FP5__amxPi` | `0x00139698` | 148 | 172 |
| `amxSimpleNewWorld__FP5__amxPi` | `0x00139430` | 240 | 340 |
| `amxSoundSequence__FP5__amxPi` | `0x0013b180` | 156 | 144 |
| `amxSound__FP5__amxPi` | `0x0013b0d8` | 164 | 152 |
| `amxStartAudio__FP5__amxPi` | `0x0013bfd0` | 64 | 68 |
| `amxStartDialog__FP5__amxPi` | `0x00139020` | 168 | 196 |
| `amxStartMusic__FP5__amxPi` | `0x0013bf90` | 60 | 64 |
| `amxStartNPCAnim__FP5__amxPi` | `0x001390c8` | 360 | 344 |
| `amxStartPlayerSel__FP5__amxPi` | `0x0013aa88` | 32 | 32 |
| `amxStartPropAnim__FP5__amxPi` | `0x00139230` | 288 | 320 |
| `amxStartWithSwoopDone__FP5__amxPi` | `0x0013b330` | 16 | 20 |
| `amxStopFollowingPath__FP5__amxPi` | `0x00139730` | 172 | 280 |
| `amxStopFollowingPlayer__FP5__amxPi` | `0x0013b908` | 148 | 248 |
| `amxStopPropAnim__FP5__amxPi` | `0x0013b380` | 60 | 64 |
| `amxTakePlayerGold__FP5__amxPi` | `0x0013bb88` | 92 | 148 |
| `amxTrigger__FP5__amxPi` | `0x0013a6e8` | 176 | 252 |
| `amxUnFriendly__FP5__amxPi` | `0x00139880` | 164 | 332 |

## Inferred

Virtual slots are a signed halfword this-adjust and a function pointer four bytes later. `amxTrigger` uses the slot at vtable byte 24. `amxPutPlayersInVehicle` uses the slot at vtable byte 112. `amxReapEnemies` uses the slot at vtable byte 8 with second argument 3. The type-id call is the function pointer at vtable byte 4 and does not assign an argument; the original left that register untouched. `amxTrigger` also left the unadjusted object in the third argument register. These reconstructions pass the adjusted object and the explicit integer only.

Pointer loads use the machine pointer width. On the reviewed EE profile that is 4 bytes, the same width as the original `lw`. Host tests store real pointers, including addresses above 4GB. `gameCam` and `levelFlags` are explicit zeros. Under `-G0` the compile places them in BSS. The retail words are 4-byte zeros. The value is zero either way. `-fno-zero-initialized-in-bss` is not part of the profile. The host compile warns that those two symbols are initialized and declared `extern`.

Object lists are walked as pointer arrays. A flag-bit miss still advances to the next object. `amxPutPlayersInVehicle` reloads `nmPlayers` only on the path that calls the slot. `amxTakePlayerGold` divides cell 1 by the original count and stores 0 when the unsigned difference is negative. `amxSimpleNewWorld` kicks when `scriptInPostAction` is nonzero, `netNetworkMode` is zero, `activePlayer` is -1, or the halfword is zero. A zero halfword is replaced with 900 before the kick. A nonzero halfword for a local player shows `noZone3` for 180 ticks and does not kick.

`amxReapEnemies` passes the box as two point addresses, matching the original `a0`/`a1` setup for `objectFindInBox__FG6Point3T0PP10GameObjecti`. Bits `0x28` in the word at object byte 12 skip the type test. A match against `PhProp`, then `Prop`, then `ItemProp` also reaches the reap slot. Bit `0x40` skips that slot. The 12-byte point copies in `amxShowCompass` and `amxStopFollowingPath` are byte copies. The original compass and path copies used an unaligned 8-byte load plus a word.

## Snapshot files

Add these to `experimental/snapshot.json` `files`. Host paths are snapshot files only.

```
5ca10f5bcd833f778a11ebb65c87caae0364d9adfdd7b58686b94f95af569f26  sources/natives-c-set-max-height/set_max_height.cpp
f97c2ac7146a51778bd05c0c23a19216660c8492f6b182e08c35dca72c271b74  sources/natives-c-set-max-height/set_max_height_host.cpp
aa6d7a7f0f016535542d58539dbff9a5cfdc9e30dcd256e566aeaffe89dda16e  sources/natives-c-set-pitch/set_pitch.cpp
c06acb3883605341e3b035032d792f0bea7abf3bfc6a82939625e1989fee1d05  sources/natives-c-set-pitch/set_pitch_host.cpp
5e741f63659a3dbc773a4fda6bfd609978af9a8fb0996e5a3b5232bd0c6abae3  sources/natives-c-disable-zoom/disable_zoom.cpp
55acccf0d1896fc9bc77005d8577f9d97967a539097cd26e6727b06e191b9ad4  sources/natives-c-disable-zoom/disable_zoom_host.cpp
cdaddc8a5d910da92e1707c63784bf5c5ba1dc0dab551ee6a5939d9b9d124b45  sources/natives-c-set-distance/set_distance.cpp
8d2b392de07bf8148c5bddfd3173a3d0c525689b28c53913c318d7ada7f9878c  sources/natives-c-set-distance/set_distance_host.cpp
fce5ee74b5a362aabc68622bfc1bc0f2cf3394a1cc36aa79ee2b6b331b5c221a  sources/natives-c-update-dust/update_dust.cpp
f9c90b547f638c1f65ce2be88109861711c3f65c5ba1de0716e1c4b65da588b7  sources/natives-c-update-dust/update_dust_host.cpp
25821bceac06465ca26fe334de7d779601872a03ef5d88cbba14d265fc6ab4bf  sources/natives-c-game-cam/game_cam.cpp
c5dc66b8453e763694987aecfca896554ab2a9957998b51837168b9e46c34cb8  sources/natives-c-game-cam/game_cam_host.cpp
b990639819f4b751d0e5474b01ff376bae6b1e48f87d001afc2fa39757d657da  sources/natives-c-level-flags/level_flags.cpp
9937fe6f43d297caca6092edcb76cdf638035dcd98f4e20188dc1e6c46aa05c8  sources/natives-c-level-flags/level_flags_host.cpp
bb9a79e1bb913a46f0aad5530a266788571f8e9d9b65ba13e3df19dc5c3d3f9b  sources/natives-c-amx-put-players-in-vehicle/put_players_in_vehicle.cpp
2a0f341c2875546d0861256f8bbe4bce7152de0cc4a23e55d342a8a2c77481ef  sources/natives-c-amx-put-players-in-vehicle/put_players_in_vehicle_host.cpp
f2b9d76b67078e4978f0ebb233ceb419e7d1e3bbca7ab366de9b4a58c8c0573e  sources/natives-c-amx-reap-enemies/reap_enemies.cpp
d3617ba9112fbe2d379bc1b4012891d7f74140f7f78f43daf9d488cd720bf23d  sources/natives-c-amx-reap-enemies/reap_enemies_host.cpp
f144b580f7aee9bac2a39bf3b3f4650daa9bb678f5b99b71783feb207b55a541  sources/natives-c-amx-remove-player-item/remove_player_item.cpp
68eb6204c5bc56981ed0ed4ff4acbe6b6e9fb1ffbf2ee5ad2cc2501effe1c30e  sources/natives-c-amx-remove-player-item/remove_player_item_host.cpp
af2825c8149136f4850ac93706ebae621470c46b82d5fb672fb99b42e3846fe9  sources/natives-c-amx-remove-quest/remove_quest.cpp
be8a9b2cd0ff7daad401b91aa78ed553d5400fea6808e2122983799e610cfe82  sources/natives-c-amx-remove-quest/remove_quest_host.cpp
2038bd13991adec4007d16aa14d587eeaf6146253a455acf6916be82d7bc5c4e  sources/natives-c-amx-save-checkpoint/save_checkpoint.cpp
ee5afb736cc8ef5928cc4fc5b883f061035d725246a9a4cf9e45c4460d6eaab3  sources/natives-c-amx-save-checkpoint/save_checkpoint_host.cpp
92595c384f0143c85df75e932de1c114e531ec6a981d622c584b2c6cf39d43ca  sources/natives-c-amx-save-done/save_done.cpp
b2c9a625c565a3f1d696ecde92e2f5f7d2201426b2f47499f07b2fc391b73147  sources/natives-c-amx-save-done/save_done_host.cpp
bad25dd0616ccc91e061c89eb0693a31587e9842a3caf044bd6463ffdc292ce9  sources/natives-c-amx-save-end-config/save_end_config.cpp
ed31322a4c8a35ae2aaf3027b189e996244d4fef19b25b3a44d2ee96112ec7a8  sources/natives-c-amx-save-end-config/save_end_config_host.cpp
0f10a0901986822ae2c22309404ff7405caa423c0a8300bdc31c756953b4969f  sources/natives-c-amx-save-game/save_game.cpp
d42caa713a8c1752f73b7fa7b0063150bf3420c6ceb666a147f9e9f1d241a6d9  sources/natives-c-amx-save-game/save_game_host.cpp
cff08ef11c518d69957ff291e38629ed8fa13f99daf0094b202e6fef0878522d  sources/natives-c-amx-set-best-active-player/set_best_active_player.cpp
58914acbd96e4d78974a54d282e6c73c62802fd5854b023d931a690aefed3113  sources/natives-c-amx-set-best-active-player/set_best_active_player_host.cpp
f2ea2cfc758a03a0cbd69bb2eeb570ec1744c5e85a49273ad63e8fbea1f3444d  sources/natives-c-amx-set-entrance/set_entrance.cpp
7c458bc60b61abe04b25bbb1190de99bb540179495e9b0d09f0c58498793481c  sources/natives-c-amx-set-entrance/set_entrance_host.cpp
9d84b9e01cb06ce8f69fdfaef0ae84dd8d40142f2e9d115d9290f8b30dbd729f  sources/natives-c-amx-set-far-clip/set_far_clip.cpp
d972c8a01ae35c6e68e45e1446e34693eea74e55eec522e43ebb4f06f2eaf44d  sources/natives-c-amx-set-far-clip/set_far_clip_host.cpp
e3d6b59b90146e7e953d9ca5378c41d04a98916ca522245fbc4f000fa11fed12  sources/natives-c-amx-set-hit-points/set_hit_points.cpp
be9d9776b408778eb2d5e59252fd40abc1feb10b4ea3ca6a291665dc05c23ab8  sources/natives-c-amx-set-hit-points/set_hit_points_host.cpp
7bba098df52b1db532413a7fa3b5832aa7f78d4777f2895c9bc0bfc1ace7fd16  sources/natives-c-amx-set-info/set_info.cpp
acc070b124847e5b6aa79bf67dd9439a746bc1cf67ce3d38dd375d27e9123966  sources/natives-c-amx-set-info/set_info_host.cpp
fd8a2b72f9c600f7e9fff8472e131c4e0943a5a09013aae779c75362132e640c  sources/natives-c-amx-set-no-collide/set_no_collide.cpp
a867c5774f54f79c972b923d8ad135186fac3df354a827f7af0f5bbaea8624c1  sources/natives-c-amx-set-no-collide/set_no_collide_host.cpp
717906bd8d74c3eb45c223e6074489ad8ed4fc0b6bad0c56be708ac1d342d785  sources/natives-c-amx-set-particle/set_particle.cpp
0570d5efb03b9354cdd173e71a162daf5afefbb38cd977289525011ab8af94fb  sources/natives-c-amx-set-particle/set_particle_host.cpp
65098b1dde149bc4e22360a7c32f3417a936ccc0bca016015b1f1a4c531ccb24  sources/natives-c-amx-set-post-action/set_post_action.cpp
586b5944255a0bae7114d2a3af4ef31179402bb7021919f5250989f78e2e1e4c  sources/natives-c-amx-set-post-action/set_post_action_host.cpp
b0280e0ecaad031f72afcea475a0117539c42a84dcf3af689e1d118355b088db  sources/natives-c-amx-set-save-inhibit/set_save_inhibit.cpp
9460e5b31ffe2066c2bcd90ea067e66bb39f14ef44e325ea61950844a940f1c6  sources/natives-c-amx-set-save-inhibit/set_save_inhibit_host.cpp
d3ce63de5951b9be09fa9803fea5fea7b528165a15598274c5be7323e85d47f8  sources/natives-c-amx-setup-player-sel/setup_player_sel.cpp
4f481cc4755de8906f5b3113e0e6e1a790b1714cf4b2b3d79f00f4a7161f36e5  sources/natives-c-amx-setup-player-sel/setup_player_sel_host.cpp
f6a958fe33cf91f692810b9c79873481005702f9d98205746ba6724ca10aad6e  sources/natives-c-amx-setv/setv.cpp
01b8b8ab6a8468affd78ab4cb47cb73421580c63584a27596fdc2a97cd6b1fe9  sources/natives-c-amx-setv/setv_host.cpp
3dd5861a4236c33d6b83d71156a2fe2ed49ad098c3be2938d471b73b8ae3c048  sources/natives-c-amx-shake-camera/shake_camera.cpp
3d7bfb07112e174af9b3a1a580c1a03ab1f281add0db630b309bb69116382e66  sources/natives-c-amx-shake-camera/shake_camera_host.cpp
3068247b91a3ca0e1cdf720c97d935c9f78a763d975d77da3ab2acc0a352edfe  sources/natives-c-amx-shop-running/shop_running.cpp
d7e776eb306f169fb6d70dac70060e7032a1cf54deee5a9eec03a716b0e42e15  sources/natives-c-amx-shop-running/shop_running_host.cpp
6d4e6ba9603445ba3607785e0c7b29241eee86588e11b3879fa32a2e8ec65e6c  sources/natives-c-amx-show-bust/show_bust.cpp
468fe0e557ee3cd41d48844e1bee84cb464e6cbb04c41422c75f23b0e600e738  sources/natives-c-amx-show-bust/show_bust_host.cpp
93ff37f4fb827ae554d836cccfee72fd4799eec72f43a72716d648686abb1f58  sources/natives-c-amx-show-compass/show_compass.cpp
39853139d849386a96077205d9709796d9cb01439300f0c5835a4ad1c617beef  sources/natives-c-amx-show-compass/show_compass_host.cpp
aa9879dd44bc0dca4544b13384971053f144d60ad9fb4da7c151b82914b37a47  sources/natives-c-amx-show-monster/show_monster.cpp
655a3f24a2870d527e61e8b26c5c812928c9595a2ddaaab9b338b5059e19ba40  sources/natives-c-amx-show-monster/show_monster_host.cpp
e41b6e26b158486651ad58e304618de0ffaa8207302fb21e72aa1c116aa683cf  sources/natives-c-amx-simple-new-world/simple_new_world.cpp
7fff498951eed1fa8d5850d67ebfd97d74d1945a975807ae1e1ed5b5092e9ec1  sources/natives-c-amx-simple-new-world/simple_new_world_host.cpp
de5e43f8aecd92c4f34b4f27bcff90043e3c52e023a39c2ec63618ce0778cb28  sources/natives-c-amx-sound-sequence/sound_sequence.cpp
fafd351cd78272a5a1b2f75668d64f033cc6c9019a3043e9dc0ac5767e2556b9  sources/natives-c-amx-sound-sequence/sound_sequence_host.cpp
df1a5dd5cf3774170d7e2c16fc5c1139a2b8597e6ac9fba3d9320991fc8a07e0  sources/natives-c-amx-sound/sound.cpp
cb7aa95ade2da494baa71da07ecabcdea2d403d9e636cf55cd7ba345bfd01281  sources/natives-c-amx-sound/sound_host.cpp
a4d1e4c167757c5683d9dd8c01e8e9f7f412065590f970ed002036e19c409081  sources/natives-c-amx-start-audio/start_audio.cpp
46cb8dbb945eb9593724a8349d446e8f19d28ae3e23e2deab8354d36cbbd337b  sources/natives-c-amx-start-audio/start_audio_host.cpp
f23235f3cc098826b907494a653c43a1c8cea5834fec420281bf752997e4fd9f  sources/natives-c-amx-start-dialog/start_dialog.cpp
de257dcebf7d3801b679dbb41f9364c025d07022a573ae6f3eeff78623528a5a  sources/natives-c-amx-start-dialog/start_dialog_host.cpp
c99f8a6bbf9c79923eb80647d00eb7dbadb97c21136523d77cb2c15a8a87811c  sources/natives-c-amx-start-music/start_music.cpp
0c9f8879eed25f56fad2e29dc4e53985a11c54996a0282045ad145e3ab5933e8  sources/natives-c-amx-start-music/start_music_host.cpp
00cafac2cb9b20e1275a3f50dcd43b22925212b40b50ea80632fe2f920624a33  sources/natives-c-amx-start-npc-anim/start_npc_anim.cpp
964d322a248d36c30b667080592e310503337e68a2a34f6b9cbd9ef40cf25fb4  sources/natives-c-amx-start-npc-anim/start_npc_anim_host.cpp
09000b10e0af9be8f00baee788688d4617b0321ed2d0547555c98d7c5b3978da  sources/natives-c-amx-start-player-sel/start_player_sel.cpp
bb6fbf1b8e781ecfc7c35ace5eb4ff516cd8791338eb96d0403395da7193041c  sources/natives-c-amx-start-player-sel/start_player_sel_host.cpp
657591b4541b07cbf31c93cde5642c4a25253eccf78d1a0cfe0786d7fb7402be  sources/natives-c-amx-start-prop-anim/start_prop_anim.cpp
22f6723cf07278bd6892a4f67f0b71548cd7a8e367726cc00a29d9bae94e9626  sources/natives-c-amx-start-prop-anim/start_prop_anim_host.cpp
dc948223de38db9dda1f2e9332046386c2340c2f36b26a8e13f55ca29749a76c  sources/natives-c-amx-start-with-swoop-done/start_with_swoop_done.cpp
f5723b7c7f7e367872b6c0414f4cda9f502294feafa620f996e705beb7cab9c9  sources/natives-c-amx-start-with-swoop-done/start_with_swoop_done_host.cpp
da49092ce18242a67e67e84e142e3b75dc4572f4ca129ca5e97624857766b165  sources/natives-c-amx-stop-following-path/stop_following_path.cpp
064251a12332e6a969c6d5e43bf01d9d73275c14d6aa97fcdc048f2eb979500c  sources/natives-c-amx-stop-following-path/stop_following_path_host.cpp
8437651fffd4d23437880fb507762704b55e9e6970c498e4ee87215bfb8cb966  sources/natives-c-amx-stop-following-player/stop_following_player.cpp
6440c79d7b1e375155c19348de1c93645822737387d98f53c6261389e43a8ef0  sources/natives-c-amx-stop-following-player/stop_following_player_host.cpp
30c8b79fbc3ddd5474dbac7b9ba8357ed2f2c0725d640c5d347a5d0099dc0f10  sources/natives-c-amx-stop-prop-anim/stop_prop_anim.cpp
dde6d8df72c82f39e2aa5ca404907e68dd84c8c7c005bb9746152f3925653280  sources/natives-c-amx-stop-prop-anim/stop_prop_anim_host.cpp
dc2ba4cb1e9e4f4d519edc6e7b6a9741d72786254716ee4472949932ff4d65f2  sources/natives-c-amx-take-player-gold/take_player_gold.cpp
6efe91e221ddae8154e09830b8b6872a40af56a70469c65ef0fc223bc72c2206  sources/natives-c-amx-take-player-gold/take_player_gold_host.cpp
b2cb47fbe234ec56a8a56b6140c020686abca2e2ae74aaeee5f1ea4e3b6b431f  sources/natives-c-amx-trigger/trigger.cpp
dca931d7578a81a30a184fefb293c00d4486b53d3cd39cef3b8e02198826cd45  sources/natives-c-amx-trigger/trigger_host.cpp
01d0fbfd5aeb22eae75b12fd75c15a1a759df2023a003534d0a7818ea41c5942  sources/natives-c-amx-unfriendly/unfriendly.cpp
bbb1b2ba245f08f69bfdc8e40ca1082b1b56f340ae33731b707c003591aac14e  sources/natives-c-amx-unfriendly/unfriendly_host.cpp
```

## Build-plan entries

Append these to `experimental/build-plan.json` `inputs`. Profile `authored_cpp`, kind `independently_authored_source`, empty `header_closure`. This worktree's highest `input_order` is 389 and `default_authored_inputs` is 388. The branch is behind `origin/codex/norrath-first-reconstruction`, so these `input_order` values are provisional for this worktree only: 390 through 436. Renumber them to the next 47 free orders on the integration tip before merging. Do not add the `*_host.cpp` files as inputs. Bump `default_authored_inputs` by 47 when these are the next inputs. `emitted_functions` belongs on the snapshot input, not in the plan object. `private_object_sha256` is the SHA-256 of the EE object from this compile. Reconfirm both with `tools/build_partial.py` when the entries are merged.

```
{ "header_closure": [], "input_order": 390, "kind": "independently_authored_source", "object_name": "set_max_height.o", "profile": "authored_cpp", "sha256": "5ca10f5bcd833f778a11ebb65c87caae0364d9adfdd7b58686b94f95af569f26", "source": "sources/natives-c-set-max-height/set_max_height.cpp", "emitted_functions": [{"name": "SetMaxHeight__6Cameraf", "compiled_body_bytes": 40}], "private_object_sha256": "f8c0a4d8fd73b0ed1287385d02ca166ee7054021de1fecbef01d2d95129ec1c8" }
{ "header_closure": [], "input_order": 391, "kind": "independently_authored_source", "object_name": "set_pitch.o", "profile": "authored_cpp", "sha256": "aa6d7a7f0f016535542d58539dbff9a5cfdc9e30dcd256e566aeaffe89dda16e", "source": "sources/natives-c-set-pitch/set_pitch.cpp", "emitted_functions": [{"name": "SetPitchAtLevel__6Cameraii", "compiled_body_bytes": 100}], "private_object_sha256": "ea5ee8a4f05d82ec6c3539d17ad5689ab2f5662106acf30f0df25df37bffb508" }
{ "header_closure": [], "input_order": 392, "kind": "independently_authored_source", "object_name": "disable_zoom.o", "profile": "authored_cpp", "sha256": "5e741f63659a3dbc773a4fda6bfd609978af9a8fb0996e5a3b5232bd0c6abae3", "source": "sources/natives-c-disable-zoom/disable_zoom.cpp", "emitted_functions": [{"name": "DisableZooming__6Camera", "compiled_body_bytes": 644}], "private_object_sha256": "11d60d636f17ee6aaf45629672db08a96472b72fcb2f742f2a5929cc56ac7e43" }
{ "header_closure": [], "input_order": 393, "kind": "independently_authored_source", "object_name": "set_distance.o", "profile": "authored_cpp", "sha256": "cdaddc8a5d910da92e1707c63784bf5c5ba1dc0dab551ee6a5939d9b9d124b45", "source": "sources/natives-c-set-distance/set_distance.cpp", "emitted_functions": [{"name": "SetDistanceAtLevel__6Cameraif", "compiled_body_bytes": 592}], "private_object_sha256": "c296b13bcb89fec05a9254eadd523228182732fca45c7210e82fcc8e158a17d8" }
{ "header_closure": [], "input_order": 394, "kind": "independently_authored_source", "object_name": "update_dust.o", "profile": "authored_cpp", "sha256": "fce5ee74b5a362aabc68622bfc1bc0f2cf3394a1cc36aa79ee2b6b331b5c221a", "source": "sources/natives-c-update-dust/update_dust.cpp", "emitted_functions": [{"name": "fxUpdateDust__Fiiiiiiii", "compiled_body_bytes": 112}], "private_object_sha256": "a1846c657096c719859c4cf9ad9f883fbfff94af3d9ce092dfe77343daf9a918" }
{ "header_closure": [], "input_order": 395, "kind": "independently_authored_source", "object_name": "game_cam.o", "profile": "authored_cpp", "sha256": "25821bceac06465ca26fe334de7d779601872a03ef5d88cbba14d265fc6ab4bf", "source": "sources/natives-c-game-cam/game_cam.cpp", "emitted_functions": [{"name": "gameCam", "compiled_body_bytes": 4}], "private_object_sha256": "de2a991ce4da92e2a6bc55cd8073b5f51900d26f01cc1ab848f742c0cf8cbb23" }
{ "header_closure": [], "input_order": 396, "kind": "independently_authored_source", "object_name": "level_flags.o", "profile": "authored_cpp", "sha256": "b990639819f4b751d0e5474b01ff376bae6b1e48f87d001afc2fa39757d657da", "source": "sources/natives-c-level-flags/level_flags.cpp", "emitted_functions": [{"name": "levelFlags", "compiled_body_bytes": 4}], "private_object_sha256": "9fc6550712416a7f9bc67d2e403cbc600a7a33b791729c5c98fbdffe1a9a348a" }
{ "header_closure": [], "input_order": 397, "kind": "independently_authored_source", "object_name": "put_players_in_vehicle.o", "profile": "authored_cpp", "sha256": "bb9a79e1bb913a46f0aad5530a266788571f8e9d9b65ba13e3df19dc5c3d3f9b", "source": "sources/natives-c-amx-put-players-in-vehicle/put_players_in_vehicle.cpp", "emitted_functions": [{"name": "amxPutPlayersInVehicle__FP5__amxPi", "compiled_body_bytes": 324}], "private_object_sha256": "e5a641387a5c1d8c66b5aa9479413dca9e17abb3abeff37a4704f95b61d04bb8" }
{ "header_closure": [], "input_order": 398, "kind": "independently_authored_source", "object_name": "reap_enemies.o", "profile": "authored_cpp", "sha256": "f2b9d76b67078e4978f0ebb233ceb419e7d1e3bbca7ab366de9b4a58c8c0573e", "source": "sources/natives-c-amx-reap-enemies/reap_enemies.cpp", "emitted_functions": [{"name": "amxReapEnemies__FP5__amxPi", "compiled_body_bytes": 672}], "private_object_sha256": "9dfcbf087f8f8ea0c1ddf6d7ecb926e5b9e9c51c470e739cf43cc378d8d6c5a2" }
{ "header_closure": [], "input_order": 399, "kind": "independently_authored_source", "object_name": "remove_player_item.o", "profile": "authored_cpp", "sha256": "f144b580f7aee9bac2a39bf3b3f4650daa9bb678f5b99b71783feb207b55a541", "source": "sources/natives-c-amx-remove-player-item/remove_player_item.cpp", "emitted_functions": [{"name": "amxRemovePlayerItem__FP5__amxPi", "compiled_body_bytes": 276}], "private_object_sha256": "38a686b9565de5bb658bc36c04e5a22963cf08e9e497bfc5c9e6030ff9415bc1" }
{ "header_closure": [], "input_order": 400, "kind": "independently_authored_source", "object_name": "remove_quest.o", "profile": "authored_cpp", "sha256": "af2825c8149136f4850ac93706ebae621470c46b82d5fb672fb99b42e3846fe9", "source": "sources/natives-c-amx-remove-quest/remove_quest.cpp", "emitted_functions": [{"name": "amxRemoveQuest__FP5__amxPi", "compiled_body_bytes": 60}], "private_object_sha256": "93af649c83fbddbf5776ed3eb1929f67f50eedc36fe2b476e68b6380433e06c7" }
{ "header_closure": [], "input_order": 401, "kind": "independently_authored_source", "object_name": "save_checkpoint.o", "profile": "authored_cpp", "sha256": "2038bd13991adec4007d16aa14d587eeaf6146253a455acf6916be82d7bc5c4e", "source": "sources/natives-c-amx-save-checkpoint/save_checkpoint.cpp", "emitted_functions": [{"name": "amxSaveCheckPoint__FP5__amxPi", "compiled_body_bytes": 52}], "private_object_sha256": "cf6d8b4e3ec41cb7b595d08ea366e72b5e260ab37335780e049e4eaf0184c90e" }
{ "header_closure": [], "input_order": 402, "kind": "independently_authored_source", "object_name": "save_done.o", "profile": "authored_cpp", "sha256": "92595c384f0143c85df75e932de1c114e531ec6a981d622c584b2c6cf39d43ca", "source": "sources/natives-c-amx-save-done/save_done.cpp", "emitted_functions": [{"name": "amxSaveDone__FP5__amxPi", "compiled_body_bytes": 36}], "private_object_sha256": "ed6bb410b0cab01fab8dad62c7be7dff0919b2078e028803f8e265b88877e6f0" }
{ "header_closure": [], "input_order": 403, "kind": "independently_authored_source", "object_name": "save_end_config.o", "profile": "authored_cpp", "sha256": "bad25dd0616ccc91e061c89eb0693a31587e9842a3caf044bd6463ffdc292ce9", "source": "sources/natives-c-amx-save-end-config/save_end_config.cpp", "emitted_functions": [{"name": "amxSaveEndConfig__FP5__amxPi", "compiled_body_bytes": 284}], "private_object_sha256": "59a42cd879743318f34762320fad3d4e4650614cc28d6dc2c3f3c81d41738cdc" }
{ "header_closure": [], "input_order": 404, "kind": "independently_authored_source", "object_name": "save_game.o", "profile": "authored_cpp", "sha256": "0f10a0901986822ae2c22309404ff7405caa423c0a8300bdc31c756953b4969f", "source": "sources/natives-c-amx-save-game/save_game.cpp", "emitted_functions": [{"name": "amxSaveGame__FP5__amxPi", "compiled_body_bytes": 40}], "private_object_sha256": "ad2fa0e85013a88946643f20f46489ae0e682a599e393a73e2c71be0bfda3a6d" }
{ "header_closure": [], "input_order": 405, "kind": "independently_authored_source", "object_name": "set_best_active_player.o", "profile": "authored_cpp", "sha256": "cff08ef11c518d69957ff291e38629ed8fa13f99daf0094b202e6fef0878522d", "source": "sources/natives-c-amx-set-best-active-player/set_best_active_player.cpp", "emitted_functions": [{"name": "amxSetBestActivePlayer__FP5__amxPi", "compiled_body_bytes": 128}], "private_object_sha256": "334954b29550a19a1f4d03c0215d446ef6a69cf9864e855f2bd243ebd7c09b9b" }
{ "header_closure": [], "input_order": 406, "kind": "independently_authored_source", "object_name": "set_entrance.o", "profile": "authored_cpp", "sha256": "f2ea2cfc758a03a0cbd69bb2eeb570ec1744c5e85a49273ad63e8fbea1f3444d", "source": "sources/natives-c-amx-set-entrance/set_entrance.cpp", "emitted_functions": [{"name": "amxSetEntrance__FP5__amxPi", "compiled_body_bytes": 68}], "private_object_sha256": "be62ae8695ba49efd6d59fb267c846be1e1d2e8bd023ee4fcb0aa3b7a0336d8b" }
{ "header_closure": [], "input_order": 407, "kind": "independently_authored_source", "object_name": "set_far_clip.o", "profile": "authored_cpp", "sha256": "9d84b9e01cb06ce8f69fdfaef0ae84dd8d40142f2e9d115d9290f8b30dbd729f", "source": "sources/natives-c-amx-set-far-clip/set_far_clip.cpp", "emitted_functions": [{"name": "amxSetFarClip__FP5__amxPi", "compiled_body_bytes": 40}], "private_object_sha256": "1798ce5754496506a56171bde0db008eefd7714efea3ade6d8f4e72f54e8edee" }
{ "header_closure": [], "input_order": 408, "kind": "independently_authored_source", "object_name": "set_hit_points.o", "profile": "authored_cpp", "sha256": "e3d6b59b90146e7e953d9ca5378c41d04a98916ca522245fbc4f000fa11fed12", "source": "sources/natives-c-amx-set-hit-points/set_hit_points.cpp", "emitted_functions": [{"name": "amxSetHitPoints__FP5__amxPi", "compiled_body_bytes": 140}], "private_object_sha256": "f553b5636297e7ae9f8d3140b338bd9ecc0d5325f67b8e4be39f2630b786d00a" }
{ "header_closure": [], "input_order": 409, "kind": "independently_authored_source", "object_name": "set_info.o", "profile": "authored_cpp", "sha256": "7bba098df52b1db532413a7fa3b5832aa7f78d4777f2895c9bc0bfc1ace7fd16", "source": "sources/natives-c-amx-set-info/set_info.cpp", "emitted_functions": [{"name": "amxSetInfo__FP5__amxPi", "compiled_body_bytes": 80}], "private_object_sha256": "7445367db3d54cadbcb0cbdf4b24fa86ce475236618847e8878be9d2add57151" }
{ "header_closure": [], "input_order": 410, "kind": "independently_authored_source", "object_name": "set_no_collide.o", "profile": "authored_cpp", "sha256": "fd8a2b72f9c600f7e9fff8472e131c4e0943a5a09013aae779c75362132e640c", "source": "sources/natives-c-amx-set-no-collide/set_no_collide.cpp", "emitted_functions": [{"name": "amxSetNoCollide__FP5__amxPi", "compiled_body_bytes": 224}], "private_object_sha256": "b032dc6ad2d28111efdf80cb6305ec5f14c259dd9c387d84e608d43f78e84e0b" }
{ "header_closure": [], "input_order": 411, "kind": "independently_authored_source", "object_name": "set_particle.o", "profile": "authored_cpp", "sha256": "717906bd8d74c3eb45c223e6074489ad8ed4fc0b6bad0c56be708ac1d342d785", "source": "sources/natives-c-amx-set-particle/set_particle.cpp", "emitted_functions": [{"name": "amxSetParticle__FP5__amxPi", "compiled_body_bytes": 140}], "private_object_sha256": "3f4771f7779a5a645e2ff191bac620a050d23ecd4714d64e0516ca213c6265a1" }
{ "header_closure": [], "input_order": 412, "kind": "independently_authored_source", "object_name": "set_post_action.o", "profile": "authored_cpp", "sha256": "65098b1dde149bc4e22360a7c32f3417a936ccc0bca016015b1f1a4c531ccb24", "source": "sources/natives-c-amx-set-post-action/set_post_action.cpp", "emitted_functions": [{"name": "amxSetPostAction__FP5__amxPi", "compiled_body_bytes": 60}], "private_object_sha256": "cfd1b67f71ff5d73ebf1b3894cdf64a629bc4917e975b80d4fd675e09874360d" }
{ "header_closure": [], "input_order": 413, "kind": "independently_authored_source", "object_name": "set_save_inhibit.o", "profile": "authored_cpp", "sha256": "b0280e0ecaad031f72afcea475a0117539c42a84dcf3af689e1d118355b088db", "source": "sources/natives-c-amx-set-save-inhibit/set_save_inhibit.cpp", "emitted_functions": [{"name": "amxSetSaveInhibit__FP5__amxPi", "compiled_body_bytes": 20}], "private_object_sha256": "6fb58d3a6aae9c309fc3e4066342a20a076f35359fc82e22e5785b9fd095752d" }
{ "header_closure": [], "input_order": 414, "kind": "independently_authored_source", "object_name": "setup_player_sel.o", "profile": "authored_cpp", "sha256": "d3ce63de5951b9be09fa9803fea5fea7b528165a15598274c5be7323e85d47f8", "source": "sources/natives-c-amx-setup-player-sel/setup_player_sel.cpp", "emitted_functions": [{"name": "amxSetupPlayerSel__FP5__amxPi", "compiled_body_bytes": 32}], "private_object_sha256": "ba853ddc2dc205101735ff89aaaf624315cbaa29f140840df087637c9e7283c8" }
{ "header_closure": [], "input_order": 415, "kind": "independently_authored_source", "object_name": "setv.o", "profile": "authored_cpp", "sha256": "f6a958fe33cf91f692810b9c79873481005702f9d98205746ba6724ca10aad6e", "source": "sources/natives-c-amx-setv/setv.cpp", "emitted_functions": [{"name": "amxSetv__FP5__amxPi", "compiled_body_bytes": 76}], "private_object_sha256": "80b1f9e9568bff64c7b135bcb6a0b12f5a32a139e18cd5b62cd5852332deb8e0" }
{ "header_closure": [], "input_order": 416, "kind": "independently_authored_source", "object_name": "shake_camera.o", "profile": "authored_cpp", "sha256": "3dd5861a4236c33d6b83d71156a2fe2ed49ad098c3be2938d471b73b8ae3c048", "source": "sources/natives-c-amx-shake-camera/shake_camera.cpp", "emitted_functions": [{"name": "amxShakeCamera__FP5__amxPi", "compiled_body_bytes": 44}], "private_object_sha256": "70d327c860b0a5a99860ce719c69469d6c1d4f688977a69b36cba24331262a35" }
{ "header_closure": [], "input_order": 417, "kind": "independently_authored_source", "object_name": "shop_running.o", "profile": "authored_cpp", "sha256": "3068247b91a3ca0e1cdf720c97d935c9f78a763d975d77da3ab2acc0a352edfe", "source": "sources/natives-c-amx-shop-running/shop_running.cpp", "emitted_functions": [{"name": "amxShopRunning__FP5__amxPi", "compiled_body_bytes": 12}], "private_object_sha256": "9c97371feecf298ae05878aab03d6ce09f19ce2afb9876fcca01572b09564e93" }
{ "header_closure": [], "input_order": 418, "kind": "independently_authored_source", "object_name": "show_bust.o", "profile": "authored_cpp", "sha256": "6d4e6ba9603445ba3607785e0c7b29241eee86588e11b3879fa32a2e8ec65e6c", "source": "sources/natives-c-amx-show-bust/show_bust.cpp", "emitted_functions": [{"name": "amxShowBust__FP5__amxPi", "compiled_body_bytes": 132}], "private_object_sha256": "60003b7cfbec6cfecf8968e80e50dea103b2ac4e340f1b46dcf055f212b92d05" }
{ "header_closure": [], "input_order": 419, "kind": "independently_authored_source", "object_name": "show_compass.o", "profile": "authored_cpp", "sha256": "93ff37f4fb827ae554d836cccfee72fd4799eec72f43a72716d648686abb1f58", "source": "sources/natives-c-amx-show-compass/show_compass.cpp", "emitted_functions": [{"name": "amxShowCompass__FP5__amxPi", "compiled_body_bytes": 176}], "private_object_sha256": "dd300be11cf9c55060b30ca3bc981ce4ec75faec888864a11de20e24c64b4abc" }
{ "header_closure": [], "input_order": 420, "kind": "independently_authored_source", "object_name": "show_monster.o", "profile": "authored_cpp", "sha256": "aa9879dd44bc0dca4544b13384971053f144d60ad9fb4da7c151b82914b37a47", "source": "sources/natives-c-amx-show-monster/show_monster.cpp", "emitted_functions": [{"name": "amxShowMonster__FP5__amxPi", "compiled_body_bytes": 172}], "private_object_sha256": "1a8beebb6351c2c5ac0e59d02b3fd21ee37c966823fa6dc61006eb99459b751d" }
{ "header_closure": [], "input_order": 421, "kind": "independently_authored_source", "object_name": "simple_new_world.o", "profile": "authored_cpp", "sha256": "e41b6e26b158486651ad58e304618de0ffaa8207302fb21e72aa1c116aa683cf", "source": "sources/natives-c-amx-simple-new-world/simple_new_world.cpp", "emitted_functions": [{"name": "amxSimpleNewWorld__FP5__amxPi", "compiled_body_bytes": 340}], "private_object_sha256": "7f072b243dcaf459a8857f9f86b42c1c2a3c8e246f16779909670f2321eccf41" }
{ "header_closure": [], "input_order": 422, "kind": "independently_authored_source", "object_name": "sound_sequence.o", "profile": "authored_cpp", "sha256": "de5e43f8aecd92c4f34b4f27bcff90043e3c52e023a39c2ec63618ce0778cb28", "source": "sources/natives-c-amx-sound-sequence/sound_sequence.cpp", "emitted_functions": [{"name": "amxSoundSequence__FP5__amxPi", "compiled_body_bytes": 144}], "private_object_sha256": "220539a814b96c81330a4249de7e0378d510acecedc2e60e3ba08434507ffd13" }
{ "header_closure": [], "input_order": 423, "kind": "independently_authored_source", "object_name": "sound.o", "profile": "authored_cpp", "sha256": "df1a5dd5cf3774170d7e2c16fc5c1139a2b8597e6ac9fba3d9320991fc8a07e0", "source": "sources/natives-c-amx-sound/sound.cpp", "emitted_functions": [{"name": "amxSound__FP5__amxPi", "compiled_body_bytes": 152}], "private_object_sha256": "c74019b59392d66b605ef1cc21d2768421a6414da99fcdc133bd5a293e088346" }
{ "header_closure": [], "input_order": 424, "kind": "independently_authored_source", "object_name": "start_audio.o", "profile": "authored_cpp", "sha256": "a4d1e4c167757c5683d9dd8c01e8e9f7f412065590f970ed002036e19c409081", "source": "sources/natives-c-amx-start-audio/start_audio.cpp", "emitted_functions": [{"name": "amxStartAudio__FP5__amxPi", "compiled_body_bytes": 68}], "private_object_sha256": "11f5d822bc09e1f76223a58e18055fb3db865bba717fb4f01b82a37138e4f434" }
{ "header_closure": [], "input_order": 425, "kind": "independently_authored_source", "object_name": "start_dialog.o", "profile": "authored_cpp", "sha256": "f23235f3cc098826b907494a653c43a1c8cea5834fec420281bf752997e4fd9f", "source": "sources/natives-c-amx-start-dialog/start_dialog.cpp", "emitted_functions": [{"name": "amxStartDialog__FP5__amxPi", "compiled_body_bytes": 196}], "private_object_sha256": "28d400d0ce96c42d76c959a88a19642bb530a63160e9450c98a0921ab5cd9618" }
{ "header_closure": [], "input_order": 426, "kind": "independently_authored_source", "object_name": "start_music.o", "profile": "authored_cpp", "sha256": "c99f8a6bbf9c79923eb80647d00eb7dbadb97c21136523d77cb2c15a8a87811c", "source": "sources/natives-c-amx-start-music/start_music.cpp", "emitted_functions": [{"name": "amxStartMusic__FP5__amxPi", "compiled_body_bytes": 64}], "private_object_sha256": "161e6805e458eafa1fd6ae073bd9de4af9d57e75aa39f183cbdd9e716d40f4c8" }
{ "header_closure": [], "input_order": 427, "kind": "independently_authored_source", "object_name": "start_npc_anim.o", "profile": "authored_cpp", "sha256": "00cafac2cb9b20e1275a3f50dcd43b22925212b40b50ea80632fe2f920624a33", "source": "sources/natives-c-amx-start-npc-anim/start_npc_anim.cpp", "emitted_functions": [{"name": "amxStartNPCAnim__FP5__amxPi", "compiled_body_bytes": 344}], "private_object_sha256": "c8f57b3016256867d3b5d3cea00a689b4f76006b27092b1ea9eef279a9a69409" }
{ "header_closure": [], "input_order": 428, "kind": "independently_authored_source", "object_name": "start_player_sel.o", "profile": "authored_cpp", "sha256": "09000b10e0af9be8f00baee788688d4617b0321ed2d0547555c98d7c5b3978da", "source": "sources/natives-c-amx-start-player-sel/start_player_sel.cpp", "emitted_functions": [{"name": "amxStartPlayerSel__FP5__amxPi", "compiled_body_bytes": 32}], "private_object_sha256": "922348f0d14ccbfaa0fb97d3b1888828bfcfd198122d2c6b1a27c7d36edc8842" }
{ "header_closure": [], "input_order": 429, "kind": "independently_authored_source", "object_name": "start_prop_anim.o", "profile": "authored_cpp", "sha256": "657591b4541b07cbf31c93cde5642c4a25253eccf78d1a0cfe0786d7fb7402be", "source": "sources/natives-c-amx-start-prop-anim/start_prop_anim.cpp", "emitted_functions": [{"name": "amxStartPropAnim__FP5__amxPi", "compiled_body_bytes": 320}], "private_object_sha256": "627fb9db1985bbce18fa8c5f6a1baa5b7b43b7d385bf302005bc0bc60081022d" }
{ "header_closure": [], "input_order": 430, "kind": "independently_authored_source", "object_name": "start_with_swoop_done.o", "profile": "authored_cpp", "sha256": "dc948223de38db9dda1f2e9332046386c2340c2f36b26a8e13f55ca29749a76c", "source": "sources/natives-c-amx-start-with-swoop-done/start_with_swoop_done.cpp", "emitted_functions": [{"name": "amxStartWithSwoopDone__FP5__amxPi", "compiled_body_bytes": 20}], "private_object_sha256": "db1c1a65f09a3760c19900859b5ee3f92f72d5844fd8dcc1366fe0bfdc0791b1" }
{ "header_closure": [], "input_order": 431, "kind": "independently_authored_source", "object_name": "stop_following_path.o", "profile": "authored_cpp", "sha256": "da49092ce18242a67e67e84e142e3b75dc4572f4ca129ca5e97624857766b165", "source": "sources/natives-c-amx-stop-following-path/stop_following_path.cpp", "emitted_functions": [{"name": "amxStopFollowingPath__FP5__amxPi", "compiled_body_bytes": 280}], "private_object_sha256": "0c0a188157fdea6a66db864b3a860e6141ace14f67c3c6bb51a5ae80ea16821e" }
{ "header_closure": [], "input_order": 432, "kind": "independently_authored_source", "object_name": "stop_following_player.o", "profile": "authored_cpp", "sha256": "8437651fffd4d23437880fb507762704b55e9e6970c498e4ee87215bfb8cb966", "source": "sources/natives-c-amx-stop-following-player/stop_following_player.cpp", "emitted_functions": [{"name": "amxStopFollowingPlayer__FP5__amxPi", "compiled_body_bytes": 248}], "private_object_sha256": "90ea5b8a682b3c21e96946380506d8d8a4641fb1b1c8f8b6a5ee3f34e520a047" }
{ "header_closure": [], "input_order": 433, "kind": "independently_authored_source", "object_name": "stop_prop_anim.o", "profile": "authored_cpp", "sha256": "30c8b79fbc3ddd5474dbac7b9ba8357ed2f2c0725d640c5d347a5d0099dc0f10", "source": "sources/natives-c-amx-stop-prop-anim/stop_prop_anim.cpp", "emitted_functions": [{"name": "amxStopPropAnim__FP5__amxPi", "compiled_body_bytes": 64}], "private_object_sha256": "b6a03f2dd6d2e0b86ace249e8f79313706d5781072f0e61a7eb27aea419d0757" }
{ "header_closure": [], "input_order": 434, "kind": "independently_authored_source", "object_name": "take_player_gold.o", "profile": "authored_cpp", "sha256": "dc2ba4cb1e9e4f4d519edc6e7b6a9741d72786254716ee4472949932ff4d65f2", "source": "sources/natives-c-amx-take-player-gold/take_player_gold.cpp", "emitted_functions": [{"name": "amxTakePlayerGold__FP5__amxPi", "compiled_body_bytes": 148}], "private_object_sha256": "453cc333f6b77c3666c53f52c98dbf4b51559cc733447370bc029768609dff05" }
{ "header_closure": [], "input_order": 435, "kind": "independently_authored_source", "object_name": "trigger.o", "profile": "authored_cpp", "sha256": "b2cb47fbe234ec56a8a56b6140c020686abca2e2ae74aaeee5f1ea4e3b6b431f", "source": "sources/natives-c-amx-trigger/trigger.cpp", "emitted_functions": [{"name": "amxTrigger__FP5__amxPi", "compiled_body_bytes": 252}], "private_object_sha256": "2412f77ff5f870f9376808a3f6b1a77b652cb774f10a0d30e54e58802efc27ef" }
{ "header_closure": [], "input_order": 436, "kind": "independently_authored_source", "object_name": "unfriendly.o", "profile": "authored_cpp", "sha256": "01d0fbfd5aeb22eae75b12fd75c15a1a759df2023a003534d0a7818ea41c5942", "source": "sources/natives-c-amx-unfriendly/unfriendly.cpp", "emitted_functions": [{"name": "amxUnFriendly__FP5__amxPi", "compiled_body_bytes": 332}], "private_object_sha256": "488b508d5f5460ddcda13223a977ad2ad9691fa5887818520c5ae645e8978fb3" }
```

## Imports closed

`SetMaxHeight__6Cameraf`, `SetPitchAtLevel__6Cameraii`, `DisableZooming__6Camera`, `SetDistanceAtLevel__6Cameraif`, `fxUpdateDust__Fiiiiiiii`, `gameCam`, `levelFlags`, `amxPutPlayersInVehicle__FP5__amxPi`, `amxReapEnemies__FP5__amxPi`, `amxRemovePlayerItem__FP5__amxPi`, `amxRemoveQuest__FP5__amxPi`, `amxSaveCheckPoint__FP5__amxPi`, `amxSaveDone__FP5__amxPi`, `amxSaveEndConfig__FP5__amxPi`, `amxSaveGame__FP5__amxPi`, `amxSetBestActivePlayer__FP5__amxPi`, `amxSetEntrance__FP5__amxPi`, `amxSetFarClip__FP5__amxPi`, `amxSetHitPoints__FP5__amxPi`, `amxSetInfo__FP5__amxPi`, `amxSetNoCollide__FP5__amxPi`, `amxSetParticle__FP5__amxPi`, `amxSetPostAction__FP5__amxPi`, `amxSetSaveInhibit__FP5__amxPi`, `amxSetupPlayerSel__FP5__amxPi`, `amxSetv__FP5__amxPi`, `amxShakeCamera__FP5__amxPi`, `amxShopRunning__FP5__amxPi`, `amxShowBust__FP5__amxPi`, `amxShowCompass__FP5__amxPi`, `amxShowMonster__FP5__amxPi`, `amxSimpleNewWorld__FP5__amxPi`, `amxSoundSequence__FP5__amxPi`, `amxSound__FP5__amxPi`, `amxStartAudio__FP5__amxPi`, `amxStartDialog__FP5__amxPi`, `amxStartMusic__FP5__amxPi`, `amxStartNPCAnim__FP5__amxPi`, `amxStartPlayerSel__FP5__amxPi`, `amxStartPropAnim__FP5__amxPi`, `amxStartWithSwoopDone__FP5__amxPi`, `amxStopFollowingPath__FP5__amxPi`, `amxStopFollowingPlayer__FP5__amxPi`, `amxStopPropAnim__FP5__amxPi`, `amxTakePlayerGold__FP5__amxPi`, `amxTrigger__FP5__amxPi`, `amxUnFriendly__FP5__amxPi`.

## Imports opened

`__eq__C9type_infoRC9type_info`, `__tfP4Prop`, `__tfP6PhProp`, `__tfP8ItemProp`, `activePlayer`, `amx_GetAddr`, `amx_GetString`, `animAdd__FP14AnimationStateP15AnimationHeaderi`, `animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi`, `animInitState__FP14AnimationState`, `animResetFramestates__FP14AnimationState`, `animSetToEnd__FP14AnimationState`, `DLG_StartDialog__FPcP9DlgPacketii`, `DLG_StartWithSwoopDone`, `findTagString__10GameObjectPci`, `floorDrawEnable`, `fxDust`, `gameDifficulty`, `gameNetKick__Fi`, `gameNoDraw`, `gameOver`, `gamePause`, `gameRecallInhibit`, `gameSetCameraShake__Ffb`, `hudSetInfoMessage__FPUsiic`, `hudShowBust__FPcPUsii`, `hudShowCompass__FG6Point3PUs`, `inventoryGetItemIndex__FiPcPiT2`, `levelName`, `lumpFind__FPvPc`, `lumpFindResource__FPcT0`, `lumpLoad__FPc`, `MC_GameConfig`, `MC_GetAsyncMode__Fv`, `MC_SaveCheckPoint__Fiii`, `MC_SaveConfig__Fv`, `MC_SaveGameBrowser__Fi`, `netGetLocalPlayerNm__Fi`, `netNetworkMode`, `netPlayerIsLocal__Fi`, `nmPlayers`, `objectDrawEnable`, `objectFindInBox__FG6Point3T0PP10GameObjecti`, `objectFindObject__FPc`, `objectFindObject__FPcPP10GameObjecti`, `objectUpdateInGrid__FP10GameObject`, `P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v`, `player`, `playerSelectionSetup__Fv`, `playerSelectionStart__Fv`, `removeItemFromInventory__Fiiii`, `removeQuestFromLog__FPc`, `scePrintf`, `scriptInPostAction`, `scriptParticles`, `scriptSetGameState__FPci`, `scriptSetPostAction__FPc`, `shopRunning`, `soundMakeSequenceUnpausable__Fi`, `soundPlaySequence__FP15AnimationHeaderP10GameObjectf`, `soundPlaySound__FP10_vagHeaderP6Point3fff`, `soundStartDialogStream__FiPci`, `soundStartMusic__FPci`, `sprintf`, `startEntranceName`, `strcpy`, `textGetString__FPc`, `worldSetFarClip__Ff`.

`scePrintf` is already emitted by the libc-format unit. This package only references it. `sprintf` and `strcpy` are the original symbols, not the `norrath_format_string` body.

## Build-plan entries needed

The 47 entries in the build-plan section above. Empty `header_closure` on every one. No host file is an input. The integrator assigns live `input_order` values and adds the snapshot `files` hashes from the section above.

## Authored but never executed

All 47 EE objects in the table. The host tests ran on this machine. Nothing from this package was executed on the EE, linked into a partial image, or run as the game. `amx_GetAddr`, `amx_GetString`, and every opened callee were stubs in those host tests.

## Unknown

None of the compiled bodies is an original-byte match. Register allocation, delay slots, and the original stack frame sizes are not reproduced. The type-id call's leftover first argument and `amxTrigger`'s leftover third argument are not given meaning. `amxReapEnemies` still contains the null-object call to `__throw_bad_typeid`, but the reviewed `-O2` object does not reference that symbol. The flags word is loaded before that test, so a null object would already have faulted. The unaligned path and compass copies are byte copies. Their original access width is not claimed.

`player`, `fxDust`, `scriptParticles`, `levelName`, `MC_GameConfig`, `startEntranceName`, and the draw and network words are imports. Their retail sizes were not re-authored. `scriptParticles` is indexed as a pointer array. The retail object is 12 bytes. Natives before `amxPutPlayersInVehicle` in byte order belong to other packages. The ten `worldWater*` words belong to world-state. Hardware timing, network kicks, inventory, animation, and dialog playback were not run.

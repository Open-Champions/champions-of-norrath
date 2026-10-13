# w9-startup-crt-a: startup and runtime support (a)

Branch `agents/w9-startup-crt-a`. Every source lives under
`experimental/sources/w9-startup-crt-a-*`. Each function is its own commit and
compiled with the reviewed EE toolchain at the shared `authored_cpp` flags (the
assembly unit with the same assembler). Each was run against a host contract
that stubs its callees and fails when a stored value, branch or call order is
wrong. No game binary, disassembly or extracted data is committed; the one
comparison script, `w9-startup-crt-a-check.py`, reads a private hash-pinned
executable and prints only a pass line.

## Imports closed (by this package)

Functions: AllocTimerCounter, CallRpc, InitTLB, InvalidDCache, MemcpyS,
SetTimerHandler, StartTimerCounter, TerminateThread, UnpreparePowerOff__Fv,
`_GLOBAL_$I$hudNoDraw`, `__11AddParticleG6Point3PPci`, `__11ParticleDef`,
`__11FireBombletG6Point3sPcT1P6Point3i`, `__12RecallEffectG6Point3`,
`__13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description`,
`__14AnimationStateRC14AnimationState`, `__3NPCG6Point3sPcN33ifPPc`, `__3Webii`.

Data: `_14WoodElfSoldier$m_commanderList`, `_3Orc$m_commanderList`,
`_6Wraith$s_rWraithList`, `_3map$mapOffset`, `_3map$mapwidth`,
`_3map$mapheight`, `_3map$maptiles`, `_4item$globalItem`, `_4item$picBuffer`,
`_4item$picInProgress`, `_4item$picList`, `_4item$picLoadBuffer`,
`_4item$picLoadFinished`, `_4item$picsToLoad`.

Closed elsewhere (already authored by other units on the shared branch, nothing
added here): `_GLOBAL_$D$lastkey`, `_GLOBAL_$D$objectIgnoreFlags`,
`_GLOBAL_$I$addpmlist__FPc`, `_GLOBAL_$I$lastkey`,
`_GLOBAL_$I$objectIgnoreFlags` (w8-object-list-math units) and
`__16ListSafeIteratorP8ListHeadi` (w8-object-list-math-safe-iterator-construct).

## Imports opened (left unresolved, owned elsewhere)

DIntr, EIntr, iAllocTimerCounter, iStartTimerCounter, GetMemorySize,
InitTLB32MB, _InitTLB, _sceIDC, scePrintf, memcpy, strcpy, TerminateThread (from
UnpreparePowerOff), sceCdPOffCallback, tid, blockAlloc__Fi,
`__11ResourceMapPcT1`, `__15GlowingCylinderG6Point3iiii`, `_vt$11AddParticle`,
`_vt$11FireBomblet`, `_vt$10Projectile`, `_vt$13MissileWeapon`,
`_vt$12RecallEffect`, `_vt$3NPC`, particleInit__11AddParticlePPc,
lightInit__11AddParticlePPc, objectAddToRunList__FP10GameObject,
listOnewayLink__FP8ListHeadT0Pv, lightDeactivate__FP5Light,
lumpFindResource__FPcT0, soundPlaySequence__FP15AnimationHeaderP10GameObjectf,
soundPlaySound__FP10_vagHeaderP6Point3fff, P_StartTrail__Fiifiiiii,
P_ExtendTrail__FiG6Point3, frameNm, levelFlags, playedStinger, and the provider
names used by several units for routines whose authored owner uses another
spelling: norrath_game_object_construct, norrath_light_add,
norrath_lump_find_resource, norrath_actor_* (animation, model chunks, slow run
list, tag string), norrath_creature_finish_construction,
norrath_animation_frame_state_acquire, norrath_actors_a_view_438e98. The
lead integrator must map these provider names to the executable names (or the
other way round) when merging; until then they stay unresolved.

## Build-plan entries needed

Not added (shared file). Each is `kind: independently_authored_source`,
`profile: authored_cpp`, object name the source stem plus `.o`, source path as
below, sha256 of the file as committed:

- w9-startup-crt-a-add-particle-ctor-numbered/add_particle_ctor_numbered.cpp
- w9-startup-crt-a-alloc-timer-counter/alloc_timer_counter.cpp
- w9-startup-crt-a-animation-state-copy-ctor/animation_state_copy_ctor.cpp
- w9-startup-crt-a-call-rpc/call_rpc.cpp
- w9-startup-crt-a-commander-lists/commander_lists.cpp
- w9-startup-crt-a-fire-bomblet-ctor/fire_bomblet_ctor.cpp
- w9-startup-crt-a-global-init-hud-no-draw/global_init_hud_no_draw.cpp
- w9-startup-crt-a-init-tlb/init_tlb.cpp
- w9-startup-crt-a-invalid-dcache/invalid_dcache.cpp
- w9-startup-crt-a-item-global-item/global_item.cpp
- w9-startup-crt-a-item-pic-state/pic_state.cpp
- w9-startup-crt-a-map-data/map_data.cpp
- w9-startup-crt-a-memcpy-s/memcpy_s.cpp
- w9-startup-crt-a-missile-weapon-ctor/missile_weapon_ctor.cpp
- w9-startup-crt-a-npc-named-ctor/npc_named_ctor.cpp
- w9-startup-crt-a-particle-def-default-ctor/particle_def_default_ctor.cpp
- w9-startup-crt-a-recall-effect-ctor/recall_effect_ctor.cpp
- w9-startup-crt-a-set-timer-handler/set_timer_handler.cpp
- w9-startup-crt-a-start-timer-counter/start_timer_counter.cpp
- w9-startup-crt-a-terminate-thread/terminate_thread.S (assembly, same pattern as audio-add-dma-handler/add_dma_handler.S)
- w9-startup-crt-a-unprepare-power-off/unprepare_power_off.cpp
- w9-startup-crt-a-web-ctor/web_ctor.cpp

The `host_contract.cpp` / `host_check.cpp` files are tests, not plan inputs.

## Authored but never executed

Nothing here has run on an EE, an emulator or the game. The host contracts run
the authored code on the build machine against stub callees. The data units
(commander lists, map data, item pic state, global item table) are only
compiled; the global item name table was compared in order with the private
executable by `w9-startup-crt-a-check.py`, which needs `--original` pointing at
the private file. Compiled bodies are not claimed to match original bytes.

## Unknown

- Meaning of the MissileWeapon kind values and integers, of the description
  entry keys 21, 22 and 24, and of the six integer arguments.
- Element types of the three commander/Wraith lists (read as list head words,
  inferred from their static initialisers and destructors).
- Tile layout of `_3map$maptiles` (93312 bytes; 432 x 216 is only arithmetic).
- True size of `_4item$picLoadBuffer` (32768 is an upper bound from the next
  symbol); why `picInProgress` starts at -100000.
- What the kernel does for TerminateThread's service number, and the actual
  names of the provider routines listed above.
- Original storage placement: zero-initialised definitions here land in
  uninitialised data while the executable has them in initialised data.

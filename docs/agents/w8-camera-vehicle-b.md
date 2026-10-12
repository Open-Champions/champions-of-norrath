# Camera and vehicle methods (2/2)

Package `w8-camera-vehicle-b`, branch `agents/w8-camera-vehicle-b`. Private
reference: `work/private/SLUS_205.65`; addresses below are that file's symbol
values. No game bytes, disassembly or decompiler output are committed. Each
function was compiled with the reviewed EE g++ `authored_cpp` flags and has a
host contract under its `host/` directory (not a plan input) that returns
non-zero when the behavior differs. Several contracts were checked with
deliberate source mutations, which they caught. The two `set` contracts
use a table of the original's (offset, word) stores, extracted locally from the
reference with a throwaway straight-line decoder and committed only as numbers.

Compiled bodies differ in size and instruction choice from the image extents.
These are behavior reconstructions, not byte matches. Nothing here has run on
the EE or as gameplay.

## Functions

| source (under `experimental/`) | symbol | compiled body bytes |
|---|---|---|
| `sources/w8-camera-vehicle-b-lava-event-activate/lava_event_activate.cpp` | `eventActivate__11LavaTractor` | 148 |
| `sources/w8-camera-vehicle-b-lava-event-collision-ground/lava_event_collision_ground.cpp` | `eventCollisionGround__11LavaTractor` | 8 |
| `sources/w8-camera-vehicle-b-lava-event-collision-wall/lava_event_collision_wall.cpp` | `eventCollisionWall__11LavaTractorP6Point3T1` | 236 |
| `sources/w8-camera-vehicle-b-lava-event-cur-rpm/lava_event_cur_rpm.cpp` | `eventCurRPM__11LavaTractor` | 88 |
| `sources/w8-camera-vehicle-b-lava-event-deactivate/lava_event_deactivate.cpp` | `eventDeActivate__11LavaTractor` | 44 |
| `sources/w8-camera-vehicle-b-lava-event-gas-off/lava-event-gas-off.cpp` | `eventGasOff__11LavaTractor` | 156 |
| `sources/w8-camera-vehicle-b-lava-event-gas-on/lava-event-gas-on.cpp` | `eventGasOn__11LavaTractor` | 156 |
| `sources/w8-camera-vehicle-b-lava-event-steering-change/lava-event-steering-change.cpp` | `eventSteeringChange__11LavaTractor` | 156 |
| `sources/w8-camera-vehicle-b-lava-set/lava_set.cpp` | `set__11LavaTractor` | 596 |
| `sources/w8-camera-vehicle-b-skul-set/skul_set.cpp` | `set__8SkulBoat` | 316 |
| `sources/w8-camera-vehicle-b-vehicle-event-activate/vehicle-event-activate.cpp` | `eventActivate__7Vehicle` | 8 |
| `sources/w8-camera-vehicle-b-vehicle-event-collision-ground/vehicle-event-collision-ground.cpp` | `eventCollisionGround__7Vehicle` | 8 |
| `sources/w8-camera-vehicle-b-vehicle-event-collision-wall/vehicle-event-collision-wall.cpp` | `eventCollisionWall__7VehicleP6Point3T1` | 8 |
| `sources/w8-camera-vehicle-b-vehicle-event-cur-rpm/vehicle-event-cur-rpm.cpp` | `eventCurRPM__7Vehicle` | 8 |
| `sources/w8-camera-vehicle-b-vehicle-event-deactivate/vehicle-event-deactivate.cpp` | `eventDeActivate__7Vehicle` | 8 |
| `sources/w8-camera-vehicle-b-vehicle-event-gas-off/vehicle-event-gas-off.cpp` | `eventGasOff__7Vehicle` | 8 |
| `sources/w8-camera-vehicle-b-vehicle-event-gas-on/vehicle-event-gas-on.cpp` | `eventGasOn__7Vehicle` | 8 |
| `sources/w8-camera-vehicle-b-vehicle-event-steering-change/vehicle-event-steering-change.cpp` | `eventSteeringChange__7Vehicle` | 8 |
| `sources/w8-camera-vehicle-b-vehicle-msg-collision/msg_collision.cpp` | `msg_collision__7VehicleP10GameObject` | 48 |
| `sources/w8-camera-vehicle-b-vehicle-msg-draw/msg_draw.cpp` | `msg_draw__7Vehicle` | 532 |
| `sources/w8-camera-vehicle-b-vehicle-msg-get-poke-name/msg_get_poke_name.cpp` | `msg_getPokeName__7VehiclePUsi` | 228 |
| `sources/w8-camera-vehicle-b-vehicle-msg-poke/msg_poke.cpp` | `msg_poke__7Vehiclei` | 8 |
| `sources/w8-camera-vehicle-b-vehicle-msg-run/msg_run.cpp` | `msg_run__7Vehicle` | 1740 |
| `sources/w8-camera-vehicle-b-vehicle-refresh-settings/refresh_settings.cpp` | `refreshSettings__7Vehicle` | 204 |

Import names are the ELF symbol names, defined as `extern "C"`.

## Observed

- The eight `Vehicle` event defaults (`0x3165a8`..`0x3165e0`, 8 bytes each) are
  a bare return. Inferred, not observed: they are the do-nothing defaults.
  `eventCollisionGround__11LavaTractor` (`0x284e08`) is also a bare return.
- LavaTractor events play sound sequences from the lump `tractor.lmp` with a
  random 1..3 variant; the file comments give the strings and addresses. The
  collision-wall event fires only when a dot product is below the float word
  `0xc02967e8`.
- `refreshSettings__7Vehicle` calls vtable slot 28 (`set`) and derives mass,
  inverse mass and three inertia terms and their inverses, then calls
  `buildCollisionInfo__7Vehicle`.
- `set__11LavaTractor` and `set__8SkulBoat` store constants, call
  `setMassAndIBody__7Vehicle`, and (LavaTractor only) `addWheel__7VehiclePciffff`
  four times.
- `msg_draw__7Vehicle` draws the body then up to four wheels; `msg_run`'s full
  step list is in its source header comment.

## Conventions the sources rely on

- `modelDraw` and the `long` mask: the original passes an 8-byte value in one
  register, so it is typed `unsigned long long`, while the 12-byte `Point3` by
  value goes by reference in `a3`; the EE compiler reproduced both conventions.
- Soft-double helpers `fptodp`, `dpcmp`, `dpsub` are declared with raw 64-bit
  patterns so the compiler emits those exact calls and not libgcc ones.
- Where the object holds 32-bit pointers, the sources define
  `NORRATH_W8_HOST` hooks (`norrath_w8_host_resolve`) used only by host contracts.
- The strings in these units are private copies; the image shares them with
  other users at `0x475f58`, `0x475f68`, `0x475f80`, `0x475f90`,
  `0x476d30`..`0x476df0`.

## Build-plan entries needed

Append as `independently_authored_source`, profile `authored_cpp`; the
integrator assigns `input_order`, and also adds the same sha256 to `files` in
plan and snapshot and raises `default_authored_inputs` by 24. A private copy
of plan and snapshot with these 24 entries passed `--check-plan`
(1431 selected inputs, 2105 files).

```json
[
 {
  "source": "sources/w8-camera-vehicle-b-lava-event-activate/lava_event_activate.cpp",
  "sha256": "9d0cdfdba1ad7630aae436abee4bab0b0758b9e346c28fd5261f43073db9da55",
  "object_name": "w8cvb_lava_event_activate.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "eventActivate__11LavaTractor",
    "compiled_body_bytes": 148
   }
  ]
 },
 {
  "source": "sources/w8-camera-vehicle-b-lava-event-collision-ground/lava_event_collision_ground.cpp",
  "sha256": "79f98e202ff09ea6a0bc796a6d0402dba24251dd1b27b9c4f8fe2f5520b0b64b",
  "object_name": "w8cvb_lava_event_collision_ground.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "eventCollisionGround__11LavaTractor",
    "compiled_body_bytes": 8
   }
  ]
 },
 {
  "source": "sources/w8-camera-vehicle-b-lava-event-collision-wall/lava_event_collision_wall.cpp",
  "sha256": "f42537198ddeab0006e9f75bdd077d059d096f055c4e56b95242da1515f8589f",
  "object_name": "w8cvb_lava_event_collision_wall.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "eventCollisionWall__11LavaTractorP6Point3T1",
    "compiled_body_bytes": 236
   }
  ]
 },
 {
  "source": "sources/w8-camera-vehicle-b-lava-event-cur-rpm/lava_event_cur_rpm.cpp",
  "sha256": "e15fc341e50d720be535890119e72405be661504ed666e974c711d58b7be057f",
  "object_name": "w8cvb_lava_event_cur_rpm.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "eventCurRPM__11LavaTractor",
    "compiled_body_bytes": 88
   }
  ]
 },
 {
  "source": "sources/w8-camera-vehicle-b-lava-event-deactivate/lava_event_deactivate.cpp",
  "sha256": "2d2da957975fc77df67bb62288986700a80ba0e47cb2de7d0b1a5edd107d63cb",
  "object_name": "w8cvb_lava_event_deactivate.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "eventDeActivate__11LavaTractor",
    "compiled_body_bytes": 44
   }
  ]
 },
 {
  "source": "sources/w8-camera-vehicle-b-lava-event-gas-off/lava-event-gas-off.cpp",
  "sha256": "ce6c72ee6d7abc4a651d1ea20b2a4aceba1d5225f707491cf5238be88e739b9e",
  "object_name": "w8cvb_lava_event_gas_off.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "eventGasOff__11LavaTractor",
    "compiled_body_bytes": 156
   }
  ]
 },
 {
  "source": "sources/w8-camera-vehicle-b-lava-event-gas-on/lava-event-gas-on.cpp",
  "sha256": "69da7f3ce15331ed6dc65228cb3aa193c929a34b0bd3fbfcf5c14061dc13224a",
  "object_name": "w8cvb_lava_event_gas_on.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "eventGasOn__11LavaTractor",
    "compiled_body_bytes": 156
   }
  ]
 },
 {
  "source": "sources/w8-camera-vehicle-b-lava-event-steering-change/lava-event-steering-change.cpp",
  "sha256": "090bbc89c367acf358ab287e5f0df8a90fe4d94d988a3acdd828d35ee8f2b458",
  "object_name": "w8cvb_lava_event_steering_change.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "eventSteeringChange__11LavaTractor",
    "compiled_body_bytes": 156
   }
  ]
 },
 {
  "source": "sources/w8-camera-vehicle-b-lava-set/lava_set.cpp",
  "sha256": "d3639507878363b697beefe6dbe886f9a038a06ee42bd2717dd8aaf3edb54164",
  "object_name": "w8cvb_lava_set.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "set__11LavaTractor",
    "compiled_body_bytes": 596
   }
  ]
 },
 {
  "source": "sources/w8-camera-vehicle-b-skul-set/skul_set.cpp",
  "sha256": "b3151e238868645041ffff973d62e116e98ab11fe48d315d4d12caf968d16e4b",
  "object_name": "w8cvb_skul_set.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "set__8SkulBoat",
    "compiled_body_bytes": 316
   }
  ]
 },
 {
  "source": "sources/w8-camera-vehicle-b-vehicle-event-activate/vehicle-event-activate.cpp",
  "sha256": "02baa7a6b3d0886b5e84d1adb870cff296254472120f23c73a73785d62ef8a7c",
  "object_name": "w8cvb_vehicle_event_activate.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "eventActivate__7Vehicle",
    "compiled_body_bytes": 8
   }
  ]
 },
 {
  "source": "sources/w8-camera-vehicle-b-vehicle-event-collision-ground/vehicle-event-collision-ground.cpp",
  "sha256": "8f7482049de685a3f409bb2f6a125cc4d5ca00d017d6b2ab0657f4ee2c7d372d",
  "object_name": "w8cvb_vehicle_event_collision_ground.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "eventCollisionGround__7Vehicle",
    "compiled_body_bytes": 8
   }
  ]
 },
 {
  "source": "sources/w8-camera-vehicle-b-vehicle-event-collision-wall/vehicle-event-collision-wall.cpp",
  "sha256": "efea49bff04690b1c6685519dfe400525838751a2ae3364a46a1669e72d72fff",
  "object_name": "w8cvb_vehicle_event_collision_wall.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "eventCollisionWall__7VehicleP6Point3T1",
    "compiled_body_bytes": 8
   }
  ]
 },
 {
  "source": "sources/w8-camera-vehicle-b-vehicle-event-cur-rpm/vehicle-event-cur-rpm.cpp",
  "sha256": "0582d2831648e5bbd8e70cee852cd9f4def228db2ea75b10b1e7e53ad5511d10",
  "object_name": "w8cvb_vehicle_event_cur_rpm.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "eventCurRPM__7Vehicle",
    "compiled_body_bytes": 8
   }
  ]
 },
 {
  "source": "sources/w8-camera-vehicle-b-vehicle-event-deactivate/vehicle-event-deactivate.cpp",
  "sha256": "feb8e874d5bde655ffff0167e58cfd577683afaa2532a5ea81785b755f9da092",
  "object_name": "w8cvb_vehicle_event_deactivate.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "eventDeActivate__7Vehicle",
    "compiled_body_bytes": 8
   }
  ]
 },
 {
  "source": "sources/w8-camera-vehicle-b-vehicle-event-gas-off/vehicle-event-gas-off.cpp",
  "sha256": "d29660108e1f32c319af00dab33667ea7f3fb029a25eecdf0dddc0ecb4f67e1d",
  "object_name": "w8cvb_vehicle_event_gas_off.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "eventGasOff__7Vehicle",
    "compiled_body_bytes": 8
   }
  ]
 },
 {
  "source": "sources/w8-camera-vehicle-b-vehicle-event-gas-on/vehicle-event-gas-on.cpp",
  "sha256": "5062ea70705ceb360405e3b82c902bc05c708bd5bce44df227ac5b7554db8453",
  "object_name": "w8cvb_vehicle_event_gas_on.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "eventGasOn__7Vehicle",
    "compiled_body_bytes": 8
   }
  ]
 },
 {
  "source": "sources/w8-camera-vehicle-b-vehicle-event-steering-change/vehicle-event-steering-change.cpp",
  "sha256": "d72182a9a2a7cf815ca4d81fe0ae2ceaca15f7eb78a2331e1d86414847031b05",
  "object_name": "w8cvb_vehicle_event_steering_change.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "eventSteeringChange__7Vehicle",
    "compiled_body_bytes": 8
   }
  ]
 },
 {
  "source": "sources/w8-camera-vehicle-b-vehicle-msg-collision/msg_collision.cpp",
  "sha256": "274af66abf03290a12cf105f14c7849498bd267594b6867711a2eeaed1f04fed",
  "object_name": "w8cvb_vehicle_msg_collision.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "msg_collision__7VehicleP10GameObject",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "source": "sources/w8-camera-vehicle-b-vehicle-msg-draw/msg_draw.cpp",
  "sha256": "73d37a96d426ad23707c6cb7dcfd7913e8f937a844ad074933c31e8448e62d6d",
  "object_name": "w8cvb_vehicle_msg_draw.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "msg_draw__7Vehicle",
    "compiled_body_bytes": 532
   }
  ]
 },
 {
  "source": "sources/w8-camera-vehicle-b-vehicle-msg-get-poke-name/msg_get_poke_name.cpp",
  "sha256": "eb87d3ea1c10591eee4d631060b0e683187eda4bcf6ae62315423f0aee683562",
  "object_name": "w8cvb_vehicle_msg_get_poke_name.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "msg_getPokeName__7VehiclePUsi",
    "compiled_body_bytes": 228
   }
  ]
 },
 {
  "source": "sources/w8-camera-vehicle-b-vehicle-msg-poke/msg_poke.cpp",
  "sha256": "ec4f93e804449f8bad748d12450b553c68eec6a1432e0ee84228abc43a1a25b2",
  "object_name": "w8cvb_vehicle_msg_poke.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "msg_poke__7Vehiclei",
    "compiled_body_bytes": 8
   }
  ]
 },
 {
  "source": "sources/w8-camera-vehicle-b-vehicle-msg-run/msg_run.cpp",
  "sha256": "0bf0256d49a90ff0ea2882608d69bc94a4935c7639b058132b5f5ca1938d5b53",
  "object_name": "w8cvb_vehicle_msg_run.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "msg_run__7Vehicle",
    "compiled_body_bytes": 1740
   }
  ]
 },
 {
  "source": "sources/w8-camera-vehicle-b-vehicle-refresh-settings/refresh_settings.cpp",
  "sha256": "e60b144ee9bdc9348228bb1115a2e1fa7aabd2530523c13840d2c01a8a7aee72",
  "object_name": "w8cvb_vehicle_refresh_settings.o",
  "profile": "authored_cpp",
  "kind": "independently_authored_source",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "refreshSettings__7Vehicle",
    "compiled_body_bytes": 204
   }
  ]
 }
]
```

## Imports closed

`eventActivate__11LavaTractor`,
`eventCollisionGround__11LavaTractor`,
`eventCollisionWall__11LavaTractorP6Point3T1`,
`eventCurRPM__11LavaTractor`,
`eventDeActivate__11LavaTractor`,
`eventGasOff__11LavaTractor`,
`eventGasOn__11LavaTractor`,
`eventSteeringChange__11LavaTractor`,
`set__11LavaTractor`,
`set__8SkulBoat`,
`eventActivate__7Vehicle`,
`eventCollisionGround__7Vehicle`,
`eventCollisionWall__7VehicleP6Point3T1`,
`eventCurRPM__7Vehicle`,
`eventDeActivate__7Vehicle`,
`eventGasOff__7Vehicle`,
`eventGasOn__7Vehicle`,
`eventSteeringChange__7Vehicle`,
`msg_collision__7VehicleP10GameObject`,
`msg_draw__7Vehicle`,
`msg_getPokeName__7VehiclePUsi`,
`msg_poke__7Vehiclei`,
`msg_run__7Vehicle`,
`refreshSettings__7Vehicle`

## Imports opened

Callees this package calls and does not define (ELF names unless noted):

`setMassAndIBody__7Vehicle`, `addWheel__7VehiclePciffff`,
`buildCollisionInfo__7Vehicle`, `addPassenger__7Vehiclei`,
`haveRoom__7Vehicle`, `isPassenger__7Vehiclei`, `textGetString__FPc`,
`ustrcpy__FPUsT0`, `lumpFindResource__FPcT0`, `eRand__10GameObject`, `sprintf`,
`soundPlaySequence__FP15AnimationHeaderP10GameObjectf`,
`soundChangeSequenceVolume__Fif`, `soundMakePersistant__Fi`, `soundStopSound__Fi`,
`norrath_list_oneway_link`, `norrath_actor_view_4844a4`, `modelClippingOn`,
`modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi`,
`modelResetOptionalParams__Fv`, `matRot__FP8Matrix34is`,
`matTransform__FP6Point3P8Matrix33T0`, `objectIsOffGrid__FP10GameObject`,
`gameLineUpdatePosition__FP17gameLinePathState`, `hudSetInfoMessage__FPUsiic`,
`hudClearInfoMessage__Fv`, `hudInfoMessageShowing__Fv`, `gameButtonToIcon__Fi`,
`usprintf__FPUsT0ffffffff`, `startForces__7Vehicle`, `doAI__7Vehicle`,
`doSteering__7Vehicle`, `doBrakes__7Vehicle`, `doThrottle__7Vehicle`,
`doSuspension__7Vehicle`, `doDriveline__7Vehicle`, `doMisc__7Vehicle`,
`getNewRotAndPos__7VehicleP6Point3T1fii`, `checkWallCollision__7Vehicle`,
`backup__7Vehicle`, `finalizePosition__7Vehicle`, `checkVehicleCollision__7Vehicle`,
`removePassenger__7Vehiclei`, `netPlayerIsConnected__Fi`, `fptodp`, `dpcmp`,
`dpsub`, and the data symbols `localNmPlayersAlive` (`0x484f48`),
`gameControlPaused` (`0x483f1c`), `player` (`0x408600`, stride 23984),
`pad` (`0x34beb8`, stride 88). Several are likely already defined by other
packages under authored names; the integrator should map them.

## Authored but never executed

All 24 functions. Host contracts ran on the host compiler with mocks; the EE
objects were compiled and never linked into an executed program.

## Unknown

- The meaning of nearly every vehicle, wheel, player and pad field offset
  used here; no names are proposed.
- Return types of the dropped helper calls (`addWheel`, `soundPlaySequence`,
  `removePassenger`, ...), and `msg_run`/`msg_draw` return types (the image
  sets no result).
- The `msg_run` text buffer size (the frame allows up to 512 bytes; 200
  UTF-16 units used) and why the LavaTractor wall threshold is below -1.
- `msg_run` argument/branch fidelity was reconstructed by reading control flow
  only; no recorded execution exists.

## Private link check

A private root (copy of `experimental/` with the 24 entries above appended)
built with the reviewed toolchain: 1431 inputs compiled and linked once,
1131 function bodies, 1203 unresolved symbols, zero COMMON. None of the 24
names listed under "Imports closed" is in that unresolved list. The count
reflects this branch's base plus only this package, so it is not an
integration delta. The link was not executed.

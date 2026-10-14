# w14-sprite-collide: sprite and world Collide virtuals

Branch `agents/w14-sprite-collide`. One commit per function, each under `experimental/sources/w14-sprite-collide-*` with an authored
`collide.cpp` and a `host_check.cpp` (not a plan input). Addresses and sizes are from the pinned private `SLUS_205.65`. Nothing here ran
on the EE or as gameplay; compiled sizes come from the reviewed EE `authored_cpp` profile and are not byte-identity claims.

## How each function was checked

* Each unit compiles with the reviewed EE g++ and `authored_cpp` flags and links in a private partial relocatable link of just these 30 units (no
  undefined symbol other than the callees listed below).
* Each has a native host check (`c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp && ./a.out`, exit 0) that stubs the callees and checks
  call order, arguments, stored words and results.
* Each was compared with the image in a private paired run (a small R5900 interpreter outside git): original and freshly compiled
  authored function run from identical memory snapshots with identical stubbed callees; call logs (arguments, plus matrix contents where
  they are passed), return value and every changed byte outside the stack must be equal. Run counts are in the commit messages. Deliberately wrong
  variants (offsets, slot, argument order, branch sense, destructor flags/order, increments, early exit) fail in each case. The 19 return-0 stubs were
  compared against the image with three scenarios each.
* `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests pass.

## Units

| Function | Image address | Image bytes | Compiled bytes |
|---|---|---|---|
| `Collide__9VICSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide` | 0x010b2ed8 | 8 | 8 |
| `Collide__9VICSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide` | 0x010b2e98 | 60 | 36 |
| `Collide__20VIEffectVolumeSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide` | 0x0114e778 | 8 | 8 |
| `Collide__20VIEffectVolumeSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide` | 0x0114e770 | 8 | 8 |
| `Collide__13VIFloraSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide` | 0x010062b8 | 8 | 8 |
| `Collide__13VIFloraSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide` | 0x010062b0 | 8 | 8 |
| `Collide__13VIGroupSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide` | 0x0101cfa0 | 620 | 460 |
| `Collide__13VIGroupSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide` | 0x0101cd30 | 620 | 460 |
| `Collide__9VIHSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide` | 0x010609f8 | 568 | 492 |
| `Collide__9VIHSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide` | 0x010607c0 | 568 | 492 |
| `Collide__11VILODSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide` | 0x01076940 | 128 | 92 |
| `Collide__11VILODSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide` | 0x010768c0 | 128 | 92 |
| `Collide__12VINameSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide` | 0x010a1210 | 8 | 8 |
| `Collide__12VINameSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide` | 0x010a1208 | 8 | 8 |
| `Collide__16VIParticleSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide` | 0x010aa738 | 8 | 8 |
| `Collide__16VIParticleSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide` | 0x010aa730 | 8 | 8 |
| `Collide__12VIPointLightR12VICollSphereR13VISceneFilterR7VISceneR9VICollide` | 0x010c3ba8 | 8 | 8 |
| `Collide__13VIPointSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide` | 0x010cb998 | 8 | 8 |
| `Collide__13VIPointSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide` | 0x010cb990 | 8 | 8 |
| `Collide__14VISimpleSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide` | 0x01167098 | 28 | 12 |
| `Collide__14VISimpleSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide` | 0x01167078 | 28 | 12 |
| `Collide__12VISkinSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide` | 0x0116d540 | 8 | 8 |
| `Collide__12VISkinSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide` | 0x0116d538 | 8 | 8 |
| `Collide__13VISoundSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide` | 0x0117c2b8 | 8 | 8 |
| `Collide__13VISoundSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide` | 0x0117c2b0 | 8 | 8 |
| `Collide__8VISpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide` | 0x01008138 | 8 | 8 |
| `Collide__19VIStreamAudioSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide` | 0x010190e0 | 8 | 8 |
| `Collide__19VIStreamAudioSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide` | 0x010190d8 | 8 | 8 |
| `Collide__7VIWorldR8VIEnvRay` | 0x010fe8f0 | 112 | 52 |
| `Collide__7VIWorldR18VISceneCollRayVars` | 0x010fe6a0 | 128 | 72 |

## Observed, inferred, unknown

* Observed: 22 of the 30 are small collision virtuals. Nineteen return 0 without touching anything (the rest of this
  package is larger). `VISimpleSprite` tail-calls `CollideObj__9VICollide...` with the word at +68; `VICSprite` (sphere) returns 0 if the word at +4456
  is set and otherwise tail-calls the capsule overload with the address +4424.
* Observed: `VILODSprite` (+76 holds a sprite index), `VIGroupSprite` (count at +64, 32-byte entries from +68) and `VIHSprite` (pool at +96, record array
  at +104, transform array at +72) fetch child sprites with `Sprite__7VIScenei` and call the child's virtual through a vtable-like table: the word at
  child+60 points to a table of 8-byte entries (signed halfword adjustment, then function word). The sphere virtual is the entry at +32 and the ray virtual the entry at +40. The
  group and the HSprite set the collide record's world matrix with `SetWorld` for each child; the group stops at the first hit (result 1) and the HSprite
  keeps walking, storing the index of the last hit at filter+12, and returns -1 if `Process__9VIHSpriteiR7VIScene(S, -1, V)` is negative.
* Observed: `Collide__7VIWorld...` wrappers pass 0 when the word at +52 is nonzero and 0x40000000 when it is zero as the second argument of the
  integer-indexed traversal; the ray-vars version copies the word at +68 into the vars first and increments it afterwards.
* Inferred: the word at +52 chooses a traversal start (a node versus a root selector); the first-argument sprite stays as an opaque word in all names.
  Field names are not used in the source (byte offsets only).
* Unknown: the names of all fields and table slots; what filter+12 means for the caller; the true meaning of `Transform__10VIMatrix44RC7VIVect3fT1` arguments; the types behind
  the `VICollide` world matrix and shapes. `VISprite` has a sphere virtual (`Collide__8VISpriteR12VICollSphere...`, 8 bytes at 0x01008130) that was not in this package's list and is not authored here.

## Imports closed

Collide__11VILODSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide, Collide__11VILODSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide, Collide__12VINameSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide, Collide__12VINameSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide, Collide__12VIPointLightR12VICollSphereR13VISceneFilterR7VISceneR9VICollide, Collide__12VISkinSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide, Collide__12VISkinSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide, Collide__13VIFloraSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide, Collide__13VIFloraSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide, Collide__13VIGroupSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide, Collide__13VIGroupSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide, Collide__13VIPointSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide, Collide__13VIPointSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide, Collide__13VISoundSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide, Collide__13VISoundSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide, Collide__14VISimpleSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide, Collide__14VISimpleSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide, Collide__16VIParticleSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide, Collide__16VIParticleSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide, Collide__19VIStreamAudioSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide, Collide__19VIStreamAudioSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide, Collide__20VIEffectVolumeSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide, Collide__20VIEffectVolumeSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide, Collide__7VIWorldR18VISceneCollRayVars, Collide__7VIWorldR8VIEnvRay, Collide__8VISpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide, Collide__9VICSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide, Collide__9VICSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide, Collide__9VIHSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide, Collide__9VIHSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide.

## Imports opened

Callees left unresolved (none authored here; several are other packages' symbols and may already be defined in the shared selection):
Begin__Ct6VIPool1Z19VIHSpriteAttachment, CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44, CollideObj__9VICollideR12VICollSphereR9VICapsule, CollideObj__9VICollideR12VICollSpherei, CollideObj__9VICollideR9VICollRayi, Collide__7VIWorldiR18VISceneCollRayVars, Collide__7VIWorldiR8VIEnvRayR7VIVect3T3, End__Ct6VIPool1Z19VIHSpriteAttachment, GetWorld__C9VICollide, Mul__10VIMatrix44RC10VIMatrix44T1, Next__Ct6VIPool1Z19VIHSpriteAttachmenti, Process__9VIHSpriteiR7VIScene, SetWorld__9VICollideRC10VIMatrix44, Sprite__7VIScenei, _$_10VIMatrix44, sceVu0CopyMatrix.
`Collide__7VIWorldiR18VISceneCollRayVars`, `Collide__7VIWorldiR8VIEnvRayR7VIVect3T3`, `CollideObj__9VICollide...` (three overloads) and the `VIPool` iterator/`Process`/`CalcTransAboutCenter` names are
the substantive new dependencies; `Sprite__7VIScenei`, `Mul__10VIMatrix44RC10VIMatrix44T1`, `_$_10VIMatrix44`, `GetWorld__C9VICollide`, `SetWorld__9VICollideRC10VIMatrix44` and `sceVu0CopyMatrix` were not on the baseline unresolved list when measured (they are defined by other packages in the shared plan or may be absent from the baseline list), so check against the integrated list.

## Build-plan entries needed

All `authored_cpp`, empty header closure, `sha256` of the source at the last commit of this branch (`emitted_functions` recorded):

```json
{"emitted_functions":[{"compiled_body_bytes":8,"name":"Collide__9VICSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_csprite_collide_ray.o","profile":"authored_cpp","sha256":"5a19fa6cd5238916df446fda92da56177c2d88fa72d7b813bc24bafa3d76dd0a","source":"sources/w14-sprite-collide-csprite-collide-ray/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":36,"name":"Collide__9VICSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_csprite_collide_sphere.o","profile":"authored_cpp","sha256":"a5c6c32a8ed7b8d106da3eb20a70ae840631f0a5b565931b47e5f2b8019b7cb2","source":"sources/w14-sprite-collide-csprite-collide-sphere/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"Collide__20VIEffectVolumeSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_effect_volume_sprite_collide_ray.o","profile":"authored_cpp","sha256":"ffe189b91fe99b6f8dfe18d7e6d3b66a35a549139dcf19d8f20eb590b5de145e","source":"sources/w14-sprite-collide-effect-volume-sprite-collide-ray/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"Collide__20VIEffectVolumeSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_effect_volume_sprite_collide_sphere.o","profile":"authored_cpp","sha256":"e1c405f7ab09199acf547a0d65507ae7afe10212002f1243506a9dc1203cc1a0","source":"sources/w14-sprite-collide-effect-volume-sprite-collide-sphere/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"Collide__13VIFloraSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_flora_sprite_collide_ray.o","profile":"authored_cpp","sha256":"78fe223e7273bc20a8e1d486ee6a387a80b2c6e11422a1b8d51771e29d48e5fe","source":"sources/w14-sprite-collide-flora-sprite-collide-ray/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"Collide__13VIFloraSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_flora_sprite_collide_sphere.o","profile":"authored_cpp","sha256":"c5c74f931c67aba50dc4a88f8a325a850d2b6cf9856782f58386776dff9ce55c","source":"sources/w14-sprite-collide-flora-sprite-collide-sphere/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":460,"name":"Collide__13VIGroupSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_group_sprite_collide_ray.o","profile":"authored_cpp","sha256":"c64252dcf7cc64511e3266ea1366bcedec81d2bbc89b7e2ebc46993f487f5e72","source":"sources/w14-sprite-collide-group-sprite-collide-ray/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":460,"name":"Collide__13VIGroupSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_group_sprite_collide_sphere.o","profile":"authored_cpp","sha256":"bcc609272c70a02551638903369bc0a630edcdb82ac9aef9ff986e890bd35991","source":"sources/w14-sprite-collide-group-sprite-collide-sphere/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":492,"name":"Collide__9VIHSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_hsprite_collide_ray.o","profile":"authored_cpp","sha256":"41cfbd0ef42f8f4a7780ae7551cf6aa9f1dfb3e9f0da1a40140a00b29aafaa97","source":"sources/w14-sprite-collide-hsprite-collide-ray/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":492,"name":"Collide__9VIHSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_hsprite_collide_sphere.o","profile":"authored_cpp","sha256":"56b06db67cd59b401983288e8d093fa4eb11b4e87376a386c32dadb9f6d16450","source":"sources/w14-sprite-collide-hsprite-collide-sphere/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":92,"name":"Collide__11VILODSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_lod_sprite_collide_ray.o","profile":"authored_cpp","sha256":"c0b2a26a680594e8cc8a8afbebd31c0bd81f19dd150103eed8c176c208b68609","source":"sources/w14-sprite-collide-lod-sprite-collide-ray/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":92,"name":"Collide__11VILODSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_lod_sprite_collide_sphere.o","profile":"authored_cpp","sha256":"8d6539e5c6017dd79233a5588ccaea0c251ef1a390585708014bcba2080a95cf","source":"sources/w14-sprite-collide-lod-sprite-collide-sphere/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"Collide__12VINameSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_name_sprite_collide_ray.o","profile":"authored_cpp","sha256":"351a394ea6681b1614de216cdf433147a53ce9cb7b0f26a05d1b84c7f772a887","source":"sources/w14-sprite-collide-name-sprite-collide-ray/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"Collide__12VINameSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_name_sprite_collide_sphere.o","profile":"authored_cpp","sha256":"bb07f12e93e6eeee1450a01d4a41102d381ee9f131f5bfdce50a86a58ee7f260","source":"sources/w14-sprite-collide-name-sprite-collide-sphere/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"Collide__16VIParticleSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_particle_sprite_collide_ray.o","profile":"authored_cpp","sha256":"ce4afc110eb616bf133848254ca81b6ae559fb24331430be8603846d68f53bc6","source":"sources/w14-sprite-collide-particle-sprite-collide-ray/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"Collide__16VIParticleSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_particle_sprite_collide_sphere.o","profile":"authored_cpp","sha256":"3d656fc805f3b1f6619e38855bde221f2170d6eb31cec260847664b349c383a7","source":"sources/w14-sprite-collide-particle-sprite-collide-sphere/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"Collide__12VIPointLightR12VICollSphereR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_point_light_collide_sphere.o","profile":"authored_cpp","sha256":"a3399ed1672cc5420cef9fd9cd9fd6451a17098644e47ced01ed2675820fe911","source":"sources/w14-sprite-collide-point-light-collide-sphere/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"Collide__13VIPointSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_point_sprite_collide_ray.o","profile":"authored_cpp","sha256":"15f9cca38cca06f35a51ac7496bcd662571559095643c416587fe898830f2f28","source":"sources/w14-sprite-collide-point-sprite-collide-ray/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"Collide__13VIPointSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_point_sprite_collide_sphere.o","profile":"authored_cpp","sha256":"5fb4143c3ac8564399f2baef55c48977064372e608ee68c26e6262d3fe4edacf","source":"sources/w14-sprite-collide-point-sprite-collide-sphere/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":12,"name":"Collide__14VISimpleSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_simple_sprite_collide_ray.o","profile":"authored_cpp","sha256":"fbf7c52b7943046d05a54ceca5fe37940cac712b4101d2a38fc1d49fb9ab37a5","source":"sources/w14-sprite-collide-simple-sprite-collide-ray/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":12,"name":"Collide__14VISimpleSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_simple_sprite_collide_sphere.o","profile":"authored_cpp","sha256":"d518ddb8fb47f777567adaea6a7077fb0c7a1594d875f368f02cb6b308a45497","source":"sources/w14-sprite-collide-simple-sprite-collide-sphere/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"Collide__12VISkinSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_skin_sprite_collide_ray.o","profile":"authored_cpp","sha256":"a877b6b5d7aff97bf929860240751a4780557e202f2a13dbe6af868540db2dce","source":"sources/w14-sprite-collide-skin-sprite-collide-ray/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"Collide__12VISkinSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_skin_sprite_collide_sphere.o","profile":"authored_cpp","sha256":"f56da241a56bd07cca32a0339ee013a1815524574f38115a09efff9d3e73037d","source":"sources/w14-sprite-collide-skin-sprite-collide-sphere/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"Collide__13VISoundSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_sound_sprite_collide_ray.o","profile":"authored_cpp","sha256":"bba43d28139c0d94abb5e66565bf4ca5fc3f6e471cf3dbb5b014e2d82b9b6e97","source":"sources/w14-sprite-collide-sound-sprite-collide-ray/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"Collide__13VISoundSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_sound_sprite_collide_sphere.o","profile":"authored_cpp","sha256":"d3902f1094aaea109d46a4f95bd13a8e1f8ed52339aa2f6c8eafee33af39f146","source":"sources/w14-sprite-collide-sound-sprite-collide-sphere/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"Collide__8VISpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_sprite_collide_ray.o","profile":"authored_cpp","sha256":"09d587b311d32d5eb2c2f8179fbf4e9af8a3fb0aa4c300567e5551e512a601d5","source":"sources/w14-sprite-collide-sprite-collide-ray/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"Collide__19VIStreamAudioSpriteR9VICollRayR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_stream_audio_sprite_collide_ray.o","profile":"authored_cpp","sha256":"06e6cd85da1836efb59bb023b536e7debe8b3eef8ddc5bef1291b207b918c5f5","source":"sources/w14-sprite-collide-stream-audio-sprite-collide-ray/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"Collide__19VIStreamAudioSpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_stream_audio_sprite_collide_sphere.o","profile":"authored_cpp","sha256":"536fe928165f8a5f601dec7acf2a2b6b7d2ace4b5839573baf59d7e2805183df","source":"sources/w14-sprite-collide-stream-audio-sprite-collide-sphere/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":52,"name":"Collide__7VIWorldR8VIEnvRay"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_world_collide_env_ray.o","profile":"authored_cpp","sha256":"6f359b87c6e49380b33e27d4070ab2001ffddace41873db76d52bf4eb6e80d92","source":"sources/w14-sprite-collide-world-collide-env-ray/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":72,"name":"Collide__7VIWorldR18VISceneCollRayVars"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_collide_world_collide_ray_vars.o","profile":"authored_cpp","sha256":"688d58c29480c196a62689474478d25c68a4ec7cd3c85f51e4eb694ff345c576","source":"sources/w14-sprite-collide-world-collide-ray-vars/collide.cpp"}
```

## Authored but never executed

All 30 functions. They ran only in the host checks and in private interpreter pairs against the image with stubbed callees; none ran
in a real game or on hardware. The vtable-table reads, the child virtual calls and the matrix state are modelled with stubs.

## Unknown / not done

* The real classes, callers and tables. The callees named above are not verified here.
* `Collide__8VISpriteR12VICollSphere...` (not in scope).

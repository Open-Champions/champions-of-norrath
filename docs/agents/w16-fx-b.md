# w16-fx-b: effect volumes, particles, trails and hierarchical sprites (part 2)

Evidence base: the pinned SLUS-20565 image (private, never committed). Every function below was read from that image, rewritten
independently, compiled with the reviewed EE toolchain, and run against the original in a private R5900 paired-run harness
(identical memory snapshots, identical stub callees, comparing call logs including argument memory, returns and every changed
byte outside the stack). Each unit also has a `host_check.cpp` that is not a build input. Commits are one function each.

## Scope check
The assignment listed 24 imports. Seven (`addEffect__FiG6Point3`, `drawDistortionParticles__Fv`, `drawPS__FR6PacketP14ParticleSystem`,
`drawSingleParticles__Fi`, `drawTrails__Fv`, `frameNumWrappedPS__FP14ParticleSystem`, `lightInit__11AddParticlePPc`) were already
defined in the shared branch by earlier waves, so they were not redone. The other 17 are authored here.

## Authored functions (observed, from the image)
| Function | Address | Size | Source directory |
|---|---|---|---|
| SetZWrite__20VIParticleDefinitionb | 0x010b8940 | 64 | w16-fx-b-set-zwrite |
| SetZTest__20VIParticleDefinitionb | 0x010b8980 | 64 | w16-fx-b-set-ztest |
| SetTextureConfiguration__20VIParticleDefinitioni | 0x010b8a28 | 8 | w16-fx-b-set-texture-configuration |
| SetBlendMode__20VIParticleDefinitionQ222VIParticleDefinitionEx9BLENDMODE | 0x010b8870 | 204 | w16-fx-b-set-blend-mode |
| SetTexture__20VIParticleDefinitioniR8VIRasterR12VIDictionary | 0x010b89c8 | 96 | w16-fx-b-set-texture |
| SetInnerOffset / SetInnerHprVar / SetOuterOffset / SetOuterHprVar (VIParticleAttributes) | 0x010b4800 / 0x010b4868 / 0x010b48d0 / 0x010b4938 | 96 each | w16-fx-b-set-inner-offset, -inner-hpr-var, -outer-offset, -outer-hpr-var |
| VITrailFxBegin__Flif | 0x01027090 | 128 | w16-fx-b-trail-fx-begin |
| __13VIHSpriteAnim | 0x0105cec8 | 36 | w16-fx-b-hsprite-anim-ctor |
| __13VISpellEffect | 0x01000778 | 64 | w16-fx-b-spell-effect-ctor |
| __22VIParticleDefinitionEx | 0x010b4bc8 | 64 | w16-fx-b-particle-definition-ex-ctor |
| SetName__15VIParticleMotifPCc | 0x010b4b80 | 64 | w16-fx-b-motif-set-name |
| SetNodeFixScale__9VIHSpriteiRC7VIVect3 | 0x010636a8 | 48 | w16-fx-b-hsprite-set-node-fix-scale |
| UpdateCenterTransform__9VIHSpriteR7VIScene | 0x010618c8 | 568 | w16-fx-b-hsprite-update-center-transform |
| SetHair__9VICSpriteiiiR7VIScene | 0x010aed40 | 936 | w16-fx-b-csprite-set-hair |

Layout facts established (observed from the stores and loads, not named): VIParticleDefinitionEx has its blend mode at +748, z-write
at +752, z-test at +756, texture configuration at +760, a material key pair at +772/+776 and a texture id at +780; its constructor
runs the attributes constructor, a motif pool constructor at +724, and defaults 0, 0, 1, 1 for +748/+752/+756/+760.
VIParticleAttributes holds four 3-float vectors at +612, +624, +636, +648 with nonzero flags at +684, +688, +692, +696 and a
counter at +716 that every setter increments. VIParticleMotif keeps the name text at +724 and its hash at +756.

## Method notes
- Paired runs compared against the original, with instruction coverage of the original: all units reached full coverage
  except padding words (SetBlendMode one jump-table nop, SetHair two alignment nops); UpdateCenterTransform's negative-result
  path is covered by its own scenarios (each run only covers its path).
- UpdateCenterTransform's original zeroes its two local matrices with VU0 `vsub`/`sqc2` macro instructions; the private
  interpreter was extended (privately) to emulate exactly that pair. Authored code uses a plain float loop. The compiled authored
  code is therefore smaller than the original (408 vs 568 bytes) and not a byte match; behaviour matches in the paired runs.
- Mutation checks: swapped Mul operands in UpdateCenterTransform (12/24 scenarios differ) and two altered conditions in SetHair
  (12 and 98 of 600 differ) are all detected.
- A host-check slip: the first UpdateCenterTransform commit was made before its host check was fixed; the next commit repairs the
  check only. The source did not change.
- No unit is claimed as a byte match; none of this was compared as an assembled bytes match, only behaviourally.

## Build-plan entries needed (append to `inputs`, adjust `input_order` to the integrator's sequence)
Each uses the existing `authored_cpp` profile and no headers. SHA-256 values are of the committed files.
```json
{"kind": "independently_authored_source", "profile": "authored_cpp", "header_closure": [], "source": "sources/w16-fx-b-csprite-set-hair/set_hair.cpp", "sha256": "5da83bd8be708946560f362de4acee0760322961a3c0fbf1df194dd65a96097c", "object_name": "w16_fx_b_csprite_set_hair_set_hair.o", "emitted_functions": ["SetHair__9VICSpriteiiiR7VIScene"]}
{"kind": "independently_authored_source", "profile": "authored_cpp", "header_closure": [], "source": "sources/w16-fx-b-hsprite-anim-ctor/ctor.cpp", "sha256": "0891b88b6663cfd7c454912cdc9ce760888e20717b4e001fec77dc5057b30045", "object_name": "w16_fx_b_hsprite_anim_ctor_ctor.o", "emitted_functions": ["__13VIHSpriteAnim"]}
{"kind": "independently_authored_source", "profile": "authored_cpp", "header_closure": [], "source": "sources/w16-fx-b-hsprite-set-node-fix-scale/set_node_fix_scale.cpp", "sha256": "931561a6b0447a55879ed91970c66c8e1dbf6a3855d1dedb1048efb3b62c0627", "object_name": "w16_fx_b_hsprite_set_node_fix_scale_set_node_fix_scale.o", "emitted_functions": ["SetNodeFixScale__9VIHSpriteiRC7VIVect3"]}
{"kind": "independently_authored_source", "profile": "authored_cpp", "header_closure": [], "source": "sources/w16-fx-b-hsprite-update-center-transform/update_center_transform.cpp", "sha256": "f1fa91159cae15f42ba0419271b81c8682dca9915e99e2dbe0cbb6259fec6a3e", "object_name": "w16_fx_b_hsprite_update_center_transform_update_center_transform.o", "emitted_functions": ["UpdateCenterTransform__9VIHSpriteR7VIScene"]}
{"kind": "independently_authored_source", "profile": "authored_cpp", "header_closure": [], "source": "sources/w16-fx-b-motif-set-name/set_name.cpp", "sha256": "8c84528a873721b30e9fe0a3946defbaba78651059b1b4e97e6b363ae785f353", "object_name": "w16_fx_b_motif_set_name_set_name.o", "emitted_functions": ["SetName__15VIParticleMotifPCc"]}
{"kind": "independently_authored_source", "profile": "authored_cpp", "header_closure": [], "source": "sources/w16-fx-b-particle-definition-ex-ctor/ctor.cpp", "sha256": "3a94e7d9816ed7cc5e6e712f933d88930162a5eb496904e0c127f4b3f94be7a9", "object_name": "w16_fx_b_particle_definition_ex_ctor_ctor.o", "emitted_functions": ["__22VIParticleDefinitionEx"]}
{"kind": "independently_authored_source", "profile": "authored_cpp", "header_closure": [], "source": "sources/w16-fx-b-set-blend-mode/set_blend_mode.cpp", "sha256": "1c220e85d7e58702f2a496da8664458889929b21b1afa1c5ad4e411a9594bb4d", "object_name": "w16_fx_b_set_blend_mode_set_blend_mode.o", "emitted_functions": ["SetBlendMode__20VIParticleDefinitionQ222VIParticleDefinitionEx9BLENDMODE"]}
{"kind": "independently_authored_source", "profile": "authored_cpp", "header_closure": [], "source": "sources/w16-fx-b-set-inner-hpr-var/set_inner_hpr_var.cpp", "sha256": "7d20c03680629c1149430ad5042b817365a4ad39a4270b0de9b23385934f9f2e", "object_name": "w16_fx_b_set_inner_hpr_var_set_inner_hpr_var.o", "emitted_functions": ["SetInnerHprVar__20VIParticleAttributesRC7VIVect3"]}
{"kind": "independently_authored_source", "profile": "authored_cpp", "header_closure": [], "source": "sources/w16-fx-b-set-inner-offset/set_inner_offset.cpp", "sha256": "e9c1f0a291d00a763d3a6fd8c587ac5ff9885dccc9cabeccb75b0d946f0a78b3", "object_name": "w16_fx_b_set_inner_offset_set_inner_offset.o", "emitted_functions": ["SetInnerOffset__20VIParticleAttributesRC7VIVect3"]}
{"kind": "independently_authored_source", "profile": "authored_cpp", "header_closure": [], "source": "sources/w16-fx-b-set-outer-hpr-var/set_outer_hpr_var.cpp", "sha256": "b7e2fc62ed3721973ace9a1772cd630dae898f5f53864f737193d0576950bf9f", "object_name": "w16_fx_b_set_outer_hpr_var_set_outer_hpr_var.o", "emitted_functions": ["SetOuterHprVar__20VIParticleAttributesRC7VIVect3"]}
{"kind": "independently_authored_source", "profile": "authored_cpp", "header_closure": [], "source": "sources/w16-fx-b-set-outer-offset/set_outer_offset.cpp", "sha256": "be3e0cffdfa8d56d97fbf46cdaeb149f8212af937ee27b25cddfb366d098b28d", "object_name": "w16_fx_b_set_outer_offset_set_outer_offset.o", "emitted_functions": ["SetOuterOffset__20VIParticleAttributesRC7VIVect3"]}
{"kind": "independently_authored_source", "profile": "authored_cpp", "header_closure": [], "source": "sources/w16-fx-b-set-texture/set_texture.cpp", "sha256": "400fca8edd5d7d09d6b7c7fc9ff5828fcd91f41470a459a129d848dfe62a1db9", "object_name": "w16_fx_b_set_texture_set_texture.o", "emitted_functions": ["SetTexture__20VIParticleDefinitioniR8VIRasterR12VIDictionary"]}
{"kind": "independently_authored_source", "profile": "authored_cpp", "header_closure": [], "source": "sources/w16-fx-b-set-texture-configuration/set_texture_configuration.cpp", "sha256": "997f1d492b96e71969e6cd144540225ff66cbeb071a05c6845c62185b3399e92", "object_name": "w16_fx_b_set_texture_configuration_set_texture_configuration.o", "emitted_functions": ["SetTextureConfiguration__20VIParticleDefinitioni"]}
{"kind": "independently_authored_source", "profile": "authored_cpp", "header_closure": [], "source": "sources/w16-fx-b-set-ztest/set_ztest.cpp", "sha256": "4a01bb34e5ea7d69e1049c12e865e2ed1b5ad34a761dab4e3a484ca093753cff", "object_name": "w16_fx_b_set_ztest_set_ztest.o", "emitted_functions": ["SetZTest__20VIParticleDefinitionb"]}
{"kind": "independently_authored_source", "profile": "authored_cpp", "header_closure": [], "source": "sources/w16-fx-b-set-zwrite/set_zwrite.cpp", "sha256": "184a6efcd39750e4bb09a8d62f03d131727b3c8c9d9ff5cbaa641b1a0d96f702", "object_name": "w16_fx_b_set_zwrite_set_zwrite.o", "emitted_functions": ["SetZWrite__20VIParticleDefinitionb"]}
{"kind": "independently_authored_source", "profile": "authored_cpp", "header_closure": [], "source": "sources/w16-fx-b-spell-effect-ctor/ctor.cpp", "sha256": "34c0f3bf2482ad4168f729df379965aa68e3f977d239abf6f09ed779845ae0d6", "object_name": "w16_fx_b_spell_effect_ctor_ctor.o", "emitted_functions": ["__13VISpellEffect"]}
{"kind": "independently_authored_source", "profile": "authored_cpp", "header_closure": [], "source": "sources/w16-fx-b-trail-fx-begin/trail_fx_begin.cpp", "sha256": "693eed77b0d934f49d22b082f3a8db0a45751fc151132de3271891b574152552", "object_name": "w16_fx_b_trail_fx_begin_trail_fx_begin.o", "emitted_functions": ["VITrailFxBegin__Flif"]}
```

## Final summary
**Imports closed (17):** SetBlendMode__20VIParticleDefinitionQ222VIParticleDefinitionEx9BLENDMODE, SetHair__9VICSpriteiiiR7VIScene,
SetInnerHprVar__20VIParticleAttributesRC7VIVect3, SetInnerOffset__20VIParticleAttributesRC7VIVect3, SetName__15VIParticleMotifPCc,
SetNodeFixScale__9VIHSpriteiRC7VIVect3, SetOuterHprVar__20VIParticleAttributesRC7VIVect3, SetOuterOffset__20VIParticleAttributesRC7VIVect3,
SetTextureConfiguration__20VIParticleDefinitioni, SetTexture__20VIParticleDefinitioniR8VIRasterR12VIDictionary,
SetZTest__20VIParticleDefinitionb, SetZWrite__20VIParticleDefinitionb, UpdateCenterTransform__9VIHSpriteR7VIScene, VITrailFxBegin__Flif,
__13VIHSpriteAnim, __13VISpellEffect, __22VIParticleDefinitionEx. Already closed before this package (not redone): addEffect__FiG6Point3,
drawDistortionParticles__Fv, drawPS__FR6PacketP14ParticleSystem, drawSingleParticles__Fi, drawTrails__Fv,
frameNumWrappedPS__FP14ParticleSystem, lightInit__11AddParticlePPc.

**Imports opened (referenced from my units, not defined here; some may already be defined elsewhere in the shared branch):**
AddTail__7VILListP7VILNode, Clear__13VIHSpriteAnim, GetHairTexture__13VICSpriteCusti, GetMaterialPalEntry__8VIRasterii, GetTime__7VIScene, Init__9VITrailFxlif, InterpTransAboutCenter__9VIHSpriteR7VISceneR7VIVect3R14VIHSpriteFrame, Material__8VIRasteri, Mul__10VIMatrix44RC10VIMatrix44T1, RemHead__7VILList, Scale__10VIMatrix44f, SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode, SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary, SetZTest__10VIMateriali, SetZWrite__10VIMateriali, Transform__10VIMatrix44RC6VIQuatfRC7VIVect3, Translate__10VIMatrix44RC7VIVect3, VIHashResourceID, VITrailFx$sActiveTrailFx, VITrailFx$sFreeTrailFx, _$_10VIMatrix44, _13VICSpriteCust$HairNull, __20VIParticleAttributes, __t6VIPool1ZP15VIParticleMotif, strcpy.
The data symbols among them are `VITrailFx$sFreeTrailFx` (0x011e51e8) and `VITrailFx$sActiveTrailFx` (0x011e51f8), spelled as in the
existing trail sources, and `_13VICSpriteCust$HairNull` (0x01228a00), spelled as in the image symbol table.

**Authored but never executed:** nothing ran in the game. All behaviour evidence is private paired runs with stubbed callees and
host checks with stubbed callees; the real callees (matrix maths, material setters, list helpers, hair texture lookup,
interpolation) were not run together with these functions.

**Unknown:** the names of the VIParticleDefinitionEx blend modes; the meaning of SetHair's three selectors and of the word at
this+4420 (treated as a flag that is -1 when unset) and of this+416; the meaning of this+208 and the time stamp at this+192 in
the hierarchical sprite beyond what CalcTransAboutCenter shows; the matrix multiplication convention behind Mul's operand order
(only the observed order is reproduced); the capacity of the motif name buffer at +724 (SetName does not check length); the width
semantics of the stored z-write/z-test flags; whether SetNodeFixScale's zero return is meaningful.

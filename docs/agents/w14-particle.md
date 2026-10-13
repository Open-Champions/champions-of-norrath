# w14-particle

Package 1417: particle attributes, emitters and definitions. Branch `agents/w14-particle`; sources are under `experimental/sources/w14-particle-*`, one function per directory and per commit. Each directory holds the authored `.cpp` and a `host_contract.cpp` (host-only, not a plan input).

## Method and evidence

Addresses and extents come from the pinned SLUS-20565 symbol table. Every function was compiled with the reviewed EE g++ and the `authored_cpp` flags, then run side by side with the original function in a private R5900 interpreter harness kept outside git. Both runs start from identical memory snapshots with identical scripted callee stubs. The comparison covers the return value (or the float return register), every changed byte outside the stack, and the ordered call log with argument contents. `halt` results were treated as failures. Source mutations (changing an offset, a constant, a shift, a loop bound, a flag test, a shift of 724 to 720) were each caught. A native host contract per function additionally checks the behaviour. Fresh EE objects are not claimed to be byte matches of the original; the compiled sizes differ for the larger functions (for example `Update__17VIParticleEmitter` compiles to 2,428 bytes against the original 2,856).

## Observed (per function)

Setters on the attribute block (`VIParticleAttributes`): each of `SetFriction`(+0), `SetBirthrate`(+4), `SetBirthrateVar`(+8), `SetLifespan`(+12), `SetLifespanVar`(+16), `SetVelocity`(+20), `SetVelocityVar`(+24), `SetStartSize`(+40), `SetStartSizeVar`(+44), `SetEndSize`(+48), `SetEndSizeVar`(+52), `SetInheritVelocity`(+600), `SetDeltaSpawn`(+604), `SetGradientRepeat`(+608) stores its float argument at the stated offset and adds one to the word at +716. `SetStartColorVar`(+568) and `SetEndColorVar`(+584) copy 16 bytes the same way. `SetGradientColor` copies 16 bytes to +56+16*index with no bounds check. `SetGravityOn` stores -5.0f or 0.0f at +720. `SetNozzleAxis` copies three floats to +660 and calls `Normalize__7VIVect3` on them. `SetNozzleHprVar` copies three floats to +672 and stores at +700 whether any of them is non-zero. Every one of them bumps the counter at +716.

Emitter and system accessors: `DeltaTime__C16VIParticleSystem` returns the float at +16. `ParticleDefinition__16VIParticleSystemi` indexes 12-byte records from the pointer at +92. `ParticleCount__C17VIParticleEmitter` returns the word at +172. `SetLocation__17VIParticleEmitterRC7VIVect3` moves the triple at +16 to +28, stores the new one at +16 and, only the first time (flag at +980), also overwrites +28. `UnlinkAllMotifBlenders__17VIParticleEmitter` pops the list at +176 and zeroes the word at +16 of each node.

Motif pool users: `FirstMotif__C22VIParticleDefinitionEx` and `DestroyMotif__22VIParticleDefinitionExP15VIParticleMotif` walk the pool at definition +724 with the `VIPool<VIParticleMotif*>` template members. `_$_14VIMotifBlender` and `_$_22VIParticleDefinitionEx` are destructors (flag bit 0 selects `__builtin_delete`; the second one first destroys the pool with flag 2).

`P_SetTrailWidth__Fii` writes `width >> 2` to byte 1039 of `trail[(id & 0xff)]` when the halfword at 1024 equals the whole id and id is non-zero.

`Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition` resets the emitter, links it into the system list at +52 and copies the 724-byte attribute block of the definition to emitter +188. `Update__17VIParticleEmitter` spawns particles from the system's free list at +64 using that copied block; the header comment of its source lists every field it reads and writes.

## Inferred

The emitter offsets (+188 attribute copy, +212 velocity, +848 nozzle axis, and so on) follow the setter offsets plus 188. Inner/outer offset and hpr-variance triples at attribute +612/+624/+636/+648 with flags +684/+688/+692/+696 are inferred from the setters' flag stores. The particle record offsets written by Update (+20 inverse lifespan, +24 friction, +28 gradient repeat, +32 position, +44 velocity, +88 start size, +92 size rate, +96 gravity, +100 frame) are named from the values stored; they are not an established layout.

## Build-plan entries needed

Append these to `inputs` of `experimental/build-plan.json` (and the matching `files`/snapshot records) in this order. The `sha256` is of the `.cpp` file. `input_order` is for the integrator to assign.

```json
[
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_attr_set_birthrate.o",
  "profile": "authored_cpp",
  "sha256": "7aaaf26ed014a22e7521eff9e50f458b3e1c59832efdad7a4217953e485b8d3e",
  "source": "sources/w14-particle-attr-set-birthrate/set_birthrate.cpp",
  "emitted_functions": [
   {
    "name": "SetBirthrate__20VIParticleAttributesf",
    "compiled_body_bytes": 20
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_attr_set_birthrate_var.o",
  "profile": "authored_cpp",
  "sha256": "8b6e75d58d702ea2dabb7158a704666adc04d91b91de73fc0702d067a0967cac",
  "source": "sources/w14-particle-attr-set-birthrate-var/set_birthrate_var.cpp",
  "emitted_functions": [
   {
    "name": "SetBirthrateVar__20VIParticleAttributesf",
    "compiled_body_bytes": 20
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_attr_set_delta_spawn.o",
  "profile": "authored_cpp",
  "sha256": "0513349ed286c3ed9d5373455db08de8716357283b339776f3098fcbf107ab32",
  "source": "sources/w14-particle-attr-set-delta-spawn/set_delta_spawn.cpp",
  "emitted_functions": [
   {
    "name": "SetDeltaSpawn__20VIParticleAttributesf",
    "compiled_body_bytes": 20
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_attr_set_end_color_var.o",
  "profile": "authored_cpp",
  "sha256": "1627797485bf761f6a04df48d0be30f5dcc7b981f1c4bcf1477b7e43a57eed66",
  "source": "sources/w14-particle-attr-set-end-color-var/set_end_color_var.cpp",
  "emitted_functions": [
   {
    "name": "SetEndColorVar__20VIParticleAttributesRC10VIColor32F",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_attr_set_end_size.o",
  "profile": "authored_cpp",
  "sha256": "f91e23aa43c698f783b968e815970ca24d5880a2a4a99a95da126d0f06ffc60a",
  "source": "sources/w14-particle-attr-set-end-size/set_end_size.cpp",
  "emitted_functions": [
   {
    "name": "SetEndSize__20VIParticleAttributesf",
    "compiled_body_bytes": 20
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_attr_set_end_size_var.o",
  "profile": "authored_cpp",
  "sha256": "ade84d1bd4db05453c115370f4c5dbb18e92779ed4486aa8565154000b5c7c8f",
  "source": "sources/w14-particle-attr-set-end-size-var/set_end_size_var.cpp",
  "emitted_functions": [
   {
    "name": "SetEndSizeVar__20VIParticleAttributesf",
    "compiled_body_bytes": 20
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_attr_set_friction.o",
  "profile": "authored_cpp",
  "sha256": "d42ce7711178dd0b784b27885fc27c6cd157c01d7c86d8a9db87858c28a29649",
  "source": "sources/w14-particle-attr-set-friction/set_friction.cpp",
  "emitted_functions": [
   {
    "name": "SetFriction__20VIParticleAttributesf",
    "compiled_body_bytes": 20
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_attr_set_gradient_color.o",
  "profile": "authored_cpp",
  "sha256": "439a30a83d4a46e0b6deda83a02a040f8553663b30c8f3412fdcab4f8eebf97d",
  "source": "sources/w14-particle-attr-set-gradient-color/set_gradient_color.cpp",
  "emitted_functions": [
   {
    "name": "SetGradientColor__20VIParticleAttributesiRC10VIColor32F",
    "compiled_body_bytes": 60
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_attr_set_gradient_repeat.o",
  "profile": "authored_cpp",
  "sha256": "d4d8b9cd4cf0d3eb98dc633407ed66747aada70b44b6fa5d550b4f53385cfbe3",
  "source": "sources/w14-particle-attr-set-gradient-repeat/set_gradient_repeat.cpp",
  "emitted_functions": [
   {
    "name": "SetGradientRepeat__20VIParticleAttributesf",
    "compiled_body_bytes": 20
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_attr_set_gravity_on.o",
  "profile": "authored_cpp",
  "sha256": "6c5ba41b095411233951d1693fe2e7b2d5e5e42583e37d1585fc3bc277587d71",
  "source": "sources/w14-particle-attr-set-gravity-on/set_gravity_on.cpp",
  "emitted_functions": [
   {
    "name": "SetGravityOn__20VIParticleAttributesi",
    "compiled_body_bytes": 64
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_attr_set_inherit_velocity.o",
  "profile": "authored_cpp",
  "sha256": "60840befdcf873e0c4951f9735f4a946cf258fe40c5ef47c5e79228966d834f3",
  "source": "sources/w14-particle-attr-set-inherit-velocity/set_inherit_velocity.cpp",
  "emitted_functions": [
   {
    "name": "SetInheritVelocity__20VIParticleAttributesf",
    "compiled_body_bytes": 20
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_attr_set_lifespan.o",
  "profile": "authored_cpp",
  "sha256": "fc0724d0560ba0aa09e100db5a769b8e0aa95bc6204126732b9d8f68286e2a3b",
  "source": "sources/w14-particle-attr-set-lifespan/set_lifespan.cpp",
  "emitted_functions": [
   {
    "name": "SetLifespan__20VIParticleAttributesf",
    "compiled_body_bytes": 20
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_attr_set_lifespan_var.o",
  "profile": "authored_cpp",
  "sha256": "bbe083e7c182e858ad166fcf0d9bbec2026a3d0708482d7b61dc4ebcd9f997f4",
  "source": "sources/w14-particle-attr-set-lifespan-var/set_lifespan_var.cpp",
  "emitted_functions": [
   {
    "name": "SetLifespanVar__20VIParticleAttributesf",
    "compiled_body_bytes": 20
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_attr_set_nozzle_axis.o",
  "profile": "authored_cpp",
  "sha256": "22322d70822f455ffcc62802aa5446b73713a1faf02089ba589d7e5a9f35bbdf",
  "source": "sources/w14-particle-attr-set-nozzle-axis/set_nozzle_axis.cpp",
  "emitted_functions": [
   {
    "name": "SetNozzleAxis__20VIParticleAttributesRC7VIVect3",
    "compiled_body_bytes": 76
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_attr_set_nozzle_hpr_var.o",
  "profile": "authored_cpp",
  "sha256": "2b7bee2c10481f6a936393860120db814e4923a1512bd8bd61495e7e62356c71",
  "source": "sources/w14-particle-attr-set-nozzle-hpr-var/set_nozzle_hpr_var.cpp",
  "emitted_functions": [
   {
    "name": "SetNozzleHprVar__20VIParticleAttributesRC7VIVect3",
    "compiled_body_bytes": 132
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_attr_set_start_color_var.o",
  "profile": "authored_cpp",
  "sha256": "8711aa45457375686e70355da8b4d9f9f2569713a8e1e89692f762e4393506d8",
  "source": "sources/w14-particle-attr-set-start-color-var/set_start_color_var.cpp",
  "emitted_functions": [
   {
    "name": "SetStartColorVar__20VIParticleAttributesRC10VIColor32F",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_attr_set_start_size.o",
  "profile": "authored_cpp",
  "sha256": "8b82ac17a8f2bc90e18748786a860e206cd723450f076a1a73c9136c381bfa23",
  "source": "sources/w14-particle-attr-set-start-size/set_start_size.cpp",
  "emitted_functions": [
   {
    "name": "SetStartSize__20VIParticleAttributesf",
    "compiled_body_bytes": 20
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_attr_set_start_size_var.o",
  "profile": "authored_cpp",
  "sha256": "55b829f11e5fef851e649ef68b3399614dc1cb14c4996c421a6c2a47a7a2869d",
  "source": "sources/w14-particle-attr-set-start-size-var/set_start_size_var.cpp",
  "emitted_functions": [
   {
    "name": "SetStartSizeVar__20VIParticleAttributesf",
    "compiled_body_bytes": 20
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_attr_set_velocity.o",
  "profile": "authored_cpp",
  "sha256": "a72fa9db70fb591682ddbf91f7fb9e0da14df4dd901850e02135e765665e4856",
  "source": "sources/w14-particle-attr-set-velocity/set_velocity.cpp",
  "emitted_functions": [
   {
    "name": "SetVelocity__20VIParticleAttributesf",
    "compiled_body_bytes": 20
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_attr_set_velocity_var.o",
  "profile": "authored_cpp",
  "sha256": "c0d5f6d7e4166952c6ea42d062ccf811a6add9b7b0bd2a1dce3b7f5d5af9b088",
  "source": "sources/w14-particle-attr-set-velocity-var/set_velocity_var.cpp",
  "emitted_functions": [
   {
    "name": "SetVelocityVar__20VIParticleAttributesf",
    "compiled_body_bytes": 20
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_definition_destroy_motif.o",
  "profile": "authored_cpp",
  "sha256": "cbd842c7a7b323729406dd274e4a45b0a5d97e1a970ef7cda9b68cabc219aa80",
  "source": "sources/w14-particle-definition-destroy-motif/destroy_motif.cpp",
  "emitted_functions": [
   {
    "name": "DestroyMotif__22VIParticleDefinitionExP15VIParticleMotif",
    "compiled_body_bytes": 196
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_definition_ex_dtor.o",
  "profile": "authored_cpp",
  "sha256": "36648b05183b2146b730fb45886faba08dd58353d721f3112ded21214b44fd12",
  "source": "sources/w14-particle-definition-ex-dtor/definition_ex_dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_22VIParticleDefinitionEx",
    "compiled_body_bytes": 96
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_definition_first_motif.o",
  "profile": "authored_cpp",
  "sha256": "417e70ee53b159c5a203cca9cef05ab744b7db212a591c9b96514fdb14ccf19b",
  "source": "sources/w14-particle-definition-first-motif/first_motif.cpp",
  "emitted_functions": [
   {
    "name": "FirstMotif__C22VIParticleDefinitionEx",
    "compiled_body_bytes": 84
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_emitter_create.o",
  "profile": "authored_cpp",
  "sha256": "60cd84b2361b476887489f30fec9e3397e928a4ceef4f482d8cfdef39627a918",
  "source": "sources/w14-particle-emitter-create/emitter_create.cpp",
  "emitted_functions": [
   {
    "name": "Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition",
    "compiled_body_bytes": 328
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_emitter_particle_count.o",
  "profile": "authored_cpp",
  "sha256": "56c4c51d55aa25e7bf70c1d3e0373d7ad7e4c28ba1d545cfbb0685bde8d75e13",
  "source": "sources/w14-particle-emitter-particle-count/particle_count.cpp",
  "emitted_functions": [
   {
    "name": "ParticleCount__C17VIParticleEmitter",
    "compiled_body_bytes": 8
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_emitter_set_location.o",
  "profile": "authored_cpp",
  "sha256": "2f71757ff3f271f9cc417300750577be05acad8358a6d9a867cad24f53e9051a",
  "source": "sources/w14-particle-emitter-set-location/set_location.cpp",
  "emitted_functions": [
   {
    "name": "SetLocation__17VIParticleEmitterRC7VIVect3",
    "compiled_body_bytes": 84
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_emitter_unlink_all_motif_blenders.o",
  "profile": "authored_cpp",
  "sha256": "36b9658d79c798e1b3b2f36626520da7a42921549395c50145c6ae2a2a8a7247",
  "source": "sources/w14-particle-emitter-unlink-all-motif-blenders/unlink_all.cpp",
  "emitted_functions": [
   {
    "name": "UnlinkAllMotifBlenders__17VIParticleEmitter",
    "compiled_body_bytes": 60
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_emitter_update.o",
  "profile": "authored_cpp",
  "sha256": "2c2b38bedb0066ceba39d15ca9be7d8966ab3ac0f9bb0077cb241931294cfbf0",
  "source": "sources/w14-particle-emitter-update/emitter_update.cpp",
  "emitted_functions": [
   {
    "name": "Update__17VIParticleEmitter",
    "compiled_body_bytes": 2428
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_motif_blender_dtor.o",
  "profile": "authored_cpp",
  "sha256": "56d405b3ff0b2b05cdccb049133f3dbb05abfdff68200ba4c621a1f9b9291329",
  "source": "sources/w14-particle-motif-blender-dtor/motif_blender_dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_14VIMotifBlender",
    "compiled_body_bytes": 104
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_p_set_trail_width.o",
  "profile": "authored_cpp",
  "sha256": "ac879e7b4a18c854ea6485d61e42bca4c9063330280b33ae25419607bca9d50c",
  "source": "sources/w14-particle-p-set-trail-width/set_trail_width.cpp",
  "emitted_functions": [
   {
    "name": "P_SetTrailWidth__Fii",
    "compiled_body_bytes": 68
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_system_delta_time.o",
  "profile": "authored_cpp",
  "sha256": "91095c407c40fa8c43c62fdda5d9144bb21b246bf852a103e6d5ffd66d1f569a",
  "source": "sources/w14-particle-system-delta-time/delta_time.cpp",
  "emitted_functions": [
   {
    "name": "DeltaTime__C16VIParticleSystem",
    "compiled_body_bytes": 8
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_particle_system_particle_definition.o",
  "profile": "authored_cpp",
  "sha256": "4d72adf3d00b20cd237d99b2deea403cca577248671fcf937a4122ac5cac4617",
  "source": "sources/w14-particle-system-particle-definition/particle_definition.cpp",
  "emitted_functions": [
   {
    "name": "ParticleDefinition__16VIParticleSystemi",
    "compiled_body_bytes": 28
   }
  ]
 }
]
```

## Imports closed

`SetBirthrate__20VIParticleAttributesf`, `SetBirthrateVar__20VIParticleAttributesf`, `SetDeltaSpawn__20VIParticleAttributesf`, `SetEndColorVar__20VIParticleAttributesRC10VIColor32F`, `SetEndSize__20VIParticleAttributesf`, `SetEndSizeVar__20VIParticleAttributesf`, `SetFriction__20VIParticleAttributesf`, `SetGradientColor__20VIParticleAttributesiRC10VIColor32F`, `SetGradientRepeat__20VIParticleAttributesf`, `SetGravityOn__20VIParticleAttributesi`, `SetInheritVelocity__20VIParticleAttributesf`, `SetLifespan__20VIParticleAttributesf`, `SetLifespanVar__20VIParticleAttributesf`, `SetNozzleAxis__20VIParticleAttributesRC7VIVect3`, `SetNozzleHprVar__20VIParticleAttributesRC7VIVect3`, `SetStartColorVar__20VIParticleAttributesRC10VIColor32F`, `SetStartSize__20VIParticleAttributesf`, `SetStartSizeVar__20VIParticleAttributesf`, `SetVelocity__20VIParticleAttributesf`, `SetVelocityVar__20VIParticleAttributesf`, `DestroyMotif__22VIParticleDefinitionExP15VIParticleMotif`, `_$_22VIParticleDefinitionEx`, `FirstMotif__C22VIParticleDefinitionEx`, `Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition`, `ParticleCount__C17VIParticleEmitter`, `SetLocation__17VIParticleEmitterRC7VIVect3`, `UnlinkAllMotifBlenders__17VIParticleEmitter`, `Update__17VIParticleEmitter`, `_$_14VIMotifBlender`, `P_SetTrailWidth__Fii`, `DeltaTime__C16VIParticleSystem`, `ParticleDefinition__16VIParticleSystemi`.

## Imports opened

`Remove__7VILNode` (also used by neighbours), `AddTail__7VILListP7VILNode`, `_$_t6VIPool1ZP15VIParticleMotif`, `Size__Ct6VIPool1ZP15VIParticleMotif`, `Begin__Ct6VIPool1ZP15VIParticleMotif`, `End__Ct6VIPool1ZP15VIParticleMotif`, `Next__Ct6VIPool1ZP15VIParticleMotifi`, `Erase__t6VIPool1ZP15VIParticleMotifi`, `RecomputeStaticBBox__17VIParticleEmitter`, `BlendMotifs__17VIParticleEmitter`, `TextureConfiguration__C22VIParticleDefinitionEx`, `__ml__FRC7VIVect3RC10VIMatrix33`, `RotateHPR__10VIMatrix44RC7VIVect3`, and the data symbol `trail` (owned by the `particles` package, not by this one). The following are already authored in the shared plan and only referenced: `Normalize__7VIVect3`, `__builtin_delete`, `RemHead__7VILList`, `Identity__10VIMatrix33`, `Identity__10VIMatrix44`, `Destroy__17VIParticleEmitter`, `rand`, `Distance__C7VIVect3RC7VIVect3`, `Mul__C10VIMatrix44RC7VIVect3R7VIVect3`, `_$_10VIMatrix44`.

## Authored but never executed

All 32 functions. They compile for the EE and were executed only on the private R5900 interpreter against the original image with scripted callees, and on the host against stubs. Nothing ran on EE hardware, in an emulator or in the game. Callee behaviour (`rand`, the list and matrix routines, `BlendMotifs`) is scripted, not real.

## Unknown

- Whether any compiled body matches the original instruction for instruction (the larger ones do not; sizes differ).
- `Update__17VIParticleEmitter` leaves the pair (frame base, divisor) uninitialised in the original when `TextureConfiguration__C22VIParticleDefinitionEx` returns anything other than 1 to 4; the source zeroes both, so that case is not equivalent, and a divide-by-zero `break` path of the original (word at emitter +896 equal to zero with a pattern set) is untested because it cannot be reached with a valid run.
- Which words are the field names behind +76, +172, +912, +940, +952, +984 of the emitter and what consumes the counter at attribute +716.
- The sizes of the attribute block beyond +724 (the motif pool starts there) and the array bound of the gradient colours.
- Hardware `rand`, floating-point flags and interrupt behaviour.

# w15-vi-scene-b notes

Package: scene, world, collide, zone and vector math (2 of 2). All 22 functions below are authored under
`experimental/sources/w15-vi-scene-b-*`, one verified function per commit, branch `agents/w15-vi-scene-b`.

Checks performed per function: compile with the reviewed `authored_cpp` flags; a native host check (`host_check.cpp`,
not a build input, recorder callees); and a private paired run of the original image function against the authored one
from identical memory snapshots with identical stub callees, comparing call logs and every changed byte outside the
stack (zero mismatches, no HALT; instruction coverage of the original was checked, uncovered words were not individually examined for
these two functions; they are usually nops or annulled slots). The harness and its output are private and are not in git. Observed/inferred/unknown for each
function are in the header comment of its source file.

## Imports closed
Collide__7VIWorldiR18VISceneCollRayVars, Collide__7VIWorldiR8VIEnvRayR7VIVect3T3,
QueryIntersection__7VIWorldiR20VISceneIntersectVars, Collide__8VIEnvRayUifR7VIVect3,
Collide__8VISpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide, CollideObj__9VICollideR12VICollSphereR9VICapsule,
CollideObj__9VICollideR12VICollSpherei, CollideObj__9VICollideR9VICollRayi, SetWorld__9VICollideRC10VIMatrix44,
QueryObj__9VICollideRC6VIBBoxR14VIQueryTriListi, ShareCollBuffer__9VICollidei, GetWorld__C9VICollide,
RemoveActor__7VIScenei, ShareAnimation__7VIScenei, ShareRefMap__7VIScenei, ShareSprite__7VIScenei,
CopySprite__7VISceneii, Size__C6VIBBox, Contains__C6VIBBoxRC7VIVect3, Magnitude__C7VIVect3,
PointInSolid__C8VISphereRC7VIVect3, __ml__FRC7VIVect3RC10VIMatrix33.

## Imports opened (callees left unresolved, owned elsewhere)
Collide__6VIZoneR18VISceneCollRayVars, Collide__6VIZoneR8VIEnvRayR7VIVect3T2, Collide__9VICollideR12VICollSphereR12VICollBuffer,
Collide__9VICollideR12VICollSphereR9VICapsule, Collide__9VICollideR9VICollRayR12VICollBuffer,
Erase__t6VIList1ZQ27VIScene12VISceneOccupi, Init__12VICollSphereRC12VICollSphereRC10VIMatrix44,
Init__9VICollRayRC9VICollRayRC10VIMatrix44, InvertAffine__10VIMatrix44RC10VIMatrix44, Normalize__7VIVect4,
QueryIntersection__6VIZoneR20VISceneIntersectVars, Query__9VICollideRC6VIBBoxT1R14VIQueryTriListR12VICollBuffer,
Transform__C6VIBBoxRC10VIMatrix44R6VIBBox, __12VICollSphere, __9VICollRay, __ml__FRC7VIVect4RC10VIMatrix44, sceVu0CopyMatrix.

## Build-plan entries needed
Sources are not in `experimental/build-plan.json` (shared file, not edited). Each entry is `kind
independently_authored_source`, profile `authored_cpp`, empty `header_closure`, one source file per directory, with the
sha256 of the file and the compiled function size. Generated entries (22 lines of JSON, in the lead integrator's format)
follow, produced from the committed sources by a private script:

```json
{"emitted_functions":[{"compiled_body_bytes":152,"name":"Contains__C6VIBBoxRC7VIVect3"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_vi_scene_b_bbox_contains.o","profile":"authored_cpp","sha256":"8c3e649963b4fdebd37d963bbbe8bc299b9b2238e2dc2fdbc9553a30cd5e2ac6","source":"sources/w15-vi-scene-b-bbox-contains/contains.cpp"}
{"emitted_functions":[{"compiled_body_bytes":56,"name":"Size__C6VIBBox"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_vi_scene_b_bbox_size.o","profile":"authored_cpp","sha256":"e34886216b44e5140445b1d5625d88790f470182fd7f3866e21d76997ca39d81","source":"sources/w15-vi-scene-b-bbox-size/size.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"GetWorld__C9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_vi_scene_b_collide_get_world.o","profile":"authored_cpp","sha256":"ff5725312395e59400cf7b24200d9b565be6a932b57017af7fe38927fdf46858","source":"sources/w15-vi-scene-b-collide-get-world/get_world.cpp"}
{"emitted_functions":[{"compiled_body_bytes":328,"name":"CollideObj__9VICollideR12VICollSphereR9VICapsule"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_vi_scene_b_collide_obj_capsule.o","profile":"authored_cpp","sha256":"f3ea9a8d418421052601eff099ed813f0f5bd2cde9c0cc7a323b55293514cba0","source":"sources/w15-vi-scene-b-collide-obj-capsule/collide_obj.cpp"}
{"emitted_functions":[{"compiled_body_bytes":344,"name":"CollideObj__9VICollideR9VICollRayi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_vi_scene_b_collide_obj_ray.o","profile":"authored_cpp","sha256":"12ea3a9e5e462c0f49ff7d2073ed7f6e9314a9c8911033f6f82207fde27c7785","source":"sources/w15-vi-scene-b-collide-obj-ray/collide_obj.cpp"}
{"emitted_functions":[{"compiled_body_bytes":344,"name":"CollideObj__9VICollideR12VICollSpherei"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_vi_scene_b_collide_obj_sphere.o","profile":"authored_cpp","sha256":"de84cd95e241cd5160fa122986466467af311091d377d5556402ebf5139d90ec","source":"sources/w15-vi-scene-b-collide-obj-sphere/collide_obj.cpp"}
{"emitted_functions":[{"compiled_body_bytes":120,"name":"QueryObj__9VICollideRC6VIBBoxR14VIQueryTriListi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_vi_scene_b_collide_query_obj.o","profile":"authored_cpp","sha256":"0014b8df7c72bc0fd9c0257398bdd04914650c34d0a8757ff3fba3a755d3f382","source":"sources/w15-vi-scene-b-collide-query-obj/query_obj.cpp"}
{"emitted_functions":[{"compiled_body_bytes":60,"name":"SetWorld__9VICollideRC10VIMatrix44"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_vi_scene_b_collide_set_world.o","profile":"authored_cpp","sha256":"6dec819e6542928d2cfc9af14a0aa8054180c7332645a64db20ae27016f390ad","source":"sources/w15-vi-scene-b-collide-set-world/set_world.cpp"}
{"emitted_functions":[{"compiled_body_bytes":44,"name":"ShareCollBuffer__9VICollidei"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_vi_scene_b_collide_share_buffer.o","profile":"authored_cpp","sha256":"2f96a0f6fa622b78f8238d4aa5576d8744178bb1371226dc555033f0ed3d69a1","source":"sources/w15-vi-scene-b-collide-share-buffer/share.cpp"}
{"emitted_functions":[{"compiled_body_bytes":16,"name":"Collide__8VIEnvRayUifR7VIVect3"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_vi_scene_b_envray_collide.o","profile":"authored_cpp","sha256":"b680e06e96cf590f2d624f71d66a24b8e4b668027ad6adde99a46c06bcb67cce","source":"sources/w15-vi-scene-b-envray-collide/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":68,"name":"CopySprite__7VISceneii"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_vi_scene_b_scene_copy_sprite.o","profile":"authored_cpp","sha256":"11ae226b6e0b13baf3bf288ce9a8dad352c2c402fab61eaf69a03c4957579563","source":"sources/w15-vi-scene-b-scene-copy-sprite/copy_sprite.cpp"}
{"emitted_functions":[{"compiled_body_bytes":312,"name":"RemoveActor__7VIScenei"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_vi_scene_b_scene_remove_actor.o","profile":"authored_cpp","sha256":"d841a93236554824733370c8e5d59146d33cc0f38355b9bf0e5919611720b792","source":"sources/w15-vi-scene-b-scene-remove-actor/remove_actor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":44,"name":"ShareAnimation__7VIScenei"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_vi_scene_b_scene_share_animation.o","profile":"authored_cpp","sha256":"3e8083bca59d104bc0883c51166f730862686650a0f5e174bb7b0e5437afb7ef","source":"sources/w15-vi-scene-b-scene-share-animation/share.cpp"}
{"emitted_functions":[{"compiled_body_bytes":44,"name":"ShareRefMap__7VIScenei"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_vi_scene_b_scene_share_ref_map.o","profile":"authored_cpp","sha256":"0940a03e4613ff2782e238e2da56d3fe6f4e6d1d8a1661b1db5e483ee3b74379","source":"sources/w15-vi-scene-b-scene-share-ref-map/share.cpp"}
{"emitted_functions":[{"compiled_body_bytes":44,"name":"ShareSprite__7VIScenei"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_vi_scene_b_scene_share_sprite.o","profile":"authored_cpp","sha256":"1031c63c92d17dae475db8beba3a04002d0795dc304dacd18cbf3a4922e601a4","source":"sources/w15-vi-scene-b-scene-share-sprite/share.cpp"}
{"emitted_functions":[{"compiled_body_bytes":96,"name":"PointInSolid__C8VISphereRC7VIVect3"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_vi_scene_b_sphere_point_in_solid.o","profile":"authored_cpp","sha256":"ce4baf0dd4a3b6bf5b5ddf0fff38919b9b12078b5288ba1143935c12c0310a9f","source":"sources/w15-vi-scene-b-sphere-point-in-solid/point_in_solid.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"Collide__8VISpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_vi_scene_b_sprite_collide_sphere.o","profile":"authored_cpp","sha256":"0866f52d1f4b54fe636ead63fe65becd8e64bba1610c0ec438b00716d92cd6c6","source":"sources/w15-vi-scene-b-sprite-collide-sphere/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":40,"name":"Magnitude__C7VIVect3"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_vi_scene_b_vect3_magnitude.o","profile":"authored_cpp","sha256":"32aac71bc7eefbfc11ebc43278a26467580afb9a9a512810f5ce260a3746ffb5","source":"sources/w15-vi-scene-b-vect3-magnitude/magnitude.cpp"}
{"emitted_functions":[{"compiled_body_bytes":128,"name":"__ml__FRC7VIVect3RC10VIMatrix33"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_vi_scene_b_vect3_mul_matrix33.o","profile":"authored_cpp","sha256":"7abec8576f57d7924a7c061c0239cf101e5548f390b47d839be45966c0d1af45","source":"sources/w15-vi-scene-b-vect3-mul-matrix33/mul.cpp"}
{"emitted_functions":[{"compiled_body_bytes":692,"name":"Collide__7VIWorldiR8VIEnvRayR7VIVect3T3"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_vi_scene_b_world_collide_envray.o","profile":"authored_cpp","sha256":"9efe9f73e7d7d48e171032203973443487915a9d58dc3eac37624b9664dd8467","source":"sources/w15-vi-scene-b-world-collide-envray/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":800,"name":"Collide__7VIWorldiR18VISceneCollRayVars"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_vi_scene_b_world_collide_ray_vars.o","profile":"authored_cpp","sha256":"c7fa1b7ce50376e4bae14b6b465bb022f9eecc9e85b805e96f4a597e9d7b610f","source":"sources/w15-vi-scene-b-world-collide-ray-vars/collide.cpp"}
{"emitted_functions":[{"compiled_body_bytes":472,"name":"QueryIntersection__7VIWorldiR20VISceneIntersectVars"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_vi_scene_b_world_query_intersection.o","profile":"authored_cpp","sha256":"dddea86c884e42b9112ba9b6ec5d69e864c460489d1bf1a2e9fd3cc9b2be5116","source":"sources/w15-vi-scene-b-world-query-intersection/query_intersection.cpp"}
```

## Authored but never executed
Nothing here has run in the game or in the partial build's link. Execution evidence is only the paired host-interpreter
runs described above and the native host checks; callees were stubs. The functions are not linked into any executable.

## Unknown
- Field names for the VISceneIntersectVars, VISceneCollRayVars and zone structures; offsets are used as observed only.
- Result-value meaning of the zone queries (negative result treated only as the observed early-exit condition).
- Whether tree indices with top nibbles other than 0, 1, 2, 4 and 0xc are ever produced by the world builder.
- Behaviour of the unresolved callees listed above; no claim is made about their contents.

# w16-game-a: game-loop math, physics body, light and world helpers

## What I found
Of the 27 names in the package, 21 already have complete authored sources in this tree that the build plan on this branch does not select yet (so they still show as unresolved here). Four more are defined by units already in the plan or elsewhere. Only `evalHermite3D`, `getTweeks` and three large functions had no definition anywhere.

Existing sources, not in `experimental/build-plan.json` at this branch tip (entries and hashes are in the notes of their own packages, `w9-render-math-a.md` and `w12-vimath-b.md`; I did not duplicate them):
`bodyAddForce`, `bodyApplyForce`, `bodyApplyImpulse`, `bodyGetVelocity`, `bodyInit`, `bodyUpdateMatrix`, `distanceToPlane`, `drawLines`, `getMegatileOriginalPosition`, `lightActivate`, `lightDisableLocalLights`, `lightGlobalCharLightScale`, `lightPopSceneLights` (all `w9-render-math-a-*`), `extractRotation`, `makeQuaternion`, `matMul` (both overloads), `matScale`, `matTranspose`, `quatTransform`, `vqDecompress` (all `w12-vimath-b-*`), `gamePositionVisible` (`w9-render-math-a-game-position-visible`; other units already in the plan also define the same name, so selecting it needs a duplicate-definition check by the integrator). `specialparse` is `megatile-specialparse`, already in the plan.

## Authored here (one commit each)
- `evalHermite3D__FfP6Point3N41` (0x155fc0, 304 bytes): `w16-game-a-eval-hermite3d`. Observed: the four weights and left-to-right sum; output written last. Inferred: cubic Hermite basis, with the first two vectors positions and the last two tangents. Paired against the original on the private interpreter, 80 scenarios (special t values, huge values, output aliasing an input), 76/76 words covered; a weight mutation is caught.
- `getTweeks__FiPsT1P6Point3` (0x27e7d0, 800 bytes): `w16-game-a-get-tweeks`. Observed: ten-record table of two shorts and three singles, unchecked index, two short outputs, then `matUntransform34Vec` on the camera matrix, whose result is returned. Paired for all ten indices, 200/200 words covered, a table-value mutation is caught. Index outside 0..9 not specified.

## Not done (blocked or too large)
- `draw__3mapP12_worldHeaderfffffffG6Point2PFiP6Point3PsPiPiPi_is` (0x15c628, 5,936 bytes): needs the extents and contents of two unnamed float tables, the `_3map$` class statics and a `swap<int>` instantiation first (see `w9-render-math-a.md`). Not started.
- `megatileGenerate__FP12_worldHeader` (0x25e3a8, 19,680 bytes): soft-double helpers and `setjmp`/`longjmp`; see `w12-light-model.md`. Not started.
- `routePlot__FR10RouteQueue` (0x2c0a10, 19,812 bytes): see `w10-world-d.md`. Not started.

## Summary
- Imports closed (new units, once the plan entries below are added): `evalHermite3D__FfP6Point3N41`, `getTweeks__FiPsT1P6Point3`.
- Imports opened: none new (`matUntransform34Vec__FP6Point3P8Matrix34T0` and `worldCameraMatrix` are used and defined elsewhere).
- Build-plan entries needed (private plan, not edited in git):
  - `inputs`: `{"header_closure":[],"kind":"independently_authored_source","object_name":"eval_hermite3d.o","profile":"authored_cpp","sha256":"72ecf1c64ec2626d8cd40c672a1e82c7b95f69a0a417744b0ffb74510b5cb0f5","source":"sources/w16-game-a-eval-hermite3d/eval_hermite3d.cpp"}` (compiled body 208 bytes) and the same shape for `get_tweeks.o`, sha256 `27b98adb2ad07dba2f20f3686b02a4601ed9cd7ae6887aa8cdf16b531e1f45ce`, source `sources/w16-game-a-get-tweeks/get_tweeks.cpp` (160 bytes).
  - `files`: the same two path to sha256 pairs.
  - plus the 21 existing units listed above, from their own notes.
- Authored but never executed: both functions ran only on a private R5900 interpreter against the original image's data, with the callee stubbed. Not run on the EE, in the full build, or in gameplay. I did not run a partial build (other agents were building; compiled each unit alone with the reviewed toolchain).
- Unknown: whether the shorts from `getTweeks` are angles; the meaning of the table values; the three large functions above.

{ lib, stdenv, cmake, buildTests ? false, buildRay ? false, buildExamples ? false }:

let
  pname = "htracer";
  htracerJson = builtins.fromJSON (builtins.readFile ./htracer.json);

  version = htracerJson.version;

  mkCMakeFlag = opt: if opt then "ON" else "OFF";

  buildRayFlag = mkCMakeFlag buildRay;
  buildTestsFlag = mkCMakeFlag buildTests;
  buildExamplesFlag = mkCMakeFlag buildExamples;

in
  stdenv.mkDerivation
  {
    inherit pname;

    name = "${pname}-${version}";
    inherit version;

    src = lib.cleanSource ./.;

    nativeBuildInputs = [ cmake ];

    cmakeFlags = [
      "-DHTRACER_BUILD_RAY=${buildRayFlag}"
      "-DHTRACER_BUILD_TESTS=${buildTestsFlag}"
      "-DHTRACER_BUILD_EXAMPLES=${buildExamplesFlag}"
    ];

    # Multiple outputs using CMake appears to be broken when using the FILE_SET method.
    # I may be doing something wrong or maybe it gets fixed upstream at some point.
    #outputs = [ "out" "dev" ];
  }

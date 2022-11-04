{ lib, stdenv, cmake, buildRay ? true }:

let
  pname = "htracer";
  htracerJson = builtins.fromJSON (builtins.readFile ./htracer.json);

  version = htracerJson.version;

  buildRayFlag = if buildRay then "ON" else "OFF";
in
  stdenv.mkDerivation
  {
    inherit pname;

    name = "${pname}-${version}";
    inherit version;

    src = lib.cleanSource ./.;

    nativeBuildInputs = [ cmake ];

    cmakeFlags = [ "-DHTRACER_BUILD_RAY=${buildRayFlag}" ];

    outputs = [ "out" "dev" ];
  }

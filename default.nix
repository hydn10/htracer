{ lib, stdenv, cmake, buildRay ? true }:

let
  pname = "htracer";
  version = "0.3.0";
  
  buildRayFlag = if buildRay then "ON" else "OFF";
in
  stdenv.mkDerivation
  {
    inherit pname;

    name = "${pname}-${version}";
    inherit version;

    src = lib.cleanSource ./.;

    nativeBuildInputs = [ cmake ];
  
    # TODO: Can this depend on whether "out" or "dev" is being built?
    cmakeFlags = [ "-DHTRACER_BUILD_RAY=${buildRayFlag}" ];

    #outputs = [ "out" "dev" ];
  }

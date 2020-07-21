{ lib, stdenv, cmake, clang-tools }:


stdenv.mkDerivation
{
  name = "htracer";
  src = lib.cleanSource ./.;

  nativeBuildInputs = [ cmake clang-tools ];
}

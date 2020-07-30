{ lib, stdenv, cmake }:


stdenv.mkDerivation
{
  name = "htracer";
  src = lib.cleanSource ./.;

  nativeBuildInputs = [ cmake ];
}

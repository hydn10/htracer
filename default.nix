{ lib, llvmPackages_14, gcc11Stdenv, cmake }:

let
  # p-stdenv = gcc11Stdenv;
  p-stdenv = llvmPackages_14.stdenv;

in
  p-stdenv.mkDerivation
  {
    name = "htracer";
    src = lib.cleanSource ./.;

    nativeBuildInputs = [ cmake ];
  }

{ pkgs ? import <nixpkgs> {}, pkg ? (pkgs.callPackage ./. {}) }:


pkgs.mkShell.override { stdenv = pkg.stdenv; } {
  inputsFrom = [ pkg ];
  nativeBuildInputs = [
    pkgs.llvmPackages_22.clang-tools
    pkgs.gdb
    pkgs.valgrind
  ];
}

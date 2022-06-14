{ pkgs ? import <nixpkgs> {}, pkg ? (pkgs.callPackage ./. {}) }:


pkgs.mkShell.override { stdenv = pkg.stdenv; } {
  inputsFrom = [ pkg ];
  nativeBuildInputs = [
    pkgs.clang-tools_14
    pkgs.gdb
    pkgs.valgrind
    pkgs.kcachegrind
    pkgs.graphviz
  ];
}

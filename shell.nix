{ pkgs ? import <nixpkgs> {}, pkg ? (pkgs.callPackage ./. {}) }:


pkgs.mkShell.override { stdenv = pkg.stdenv; } {
  inputsFrom = [ pkg ];
  nativeBuildInputs = [
    pkgs.clang-tools_18
    pkgs.gdb
    pkgs.valgrind
    pkgs.kcachegrind
    pkgs.graphviz
  ];
}

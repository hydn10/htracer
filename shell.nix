{ pkgs ? import <nixpkgs> {}, pkg ? (pkgs.callPackage ./. {}) }:


let
  clang-tools = pkgs.clang-tools.override {
    llvmPackages = pkgs.llvmPackages_14;
  };
in
  pkgs.mkShell.override { stdenv = pkg.stdenv; } {
    inputsFrom = [ pkg ];
    nativeBuildInputs = [
      clang-tools
      pkgs.gdb
      pkgs.valgrind
      pkgs.kcachegrind
      pkgs.graphviz
    ];
  }

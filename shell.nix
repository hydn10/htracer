{ pkgs ? import <nixpkgs> {} }:


let
  package = pkgs.callPackage ./. {};

  clang-tools = pkgs.clang-tools.override {
    llvmPackages = pkgs.llvmPackages_10;
  };

in
  pkgs.mkShell {
    inputsFrom = [ package ];
    nativeBuildInputs = [ clang-tools pkgs.gdb ];
  }

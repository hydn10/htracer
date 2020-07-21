{ pkgs ? import <nixpkgs> {} }:


let
  package = pkgs.callPackage ./. {};

in
  pkgs.mkShell {
    inputsFrom = [ package ];

    shellHook = ''
      PS1="\[\033[01;32m\]\u\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\] -> \[\033[01;35m\]${package.name}\[\033[00m\]$ "
    '';
  }

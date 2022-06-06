{ pkgs ? import <nixpkgs> {} }:

pkgs.callPackage ./. { buildRay = false;}

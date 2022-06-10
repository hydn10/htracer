{
  description = "A simple C++ CPU raytracer library.";

  outputs = { self, nixpkgs }:
    let
      pkgs-lin64 = import nixpkgs { system = "x86_64-linux"; };
      htracerDrv-lin64 = pkgs-lin64.callPackage ./default.nix {};
    in
    {
      apps.x86_64-linux.ray = 
      {
        type = "app";
        program = "${htracerDrv-lin64}/bin/ray";
      };

      apps.x86_64-linux.default = self.apps.x86_64-linux.ray;

      overlays.default = final: prev: { htracer = self.packages.x86_64-linux.htracer; };

      packages.x86_64-linux.htracer = htracerDrv-lin64;

      packages.x86_64-linux.default = self.packages.x86_64-linux.htracer;

      devShells.x86_64-linux.default = import ./shell.nix {
        pkgs = pkgs-lin64;
        pkg = htracerDrv-lin64;
      };
    };
}

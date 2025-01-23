{
  description = "A modern, modular, header-only C++23 CPU raytracing library.";

  outputs = { self, nixpkgs }:
    let
      pkgs-lin64 = import nixpkgs { system = "x86_64-linux"; };
      htracerDrv-lin64 = pkgs-lin64.callPackage ./default.nix {};
    in
    {
      overlays.default = final: prev: { htracer = self.packages.x86_64-linux.htracer; };

      packages.x86_64-linux.htracer = htracerDrv-lin64;
      packages.x86_64-linux.default = self.packages.x86_64-linux.htracer;

      devShells.x86_64-linux.default = import ./shell.nix {
        pkgs = pkgs-lin64;
        pkg = htracerDrv-lin64;
      };

      checks.x86_64-linux.default =
        let
          drvWithTestsRayExamples = htracerDrv-lin64.override {
            buildTests = true;
            buildRay = true;
            buildExamples = true;
          };
        in
          drvWithTestsRayExamples.overrideAttrs (oldAttrs: {
            doCheck = true;
          });
    };
}

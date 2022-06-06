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

      # From nix >= 2.7 this should be 'packages.<system>.default'
      defaultApp.x86_64-linux = self.apps.x86_64-linux.ray;

      overlay = final: prev: { htracer = self.packages.x86_64-linux.htracer; };

      packages.x86_64-linux.htracer = htracerDrv-lin64;

      # From nix >= 2.7 this should be 'packages.<system>.default'
      defaultPackage.x86_64-linux = self.packages.x86_64-linux.htracer;

      # From nix >= 2.7, this should be 'devShells.<system>.default'
      devShell.x86_64-linux = import ./shell.nix { pkgs = pkgs-lin64; };
    };
}

{ stdenv, cmake, htracer }:

stdenv.mkDerivation {
  pname = "htracer-installed-consumer";
  version = htracer.version;

  src = ../tests/installed_consumer;

  nativeBuildInputs = [ cmake ];
  buildInputs = [ htracer ];

  doCheck = true;
}

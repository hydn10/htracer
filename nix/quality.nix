{ htracer, llvmPackages }:

(htracer.override {
  stdenv = llvmPackages.stdenv;
  buildTests = true;
  buildRay = true;
  buildExamples = true;
}).overrideAttrs (oldAttrs: {
  nativeBuildInputs = (oldAttrs.nativeBuildInputs or []) ++ [
    llvmPackages.clang-tools
  ];

  cmakeBuildType = "Debug";

  cmakeFlags = oldAttrs.cmakeFlags ++ [
    "-DCMAKE_COMPILE_WARNING_AS_ERROR=ON"
    "-DCMAKE_EXE_LINKER_FLAGS=-fsanitize=address,undefined"
    "-DCMAKE_CXX_CLANG_TIDY=${llvmPackages.clang-tools}/bin/clang-tidy;--warnings-as-errors=*"
  ];

  preConfigure = (oldAttrs.preConfigure or "") + ''
    cmakeFlagsArray+=(
      "-DCMAKE_CXX_FLAGS=-Wall -Wextra -pedantic -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all"
    )
  '';

  doCheck = true;

  UBSAN_OPTIONS = "halt_on_error=1:print_stacktrace=1";

  preCheck = (oldAttrs.preCheck or "") + ''
    cmake --build . --target all_verify_interface_header_sets
  '';
})

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![Build Status](https://github.com/hydn10/htracer/actions/workflows/ci.yaml/badge.svg)](https://github.com/hydn10/htracer/actions)

# htracer

A modern, stand-alone, header-only C++23 library for rendering ray-traced images on the CPU.  
Designed with modularity and extensibility in mind, it serves as a foundation for learning and experimenting with API design and ray tracing techniques in modern C++.

<div align="center">
	<img src="https://i.imgur.com/YWPZvKV.png" alt="disco_ring.ppm">
</div>



## Goals

Htracer prioritizes code quality, usability, and modern C++ practices.  
Its primary objectives include:

- **Modularity and Extensibility:** To design a flexible and adaptable framework, allowing users to build upon and extend its functionality with very little effort.
- **Modern C++ Practices:** To leverage features from modern C++ to ensure clean, efficient, and maintainable code.
- **Zero-Cost Abstractions:** To offer high performance without unnecessary overhead, so users pay only for the features they use.
- **No dependencies:** To write it from scratch, as an exercise, using only the standard library.



## Features

### Scene Components
- **Customizable Scene**: Build scenes using lights, materials, and geometries.  
  Provided geometries include:
  - `sphere`: Define spherical objects with position and radius.
  - `plane`: Create infinite flat surfaces with a normal vector and origin.  
  Users can also provide custom geometries by implementing the required concepts.
- **Material Properties**: Supports a shading model with configurable properties such as:
  - Colors (including RGB and HSL representations)
  - Specular coefficient.
  - Shininess and reflectivity

### Rendering Components
- **Lenses**: Choose from predefined lenses or define your own:
  - `pinhole_lens`: Produces sharp images with everything in focus.
  - `thin_lens`: Simulates depth of field with an adjustable focal plane.
- **Sensors**: Simulate different imaging effects:
  - `point_sensor`: Single ray per pixel for a basic rendering.
  - `uniform_sensor`: Models pixels as squares to achieve anti-aliasing.

### Opt-in parallelism
- **Batchers**: Optimize rendering tasks with batch processing:
  - `column_batcher`: Distributes tasks by column for improved cache efficiency.
  - `pixel_batcher`: Splits tasks at the pixel level for fine-grained control.
- **Execution Policies**: Leverage different execution policies for performance:
  - `unseq`: Sequential rendering on a single thread.
  - `par_unseq`: Multithreaded rendering for faster performance.

### Other Features
- **Randomness and Multi-Sampling**: Only applied when required by the components, ensuring optimal performance.  
  - The `make_renderer` function automatically selects the most appropriate renderer at compile time based on component properties.
- **PPM Output**: Serialize rendered images in PPM format for visualization.



## Examples

Below is a basic example to demonstrate htracer in action:

```cpp
#include <htracer/htracer.hpp>

#include <numbers>


int
main()
{
  using ht_f64 = htracer::float_traits<double>;

  ht_f64::scene scene;

  scene.add_light({.position = {-3., 6., 0.}, .color = {1., 1., 1.}, .intensity = 20});

  auto const floor_material = ht_f64::make_solid_material({0.2, 0.2, 0.2}, 0.125, 0, 200, .2);

  auto const make_sphere_material = [](ht_f64::float_type hue)
  {
    auto const color = ht_f64::hsl{hue, 0.7, 0.5}.to_srgb().to_linear();
    return ht_f64::make_solid_material(color, 0.125, 0.05, 200, .4);
  };

  scene.emplace_sphere({{-1.7, 2.0, -5.0}, 2.0}, make_sphere_material(310));
  scene.emplace_sphere({{-3.5, 1.0, 0.0}, 1.0}, make_sphere_material(42));
  scene.emplace_sphere({{2.5, 1.5, 0.0}, 1.0}, make_sphere_material(200));

  scene.emplace_plane({{0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}}, floor_material);

  ht_f64::v3 const cam_pos(0, 2.7, 3.1);
  ht_f64::v3 const cam_view(0, -0.4, -1.);
  ht_f64::v3 const cam_up(0, 1, 0);

  ht_f64::float_type const fov_degrees = 45;
  ht_f64::float_type const fov_rads = fov_degrees * std::numbers::pi / 180;

  ht_f64::camera const camera(cam_pos, cam_view, cam_up, 640, 360, fov_rads);

  // The rendering jobs will be split by column.
  htracer::rendering::batchers::column_batcher const batcher{};

  // The `constant` sensor is the simplest and does not simulate anti-aliasing.
  ht_f64::point_sensor const sensor;

  // The `pinhole` lens models is the simplest and renders all the image in focus.
  ht_f64::pinhole_lens const lens;

  // `make_renderer` will return the appropriate renderer for the properties of our components.
  // In this case the all our components are deterministic (they have no randomness)
  // and so a deterministic_renderer will be returned.
  auto const renderer = htracer::rendering::make_renderer(batcher, scene, camera, sensor, lens);

  // `unseq` means it will run on a single thread. Use `par_unseq` for multithreading.
  auto const image = renderer.render(htracer::rendering::unseq);

  // Save the image in PPM format with 1 byte per value (3 bytes per pixel).
  htracer::outputs::ppm const ppm;
  auto constexpr ppm_bpv = htracer::outputs::ppm::bytes_per_value::BPV1;
  ppm.save<ppm_bpv>("basic.ppm", image);
}
```

Running the code produces this image:

<div align="center">
	<img src="https://i.imgur.com/mjmCmzj.png" alt="basic.ppm">
</div>

Explore more examples in the [examples](examples/) folder.
    


## Usage

Htracer is self-contained and can be integrated into your project in multiple ways:   

### Add to a CMake project using CPM

```cmake
CPMAddPackage(
    NAME HTRACER
    GIT_REPOSITORY https://github.com/hydn10/htracer
    GIT_TAG origin/main
)
```

### Using Nix

Htracer is packaged as a Nix Flake:

```bash
nix build
```

You can import it to your own Flake.

```nix
inputs = {
  htracer.url = "github:hydn10/htracer";
};
```


### Build from source

1.  Clone the repository and navigate to its root directory.
2.  Configure, build and install using CMake:
    
    ```bash
    # List available presets
    cmake --list-presets
    
    # Or set options manually
    cmake -S . -B out -DCMAKE_BUILD_TYPE=Release -DHTRACER_BUILD_TESTS=ON -DHTRACER_BUILD_EXAMPLES=ON

    # Build
    cmake --build out -j

    # Run tests
    cmake --build out --target test

    # Then install
    cmake --install out
    ```


## License

This project is licensed under the [MIT License](LICENSE).

----------

Happy ray tracing! 🎉

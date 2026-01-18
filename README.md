# Maggie's Demonstration Library

Maggie's Demonstration Library (`maggie.dll` and `maggie.lib`) is a small
library used to demonstrate building and testing a DLL or static library using
Visual Studio and CMake.

This repository demonstrates a library that depends on external software that
is managed via `vcpkg`. For this example, the dependencies are
[fmt](https://github.com/fmtlib/fmt) and the
[Windows Implementation Library](https://github.com/microsoft/wil) (WIL). This
was motivated by the situation in which a library is being developed that
depends on software obtained via `vcpkg`, which is in turn used by clients that
also depend on software obtained via `vcpkg`.

In this case, it is generally recommended to distribute the library via
`vcpkg`, so that `vcpkg` can manage both the library and client dependencies
when building the client. In this example, Maggie's Demonstration Library is
distributed via a custom `vcpkg` repository.

A companion project,
[Treasure's Demonstration Library](https://github.com/jme2041/treasure),
demonstrates distributing a header-only library via `vcpkg`.

# Getting Started

Requirements:

 - [Microsoft Windows](https://www.microsoft.com/en-us/windows): The library
   is developed and tested on x86-64 versions of Windows 11.
 - [Visual Studio](https://visualstudio.microsoft.com/): The "Desktop
   development with C++" workload and "C++ CMake tools for Windows" optional
   package are required. If building with Clang, the "C++ Clang tools for
   Windows" optional package is also required. The library is developed and
   tested using Visual Studio 2026.
 - [CMake](https://cmake.org/): Version 3.31 or newer. If building in Visual
   Studio, the CMake that is bundled with Visual Studio is sufficient. If
   building via the command prompt, a standalone CMake installation is also
   needed. The library is developed and tested using CMake 3.31, 4.1, and 4.2.
 - [vcpkg](https://github.com/microsoft/vcpkg): The library is developed and
   tested on the 2025.12.12 and newer releases.

# Developers

To obtain the source code, use:

```cmd
git clone https://github.com/jme2041/maggie
```

The use of CMake presets for configuration, building, and testing is
recommended. The following CMake presets are available in
[CMakePresets.json](CMakePresets.json). All presets target the x86-64
architecture and enable tests. The presets differ by build type (debug or
release), compiler (MSVC or Clang), target library type (static or shared), and
which C and C++ runtime is linked to the targets (static or shared).

| Preset                        | Build Type | Compiler       | Target Libraries | Runtime |
| ----------------------------- | ---------- | -------------- | ------------     | ------- |
| `x64-Debug-MSVC-Shared`       | Debug      | `cl.exe`       | DLL              | DLL     |
| `x64-Debug-MSVC-Static`       | Debug      | `cl.exe`       | Static           | Static  |
| `x64-Debug-MSVC-Static-MD`    | Debug      | `cl.exe`       | Static           | DLL     |
| `x64-Debug-Clang-Shared`      | Debug      | `clang-cl.exe` | DLL              | DLL     |
| `x64-Debug-Clang-Static`      | Debug      | `clang-cl.exe` | Static           | Static  |
| `x64-Debug-Clang-Static-MD`   | Debug      | `clang-cl.exe` | Static           | DLL     |
| `x64-Release-MSVC-Shared`     | Release    | `cl.exe`       | DLL              | DLL     |
| `x64-Release-MSVC-Static`     | Release    | `cl.exe`       | Static           | Static  |
| `x64-Release-MSVC-Static-MD`  | Release    | `cl.exe`       | Static           | DLL     |
| `x64-Release-Clang-Shared`    | Release    | `clang-cl.exe` | DLL              | DLL     |
| `x64-Release-Clang-Static`    | Release    | `clang-cl.exe` | Static           | Static  |
| `x64-Release-Clang-Static-MD` | Release    | `clang-cl.exe` | Static           | DLL     |

Here is an example of how to configure, build, and test the library. Run the
following from an architecture-specific (i.e., x64 for these examples) Visual
Studio developer command prompt from the project's root directory (i.e., the
directory that contains the top-level `CMakeLists.txt`). This example uses the
`x64-Debug-MSVC-Shared` CMake preset.

```cmd
cmake --preset=x64-Debug-MSVC-Shared
cmake --build --preset=x64-Debug-MSVC-Shared
ctest --preset=x64-Debug-MSVC-Shared
```

To configure, build, and test using the Visual Studio IDE, use the option to
open a local folder and select the directory that contains the top-level
`CMakeLists.txt`.

# Consumers

Maggie's Demonstration Library is available through a custom vcpkg registry at
https://github.com/jme2041/vcpkg-registry. `maggie` is distributed via commit
hash `15cc49a86acb78926b36a19baa34e314e376e038` or newer.

To build a CMake C or C++ project that depends on `maggie`, run
`vcpkg new --application` in the directory containing the project's top-level
`CMakeLists.txt` file. This will create two files, `vcpkg-configuration.json`
and `vcpkg.json`. Add the custom `vcpkg` registry to `vcpkg-configuration.json`
so that it looks like this:

```json
{
  "default-registry": {
    "kind": "git",
    "baseline": "000...",
    "repository": "https://github.com/microsoft/vcpkg",
    "reference": "2025.12.12"
  },
  "registries": [
    {
      "kind": "git",
      "repository": "https://github.com/jme2041/vcpkg-registry",
      "reference": "main",
      "baseline": "000...",
      "packages": [
        "maggie"
      ]
    }
  ]
}
```

The `default-registry` entry is always needed (this is where `maggie` obtains
`wil` and the client obtains other dependencies). The `"000..."` in the
baseline field will be filled in automatically by `vcpkg new --application`,
but it can be adjusted manually by editing the file. The `reference` can also
be adjusted as needed.

The `registries` array contains entries for custom registries used by the
project. Add the entry for `jme2041/vcpkg-registry`, replacing `"000..."` in
the baseline field with the earliest commit hash of `jme2041/vcpkg-registry` to
be used by the project. To use `maggie`, this must be the `15cc49...` hash
listed above or newer. List the packages for which to use the custom registry
in the `packages` array.

Next, run `vcpkg add port maggie`. This will add an entry for `maggie` to
`vcpkg.json`:

```
{
  "dependencies": [
    "maggie"
  ]
}
```

If a specific minimum version of `maggie` is needed, the entry for `maggie` in
`vcpkg.json` can be edited to contain a JSON object with the version
information:

```json
{
  "dependencies": [
    {
      "name": "maggie",
      "version>=": "0.1.0.0"
    }
  ]
}
```

Finally, add `find_package(maggie CONFIG REQUIRED)` to `CMakeLists.txt` and
define `CMAKE_TOOLCHAIN_FILE` to point to the `vcpkg` toolchain file.
From the command line, this is typically
`-DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake"`. To
specify whether to link to `maggie` as a shared or static library, set the
CMake configuration variable `VCPKG_TARGET_TRIPLET`. Common triplets include
`x64-windows`, `x64-windows-static`, and `x64-windows-static-md`.

Then, in `CMakeLists.txt`, use `find_package(maggie CONFIG REQUIRED)` and link
to `maggie` using `target_link_libraries(exe_target PRIVATE maggie::maggie)`.

[Amber's Demonstration Client](https://github.com/jme2041/amber) is a small
executable that uses `maggie`. It has CMake presets that configure CMake
appropriately and a `CMakeLists.txt` that links `maggie` to the executable.

# License

Maggie's Demonstration Library is released under the MIT License. For details,
see [LICENSE.txt](LICENSE.txt).

`fmt` and `WIL` are both released under the MIT License and are used in
accordance with that license.

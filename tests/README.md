# Tests

## Prerequisites

Configured from the repository root (`build/` recommended). Catch2 is fetched automatically on first configure when **`BUILD_TESTING=ON`** (CTest default); that first fetch needs GitHub access. Turn tests off entirely with **`cmake -DBUILD_TESTING=OFF`** (skips Catch2).

## Configure and build

From the repo root:

```bash
cmake -S . -B build
cmake --build build --target ttc_tests
```

## Run tests

**Via CTest** (from `build/`):

```bash
cd build && ctest --output-on-failure
```

Limit to the parser suite:

```bash
cd build && ctest --output-on-failure -R cif_parser
```

**Via the Catch2 executable** (runs the same binaries CTest invokes):

```bash
./build/ttc_tests
```

Sample CIFs live under **`../data/samples/`** (path is baked in via `TTC_SAMPLES_DIR`; no need to `cd` into a particular directory).

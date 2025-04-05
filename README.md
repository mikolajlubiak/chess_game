# Chess game
### Demo: https://lubiak.pages.dev/chess/

# Build
* Executable:
```shell
# Generate files
cmake -S . -B build

# Build
cmake --build build

# Run
build/chess
```

* Web:
```shell
# Generate files
emcmake cmake -S . -B build_web
# or
cmake -S . -B build_web -G Ninja -DPLATFORM=Web -DCMAKE_TOOLCHAIN_FILE=/usr/lib/emscripten/cmake/Modules/Platform/Emscripten.cmake

# Copy assets
cp -r assets build_web

# Build
cmake --build build_web

# Host
emrun build_web/chess.html
```


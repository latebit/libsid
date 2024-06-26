<center>
<h1>
⚠️⚠️⚠️ Merged with <a href="https://github.com/latebit/latebit-engine/tree/main/latebit/sid">latebit</a> ⚠️⚠️⚠️
</h1>
</center>

libsid
===

A SID chip emulator to create chiptunes and sound effects. 

Originally developed for the [latebit](https://github.com/latebit/latebit-engine/) game
engine.

## Specification

[LATEST SPECIFICATION](./docs/spec-v0.md)

All the specifications present and future can be found [here](./docs/).

## Usage

```sh
sidplay <path-to-tune-file>
```

## Development

This library relies on [SDL](https://www.libsdl.org/). 

```sh
# Debian-based
sudo apt install libsdl2-dev
# Red Hat-based
sudo dnf install SDL2
# MacOS
brew install sdl2
```

You can find other releases [here](https://github.com/libsdl-org/SDL/releases) and more 
detailed instructions [here](https://wiki.libsdl.org/SDL2/Installation).

## Build

```sh
# Configure
cmake -B build

# Build
cmake --build build

# Test
cmake --build build -t test
```

The `build` folder will now have a `libsid.so` (or `libsid.dylib`, `libsid.dll`) as well as 
the `sidplay` executables.

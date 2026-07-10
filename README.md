# SmartHotel Management System

A console-based hotel management system written in C++. Manages customers,
rooms, reservations, and invoices, with data persisted to JSON files via the
bundled [nlohmann/json](https://github.com/nlohmann/json) header (`json.hpp`).

## Requirements

- A C++17-compatible compiler (the project uses `<filesystem>`), e.g.:
  - GCC 8+ / g++
  - Clang 7+
  - MSVC (Visual Studio 2017 15.7+)
- No external libraries to install — `json.hpp` is vendored in this repo.

## Build

### g++ / MinGW (Windows, Linux, macOS)

From the project root:

```sh
g++ -std=c++17 -o hotelsystem main.cpp hotelsystem.cpp customers.cpp rooms.cpp reservations.cpp invoice.cpp
```

This produces `hotelsystem` (or `hotelsystem.exe` on Windows).

### MSVC (Developer Command Prompt)

```sh
cl /std:c++17 /EHsc main.cpp hotelsystem.cpp customers.cpp rooms.cpp reservations.cpp invoice.cpp /Fe:hotelsystem.exe
```

### VS Code

The repo includes `.vscode/settings.json` with a minor editor setting
(`iostream` file association). It's not tracked in git since it's a local
preference — if you use VS Code, install the C/C++ extension and either use
the build command above in an integrated terminal, or configure your own
`tasks.json`/`launch.json` for one-click build & debug.

## Run

```sh
./hotelsystem
```

On first run the app will create/read the following JSON files in the
working directory as its data store:

- `customers.json`
- `rooms.json`
- `reservations.json`

The admin panel's backup/restore feature copies these into a `backup/`
directory.

## Project structure

| File | Purpose |
|---|---|
| `main.cpp` | Entry point |
| `hotelsystem.h` / `.cpp` | Core app logic and menu |
| `customers.h` / `.cpp` | Customer records |
| `rooms.h` / `.cpp` | Room records |
| `reservations.h` / `.cpp` | Reservation records |
| `invoice.h` / `.cpp` | Invoice generation |
| `json.hpp` | Vendored nlohmann/json library (header-only) |

# mail-backup

## Introduction

mail-backup is an automated solution to download/archive emails from IMAP servers and free services such as Outlook and Gmail. While these services handle their backups on their side, it's important to have a backup if you ever lose access to these services.

## Build

If you're already using vcpkg, you can use the existing installation or create a new one.

### Using Existing vcpkg

#### Windows

```
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake -S .
cmake --build build --config Release
```

#### Linux
```
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake -S .
cmake --build build --config Release
```

### New Installation of vcpkg

#### Windows

```
git clone https://github.com/microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.bat

cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake -S .
cmake --build build --config Release
```

#### Linux
```
git clone https://github.com/microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh

cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake -S .
cmake --build build --config Release
```

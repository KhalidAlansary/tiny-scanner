# Tiny Scanner

## Demo
![demo](https://github.com/user-attachments/assets/76ba1cf4-c8c2-483c-934e-ed950f13f082)

## How to Build and Run

### Prerequisites

- [GCC](https://gcc.gnu.org/)
- [CMake](https://cmake.org/)
- [cJSON](https://github.com/DaveGamble/cJSON)

### Instructions

1. Create a build directory
   ```sh
   mkdir build
   ```
2. Navigate into the build directory:
   ```sh
   cd build
   ```
3. Generate the build files using CMake:
   ```sh
   cmake ..
   ```
4. Build the project:
   ```sh
   make
   ```
5. Run the executable:
   ```sh
   ./scanner [file]
   ```
   - `[file]` is an optional command line argument.

# Tiny Scanner

## Demo

![demo](https://github.com/user-attachments/assets/76ba1cf4-c8c2-483c-934e-ed950f13f082)

## How to Build and Run

### Prerequisites

- [GCC](https://gcc.gnu.org/)
- [CMake](https://cmake.org/download/)
- [Python](https://www.python.org/)

### Install python dependencies

```sh
pip install -r requirements.txt
```

### Build and Run

1. Run cmake:
   ```sh
   cmake -B build
   ```
2. Build the project:
   ```sh
   make -C build
   ```
3. Run the project:
   ```sh
   python app.py
   ```

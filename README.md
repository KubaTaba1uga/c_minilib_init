# c_minilib_init

**`c_minilib_init`** is a lightweight C library for structured **module initialization and cleanup**. It manages registration, dependency resolution, and clean destruction of C modules, using simple macros and clean APIs.

## ✨ Features

- **Ordered Module Initialization**: Initializes modules respecting declared dependencies.
- **Safe Cleanup**: Ensures modules are destroyed in reverse dependency order.
- **Macro-Based Simplicity**: Use `CMI_REGISTER` and `CMI_CONFIGURE` for minimal, intuitive syntax.
- **Flexible Logging**: Pluggable runtime log functions with structured log levels.
- **Full Unit Test Coverage**: Built-in tests using the Unity framework.

## 🧠 Example Usage

### my\_module.c

```c
#include <c_minilib_init.h>
#include <stdio.h>

static int my_module_init(void) {
    puts("My Module Initialized");
    return 0;
}

static void my_module_close(void) {
    puts("My Module Closed");
}

CMI_REGISTER(my_module, my_module_init, my_module_close, NULL, 0);
```

### main.c

```c
#include <c_minilib_init.h>
#include <stdio.h>

int main(void) {
    cmi_error_t err;

    if ((err = cmi_init())) {
        printf("Initialization failed: %s\n", err->msg);
        cmi_error_destroy(&err);
        return 1;
    }

    puts("Application is running...");

    cmi_destroy();
    return 0;
}
```

## ⚙️ Build Instructions

Using [Meson](https://mesonbuild.com/):

```sh
meson setup build
meson compile -C build
```

## ✅ Run Tests

```sh
meson test -C build
```

Tests leverage [Unity](https://www.throwtheswitch.org/unity) for validation.

## 🛠️ Development Tools

Automated with [Invoke](https://www.pyinvoke.org/):

```sh
inv install    # Install required tools
inv build      # Configure & compile
inv test       # Run tests
inv format     # Format code (clang-format)
inv lint       # Run static analysis (clang-tidy)
inv clean      # Clean build artifacts
```

## 🦜 Dependency Handling

Modules declare dependencies at registration. `c_minilib_init` guarantees:

- Dependencies are initialized before dependent modules.
- Modules are closed in the reverse order of initialization.

## 📈 Logging Integration

Optionally set a custom log function during program startup:

```c
void my_logger(enum cmi_LogLevelEnum level, char *msg) {
    printf("[LOG %d]: %s\n", level, msg);
}

CMI_CONFIGURE(my_logger);
```

Supports log levels: `ERROR`, `WARNING`, `INFO`, `DEBUG`.

## 🖊️ Usage Example

Usage example is available under `example/` directory.

## 📄 License

MIT License. See [LICENSE](LICENSE) for full text.

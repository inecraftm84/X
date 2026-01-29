# X Language

**X Language** is a minimalist, dynamically extensible, system-level language with no fixed syntax.

The core philosophy of X-Lang is the **separation of the Dispatcher and the Execution Libraries**. The `xc` core itself contains no built-in logic; it acts as a central hub that dispatches script instructions to various extension libraries loaded via `.so` files.

## Key Features

* **Zero Syntax Constraints:** The definition of commands (e.g., `print` or `1+1=a`) is entirely determined by the libraries you link.
* **Hot-Swappable Extensions:** Change the language's functionality by simply swapping `.so` files—no recompilation of `xc` required.
* **Hardware-Level Access:** Write extensions in C to directly manipulate Framebuffers, GPUs, or system memory.
* **Symbol Sharing:** The core uses `-rdynamic` to provide a global variable space, allowing seamless communication between different libraries.

## 1. Compilation
You will need **GCC** to compile the core and the libraries (this only needs to be done once):

```bash
# Compile the Core (enable -rdynamic for symbol sharing)
gcc -rdynamic -o xc xc.c -ldl

# Compile Extension Libraries
gcc -shared -fPIC -o math.so math.c
gcc -shared -fPIC -o print.so print.c
```
2. Usage
To run a script, pass the script file followed by the libraries you wish to load:
```bash
./xc test.x
```

### Extension Guide
To develop a new instruction, simply implement the following function in your .c file:
```C
#include <stdio.h>
#include <string.h>
extern int* g_ptr(char *name);

void x_exec(char *line) {
    if (/* Your recognition logic here */) {
        // Execute your custom logic
    }
}
```
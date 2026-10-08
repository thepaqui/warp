# warp
**`warp`** is written in `C++2a` and uses `OpenGL 4.6`.  
It was originally made by `thepaqui` for his `scop` project for 42 Nice.
## Compiling libwarp
Create the static library `libwarp.a` using the provided Makefile.  
See the `Makefile` to see how. Or just use it as is. That's why it's here you know.  

## Compiling your project with libwarp
- When compiling your own code:
  - use the `-I/usr/include/GLFW` flag to tell the compiler where the necessary external libraries are.
  - use the `-I./<libFolderName>/include/` flag to tell the compiler where the `warp.hpp` header is.
  - use the `-L./<libFolderName>/` flag to tell the compiler where `libwarp.a` is.
  - use the `-lglfw -ldl` flags to tell the compiler to use those necessary external libraries.
    - On Windows, it might need to be `-lglfw3 -lgdi32 -lopengl32 -I C:\GLFW\include -I C:\GLFW\lib-mingw-w64` instead. No guarantees on this one.
  - use the `-lwarp` flag to tell the compiler which static library to use.
  NOTE: When linking against a static library, these `-l<libname>` flags MUST come after your own source files! Compilation may fail otherwise (lots of undefined references).

It may look something like this:
```
g++ main.cpp -I/usr/include/GLFW -I./libwarp/include -L./libwarp -lglfw -ldl -lwarp -o your_program
```

NOTE: You should check out the `final` branch of this repository for an example project using the lib!  
It comes with a nifty Makefile that will compile both `libwarp.a` and your own project files!

## Using libwarp

- The contents of this repository ***MUST*** be placed in a `warp/` folder at the root of your project.
- In your own project, include the library's header with this line:
```
#include "warp.hpp"
```
- Create a render loop function. Its prototype MUST be:
```
void myRenderLoop(GLFWwindow*, ...);
```
Probably obvious, but you really should make this an actual loop. :)
- Launch it with `launch(myRenderLoop, ...);`
- Don't forget to add an exit condition to your loop! (use ESC or something).

> **Note**: Because examples are often clearer, please check out the `example` branch of this repository for an example of how to use this library.

## Documentation

Currently, there is none.  
There is some information about the shader system in `shaders/README.md` and `shaders/KEYS.md`, but that's it.
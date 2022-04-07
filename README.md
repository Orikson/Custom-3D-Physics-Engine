# Custom-3D-Physics-Engine
A Custom 3D Physics Engine written in C++, designed in a similar form as its [predecessor](https://github.com/Orikson/Custom-2D-Physics-Engine) created in 2 dimensions, implementing rendering techniques as implemented [here](https://github.com/Orikson/3D-Realtime-Rendering).

The engine seeks to demonstrate physical properties of several types of objects constructed by simple shapes in acordance to the types of shapes and meshes that can be quickly rendered. This will, in the future, include liquids and particle systems in the best interests of my education, although will focus upon fundamental physics elements as required commonly by modern video game engines. This engine, then, will be a user interactable physics environment fit for deployment as a game. As such, it will be capable of emulating non-real physical properties such as immobility. A full list of engine functionalities can be found within its corresponding section.


## Visuals
### Render Material Samples (global illumination, without lighting)
Glass | Metal 
:-------------------------:|:-------------------------:
<img src="https://user-images.githubusercontent.com/42471346/156089190-828bea33-e256-40fc-95f9-9129d0d72f06.png" width="250" height="250"> | <img src="https://user-images.githubusercontent.com/42471346/156091233-7e8cc159-d776-4c97-b5d0-d4b594c92b0e.png" width="250" height="250">

### Render Material Samples (global illumination, with diffuse lighting)
Glass | Metal 
:-------------------------:|:-------------------------:
<img src="https://user-images.githubusercontent.com/42471346/156090082-4a824c76-eb25-46a5-a7dc-f14cd1579391.png" width="250" height="250"> | <img src="https://user-images.githubusercontent.com/42471346/156090602-e2b58f80-365e-4dd1-8659-c58d5221caf0.png" width="250" height="250">

### Render Material Samples (global illumination, with diffuse lighting and specular lighting)
TBD

### Physics Properties
#### Softbodies (and cloth)
TBD

#### Object Collisions
<img src="https://user-images.githubusercontent.com/42471346/162101295-01fdfcb5-8c8b-4097-b701-f6b42806a242.gif" width="500" height="500">
<img src="https://user-images.githubusercontent.com/42471346/162111091-427ce767-3937-483f-80f3-ed92ac50e692.gif" width="500" height="500">
Artifacts caused by lossy GIF compression. Still image before compression: 
<img src="https://user-images.githubusercontent.com/42471346/162111780-6dc39cea-dceb-414c-9b81-fd9f8f57fbe2.png" width="500" height="500">


## Installation

The graphics for this engine are handled using OpenGL, and the following instructions are for setting up the code in Windows. 

### Required packages:

Project premade compilation:
* Minimalist GNU for Windows ([MinGW](https://sourceforge.net/projects/mingw/))
  * A GCC Compiler

Libraries required for default program compilation and installed under dependency folder:
* [OpenGL](https://github.com/KhronosGroup/OpenGL-Registry)
* The OpenGL Extension Wrangler Library ([GLEW](https://github.com/nigels-com/glew))
* Simple DirectMedia Layer ([SDL](https://github.com/libsdl-org/SDL))

Useful libraries installed under dependency folder but unused (but could be as per your discretion):
* [FreeGLUT](http://freeglut.sourceforge.net/)
* [FreeType](https://freetype.org/)
* [LibPNG](http://www.libpng.org/pub/png/libpng.html)
* [LibJPEG](http://libjpeg.sourceforge.net/)

### Steps:

1. Install MinGW from the above link, and install the following packages from the installation tab labeled "Basic Setup". You can install all the packages listed there, as they only add MinGW support for Ada and Fortran.
   * mingw-developer-toolkit
   * mingw32-base
   * mingw32-gcc-g++
   * mingw32-gcc-objc
   * msys-base

2. Download this repository, and copy the files from the folders within the directory labeled "Dependencies" into their respective folders within your MinGW installation (e.g. files in the folder `/bin` are copied to `/MinGW/bin`). 

3. Add the path to your MinGW installation to your Path environment variable. To do so, open System Properties from the Control Panel and go to the Advanced tab, or search Path in the Windows search bar and select "Edit the system environment variables". Click "Environment Variables", and then select the Path variable. Click edit, then new, and then enter the path to the root directory of your MinGW installation.
   * You can verify that MinGW has been added to your Path environment variable by typing `path` into the command prompt, and confirming that the path has been added correctly

4. If launching via Visual Studio Code, edit the include path located in the file `/.vscode/c_cpp_properties.json`, and add the path to the include directory inside your MinGW installation (`/MinGW/include`).

5. Once a project is built and compiles, the default location for compiliation is under the builds folder, within the respectively named folder per OS



## Future of the project

- Develop project into a C++ library for easier installation and use.
- Implement common object definitions for common objects such as cloth

## License
[MIT](https://choosealicense.com/licenses/mit/)

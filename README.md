# Build this repository just like you would build the repository from LearnOpenGL
# Make sure that depending on your verions you change the first 2 Lines of the CMakeLists files to version 4.0 instead of 3.0, like this:

cmake_minimum_required (VERSION 4.0)
cmake_policy(VERSION 4.0)

Make sure to insert the path to the Dinosaur Points File at this file in line 168
Also make sure that the first line is the number of points! I removed the comment from my files.
YOUR_PATH_TO\LearnOpenGL\src\3.model_loading\1.model_loading\model_loading.cpp


This reposoitory changed the model_loading.cpp, model_loading.fs and model_loading.vs files in the path YOUR_PATH_TO\LearnOpenGL\src\3.model_loading\1.model_loading

and it made some changes here


        modified:   .gitignore
        modified:   CMakeLists.txt
        modified:   README.md
        modified:   src/1.getting_started/1.1.hello_window/hello_window.cpp
        modified:   src/1.getting_started/2.3.hello_triangle_exercise1/hello_triangle_exercise1.cpp
        modified:   src/2.lighting/2.1.basic_lighting_diffuse/basic_lighting_diffuse.cpp
        modified:   src/3.model_loading/1.model_loading/1.model_loading.fs
        modified:   src/3.model_loading/1.model_loading/1.model_loading.vs
        modified:   src/3.model_loading/1.model_loading/model_loading.cpp


The Dino Images are in the DinoImages folder at the root.

The report as well with the name Task 1.


# Below is the learnOpenGL read me
# learnopengl.com code repository
Contains code samples for all chapters of Learn OpenGL and [https://learnopengl.com](https://learnopengl.com). 

## Windows building
All relevant libraries are found in /libs and all DLLs found in /dlls (pre-)compiled for Windows. 
The CMake script knows where to find the libraries so just run CMake script and generate project of choice.

Keep in mind the supplied libraries were generated with a specific compiler version which may or may not work on your system (generating a large batch of link errors). In that case it's advised to build the libraries yourself from the source.

## Linux building
First make sure you have CMake, Git, and GCC by typing as root (sudo) `apt-get install g++ cmake git` and then get the required packages:
Using root (sudo) and type `apt-get install libsoil-dev libglm-dev libassimp-dev libglew-dev libglfw3-dev libxinerama-dev libxcursor-dev  libxi-dev libfreetype-dev libgl1-mesa-dev xorg-dev` .

**Build through CMake-gui:** The source directory is LearnOpenGL and specify the build directory as LearnOpenGL/build. Creating the build directory within LearnOpenGL is important for linking to the resource files (it also will be ignored by Git). Hit configure and specify your compiler files (Unix Makefiles are recommended), resolve any missing directories or libraries, and then hit generate. Navigate to the build directory (`cd LearnOpenGL/build`) and type `make` in the terminal. This should generate the executables in the respective chapter folders.

**Build through Cmake command line:**
```
cd /path/to/LearnOpenGL
mkdir build && cd build
cmake ..
cmake --build .
```

Note that CodeBlocks or other IDEs may have issues running the programs due to problems finding the shader and resource files, however it should still be able to generate the executables. To work around this problem it is possible to set an environment variable to tell the tutorials where the resource files can be found. The environment variable is named LOGL_ROOT_PATH and may be set to the path to the root of the LearnOpenGL directory tree. For example:

    `export LOGL_ROOT_PATH=/home/user/tutorials/LearnOpenGL`

Running `ls $LOGL_ROOT_PATH` should list, among other things, this README file and the resources directory.

## Mac OS X building
Building on Mac OS X is fairly simple:
```
brew install cmake assimp glm glfw freetype
cmake -S . -B build
cmake --build build -j$(sysctl -n hw.logicalcpu)
```
## Create Xcode project on Mac platform
Thanks [@caochao](https://github.com/caochao):
After cloning the repo, go to the root path of the repo, and run the command below:
```
mkdir xcode
cd xcode
cmake -G Xcode ..
```

## Glitter
Polytonic created a project called [Glitter](https://github.com/Polytonic/Glitter) that is a dead-simple boilerplate for OpenGL. 
Everything you need to run a single LearnOpenGL Project (including all libraries) and just that; nothing more. 
Perfect if you want to follow along with the chapters, without the hassle of having to manually compile and link all third party libraries!

## Ports
Check out [@srcres258](https://github.com/srcres258)'s port in Rust [here](https://github.com/srcres258/learnopengl-rust/).

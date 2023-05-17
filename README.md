# Animation-Blending

<p align="center" width="100%">
   <img width="33%" src="https://github.com/mkversaw/Animation-Blending/assets/30446047/8a8a8844-d629-4424-a894-a52b4a5b01d9"> 
</p>

## Summary

- This application takes the ![bones](https://en.wikipedia.org/wiki/Skeletal_animation) of two ![Mixamo](https://www.mixamo.com/) animations and blends them to provide a seamless transition between them.
  - Using the extracted hierarchical, skeletal, and skinning data from the FBX files, interpolates and reconstructs each frame of the animation.
  - Hierarchy data allows each joint to be stored in a tree structure with the joint names. This enables support for unique properties on specific joints e.g. the hands or head. 
    - This is displayed in the image above, where the character model can hold another model using its hand joint

- Animation blending occurs whenever the user enters or exits a state e.g. running to jumping or jumping to running.
  - Interpolates using frames from the animation currently playing and the first few of the next animation, smoothing the transition no matter the current frame
  - Frames at the start and end of an animation will be looped around to account for the start and end of each frame buffer

## Dependencies

* CMake v3.* (building) 
* GLM v0.9.* (matrix and vector math)
* GLFW v3.3.6 (OpenGL context management)
* GLEW v2.1.0 (OpenGL extension support)
* tinyobjloader v2.0.0 (Loading FBX files)
* fbx-extract (Parsing FBX files)

Build with CMake then run:

`../resources ../animations/<ANIMATION 1> ../animations/<ANIMATION 2>`



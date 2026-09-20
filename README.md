# Real-Time 3D Engine Prototype

A prototype real-time 3D rendering application built from scratch in C++ using Direct3D 11.

This project was developed as part of my capstone project for the **Multimedia Integration Technology (Techniques d'intégration multimédia)** program at Collège de Maisonneuve.

My goal was to learn the fundamentals of graphics APIs, understand real-time rendering pipelines, gain insight into how 3D graphics work under the hood, and push my C++ skills forward through a large-scale project.

This project allowed be to learn more about:
* Managing Direct3D 11 rendering pipelines, swap chains, and depth/stencil buffers.
* 3D linear algebra (matrix transformations, projections, and view matrices).
* Object picking and raycasting mechanics for 3D Gizmos.
* Designing a modular C++ architecture.

Building on the lessons learned from this architecture, I am currently re-architecting and expanding this engine using **Vulkan** to dive deeper into modern graphics APIs.

<br>

[Demo Video](https://www.youtube.com/watch?v=_35LNNOp4js)


<img width="684" height="368" alt="demo-gif" src="https://github.com/user-attachments/assets/a96d5974-10e4-4ff9-8afa-1bbe24e799a3" />

<br>

## Features
- **Interactive 3D Camera System:** Free-fly navigation allowing full movement and orientation within the 3D environment.
- **Transform Gizmos:** Custom 3D gizmos for selecting, moving, and manipulating scene objects interactively.
- **Material & Albedo Mapping:** Shader-based texture rendering pipeline supporting color map materials.
- **Scene Serialization:** Save-and-load system allowing scene states, object positions, and configurations to be persisted to disk.
- **Reflection System:** Simple reflection system allowing for more complex RTTI than standard C++ offers.
- **Custom 3D Algebra:** Custom types and functions for 3D matrix and vector-related operations.

<br>

## Project Tools
- **Language:** C++
- **Graphics API:** Direct3D 11 / HLSL
- **IDE / Build Tools:** Microsoft Visual Studio (MSBuild)
- **Platform:** Windows

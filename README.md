# Graveyard Mystery
Created by Théo & Lou COUARD - IMAC - Fourth Semester 2023

## About the project
This project includes different notion seen during the fourth semester of IMAC like Advanced OpenGL, Images Synthesis and Cleaning Code.
The goal was to implement Boids, a concept defined by Craig Reynolds in 1986.
In our context, Boids are represented as tiny stars, floating in a graveyard.

## Architecture of the project

The root directory contains the following files and directories:

- `Makefile`: The Makefile used for compiling and building the project.
- `Assets`: It contains all the assets used in the project.
  - `OBJ`: It contains all the `.obj` files used in the project (models).
  - `Textures`: It contains the textures used in the project.
    - `SkyboxFaces`: It contains all the textures of the different faces for the skybox.
- `Shaders`: It contains all the shaders used in the project.

The main content is located in the `src` directory.

- `App`: The global environment for the project.
- `Boid`: Handles the Boids behavior.
- `Camera`: Handles the main camera.
- `Framebuff`: Handles OpenGL Framebuffer creation.
- `GUI`: Handles rendering utilities (Fog, LOD).
- `Intensity`: Handles the intensities for the boids.
- `LODModel`: Defines different levels od details for the models.
- `Lights`: Handles the lighting of the scene and lights' creation.
- `Model`: Handles the initialization and the loading process for the models.
- `Shadows`: Handles the shadow mapping.
- `SkyboxEnv`: Handles the creation of the skybox.
- `Texture`: Handles OpenGL 2D Texture creation.
- `VAO`: Handles OpenGL Vertex Array Object creation.
- `VBO`: Handles OpenGL Vertex Buffer Object creation.

## Template for this project
Template from https://julesfouchy.github.io/Learn--OpenGL/

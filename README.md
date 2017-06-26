# Photon

## Important information

#### Radians vs Degrees
Any function that uses angles will take a Angle object, these can either be Degrees or Radians.
Check documentation for more info.
The basics are: ``rad(float x)`` will build an Angle with radians and ``deg(float x)`` will build an Angle with degrees.
Once built the Angle can be get as Radians or Degress using the functions.

## Dependencies
----

#### Outside project: Must be linked

- GLM: OpenGL Mathematics (http://glm.g-truc.net/0.9.8/index.html)
- GLFW: Multiplatform abstraction (http://www.glfw.org/)


##### Included in the project (dep folder)
These are dependencies which are *relatively* small and don't require linking


- Glad: OpenGL loader library (https://github.com/Dav1dde/glad)
- Stb: Nothings' libraries (https://github.com/nothings/stb)
- json.h: Json reader, parser and writer (https://github.com/nlohmann/json)
- rang.h: Multi-platform colored terminal (https://github.com/agauniyal/rang)
- tiny_obj_loader.h: .Obj file loader (https://github.com/syoyo/tinyobjloader)


GLM is not here, but may be added later on for ease of building.

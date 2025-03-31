#version 330 core
layout (location = 0) in vec3 aPos;    // position
layout (location = 1) in vec3 aColor;  // color
layout (location = 2) in vec3 aNormal; // normal

out vec3 FragPos;   // passes position to fragment for lighting
out vec3 Normal;    // for lighting calculations
out vec3 Color;     // color from the dataset

uniform mat4 model; //matrix: transforms the vertex from model space (the object’s own coordinate system) to world space.
uniform mat4 view; //matrix: transforms the vertex from world space to view/camera space (how the scene is viewed by the camera)
uniform mat4 projection; //matrix: transforms the vertex from view space to clip space, applying perspective or orthographic projection.

// Optional uniform if you want to adjust point size from C++:
uniform float uPointSize; // size of rendered point sprite, provided by user input

void main()
{
    // World-space position
    vec4 worldPos = model * vec4(aPos, 1.0); // converts the 3-component position into a 4-component homogeneous coordinate (the 1.0 is the homogeneous coordinate).
    FragPos = worldPos.xyz; // used in fs for lighting calculations

    // Transform the normal (important if model is scaled/rotated)
    //Normals need to be transformed differently than positions, especially when the model matrix includes scaling
    Normal = mat3(transpose(inverse(model))) * aNormal;

    Color = aColor; //This simply passes the color from the vertex attribute to the output variable so it can be used later in the fragment shader.

    // Standard projection transform
    gl_Position = projection * view * worldPos;

    // Set the point size (can also be fixed, e.g. gl_PointSize = 8.0)
    gl_PointSize = uPointSize; // is a built-in variable that specifies the size of the point if you're rendering GL_POINTS (point sprites).
}

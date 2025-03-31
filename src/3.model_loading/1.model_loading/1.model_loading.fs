#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec3 Color;

out vec4 FragColor;

// Basic lighting uniforms
uniform vec3 lightPos;   // position of a single point light
uniform vec3 lightColor; // color of the light
uniform bool  roundPoints; // if true, discard corners

void main()
{
    // (1) If we want a round sprite, discard pixels outside the circle
    if(roundPoints) {
        // gl_PointCoord gives [0..1] coords inside the point
        vec2 coord = gl_PointCoord - 0.5;
        if(length(coord) > 0.5)
            discard;
    }

    // (2) Diffuse (Lambert) lighting
    //  -- normal in world-space, lightPos in world-space
    vec3 norm    = normalize(Normal);             // normalized surface normal
    vec3 lightDir= normalize(lightPos - FragPos); // normalized direction from point to light
    float diff   = max(dot(norm, lightDir), 0.0); // Lambert's cosine law
    //diff is 0.0 when the point faces away from the light, and 1.0 if it faces directly toward the light.
    
    // Combine dataset color with lighting
    vec3 litColor = diff * lightColor * Color;

    FragColor = vec4(litColor, 1.0);
}

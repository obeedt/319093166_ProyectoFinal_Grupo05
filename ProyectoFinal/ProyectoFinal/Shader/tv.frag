#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D screenTexture;
uniform sampler2D noiseTexture;
uniform sampler2D offTexture;
uniform float time;
uniform bool tvOn;
uniform float noiseIntensity;

void main() {
    // Flip Y coordinate to match OpenGL texture orientation
    vec2 adjustedUV = vec2(TexCoords.x, 1.0 - TexCoords.y);
    
    if (tvOn) {
        vec3 baseColor = texture(screenTexture, adjustedUV).rgb;
        vec2 noiseUV = mod(adjustedUV + vec2(time * 0.1), 1.0); // Clamp UVs to [0,1]
        vec3 noise = texture(noiseTexture, noiseUV).rgb;
        float scanLine = sin(adjustedUV.y * 100.0 + time * 5.0) * 0.05;
        vec3 finalColor = mix(baseColor, noise, noiseIntensity) + vec3(scanLine);
        FragColor = vec4(finalColor, 1.0);
    } else {
        FragColor = texture(offTexture, adjustedUV);
    }
}
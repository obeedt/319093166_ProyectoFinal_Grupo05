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
    vec2 adjustedUV = vec2(TexCoords.x, 1.0 - TexCoords.y);
    
    if (tvOn) {
        vec2 noiseUV = mod(adjustedUV + vec2(time * 0.5, time * 0.5), 1.0);
        vec3 noise = texture(noiseTexture, noiseUV).rgb;
        vec3 baseColor = texture(screenTexture, adjustedUV).rgb;
        float scanLine = sin(adjustedUV.y * 300.0 + time * 10.0) * 0.1;
        vec3 finalColor = noise * noiseIntensity + baseColor * (1.0 - noiseIntensity);
        finalColor += vec3(scanLine);
        FragColor = vec4(finalColor, 1.0);
    } else {
        FragColor = texture(offTexture, adjustedUV);
        FragColor.rgb *= vec3(1.0, 0.8, 0.8); // Debug tint
    }
}
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// Material properties
uniform sampler2D texture_diffuse1;

// Light properties
struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight light;

uniform vec3 viewPos;

void main()
{
    // Sample texture
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    if(texColor.a < 0.1)
        discard;
    
    // Normalize normal
    vec3 norm = normalize(Normal);
    
    // Ambient
    vec3 ambient = light.ambient * texColor.rgb;
    
    // Diffuse 
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texColor.rgb;
    
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = light.specular * spec;
    
    // Combine results
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, texColor.a);
}
#version 410

layout(location = 0) out vec4 FragColor;

uniform sampler2D gSampler;

uniform vec4 gLowColor = vec4(253.0/256.0, 94.0/256.0, 83.0/256.0, 1.0);
uniform vec4 gHighColor = vec4(21.0/256.0, 40.0/256.0, 82.0/256.0, 1.0);

in vec2 TexCoords0;
in float Height;
in vec3 newPos;

float uv = 0.5;
float uy = 0.5;

void main()
{
    vec3 pn = normalize(newPos);

    float newTexX = ( asin(pn.x) / float(3.14159f) ) * uv;
    float newTexY = ( asin(pn.y) / float(3.14159f) ) + uy;

    vec2 newtexcoords = vec2(newTexX, newTexY);

    vec4 TexColor = texture2D(gSampler, newtexcoords);

    vec4 SkyColor = mix(gLowColor, gHighColor, Height);

    FragColor = TexColor * 0.7 + SkyColor * 0.3;
}

uniform sampler2D texture;
uniform sampler2D backTexture;
uniform vec2 offset;
uniform vec2 size;

void main() {
    vec2 pixelCoord = gl_TexCoord[0].xy;
    vec2 flippedUV = vec2(pixelCoord.x, 1.0 - pixelCoord.y);
    vec2 uv = flippedUV * size + offset;
    vec4 backColor = texture2D(backTexture, uv);

    vec4 spriteColor = texture2D(texture, pixelCoord);

    spriteColor.rgb = mix(backColor.rgb, spriteColor.rgb, spriteColor.a);
    gl_FragColor = spriteColor;
}
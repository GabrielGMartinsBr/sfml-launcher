uniform sampler2D texture;
uniform sampler2D backTexture;
uniform vec2 offset;
uniform vec2 size;

void main() {
    vec2 flippedUV = vec2(gl_TexCoord[0].x, 1.0 - gl_TexCoord[0].y);

    vec2 pixelCoord = gl_TexCoord[0].xy;
    vec4 texColor = texture2D(texture, pixelCoord);
    
    vec2 uv = flippedUV * size + offset;
    vec4 backColor = texture2D(backTexture, uv);

    vec3 srcRGB = texColor.rgb * texColor.a;
    vec3 destRGB = backColor.rgb * (1.0 - texColor.a);
    vec3 resultRGB = srcRGB + destRGB;

    float resultAlpha = texColor.a + backColor.a * (1.0 - texColor.a);

    gl_FragColor = vec4(resultRGB, resultAlpha);;
}
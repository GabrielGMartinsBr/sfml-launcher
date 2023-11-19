// Fragment shader code for color inversion
uniform sampler2D texture;
// uniform sampler2D backTexture;

void main() {
    vec2 pixelCoord = gl_TexCoord[0].xy;
    vec4 texColor = texture2D(texture, pixelCoord);
    // vec4 backColor = texture2D(backTexture, pixelCoord);

    vec3 srcRGB = texColor.rgb * texColor.a;
    vec3 destRGB = gl_FragColor.rgb * (1.0 - texColor.a);
    vec3 resultRGB = srcRGB + destRGB;

    float resultAlpha = texColor.a + gl_FragColor.a * (1.0 - texColor.a);

    gl_FragColor = vec4(resultRGB, resultAlpha);;
}
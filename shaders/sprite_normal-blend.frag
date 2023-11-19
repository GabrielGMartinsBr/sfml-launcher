// Fragment shader code for color inversion
uniform sampler2D texture;
uniform sampler2D backTexture;
uniform float opacity;

void main() {
    vec2 pixelCoord = gl_TexCoord[0].xy;
    vec4 texColor = texture2D(texture, pixelCoord);
    vec4 backColor = texture2D(backTexture, pixelCoord);

    // vec4 result = vec4((vec3(1.0) - color.rgb), 1.0);
    // vec4 result = backColor + color;

    vec3 srcRGB = texColor.rgb * opacity;
    vec3 destRGB = backColor.rgb * (1.0 - opacity);
    vec3 resultRGB = srcRGB + destRGB;

    float resultAlpha = opacity + backColor.a * (1.0 - opacity);

    gl_FragColor = vec4(resultRGB, resultAlpha);;
}
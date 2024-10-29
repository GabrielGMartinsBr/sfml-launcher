// Fragment shader code for opacity blend
uniform sampler2D texture;
uniform float opacity;

void main() {
    vec2 pixelCoord = gl_TexCoord[0].xy;
    vec4 texColor = texture2D(texture, pixelCoord);

    texColor.a *= opacity;
    gl_FragColor = texColor;
}
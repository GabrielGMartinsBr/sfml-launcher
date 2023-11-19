// Fragment shader code for color inversion
uniform sampler2D texture;
uniform float opacity;

void main() {
    vec2 pixelCoord = gl_TexCoord[0].xy;
    vec4 color = texture2D(texture, pixelCoord);

    gl_FragColor = color;
}

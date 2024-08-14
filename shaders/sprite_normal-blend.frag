// Fragment shader code for color inversion
uniform sampler2D texture;
uniform float opacity;

void main() {
    vec2 pixelCoord = gl_TexCoord[0].xy;
    vec4 texColor = texture2D(texture, pixelCoord);

    gl_FragColor = vec4(texColor.rgb, texColor.a * opacity);
}
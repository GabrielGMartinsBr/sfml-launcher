// Fragment shader code for color inversion
uniform sampler2D texture;

void main() {
    vec2 pixelCoord = gl_TexCoord[0].xy;
    vec4 color = texture2D(texture, pixelCoord);

    // Invert the color by subtracting it from 1.0 (white)
    // vec3 invertedColor =  color.rgb;
    vec3 invertedColor = vec3(1.0) - color.rgb;

    gl_FragColor = vec4(invertedColor, 1.0);
}

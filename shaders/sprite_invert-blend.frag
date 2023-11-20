// Fragment shader code for color inversion
uniform sampler2D texture;
uniform float opacity;

void main() {
    vec4 texColor = texture2D(texture, gl_TexCoord[0].xy);
    
    vec3 resultColor = vec3(1.0) - texColor.rgb;
    vec3 fragColor = clamp(resultColor, 0.0, 1.0);

    gl_FragColor = vec4(fragColor, opacity);
}
// Fragment shader code for color inversion
uniform sampler2D texture;
uniform float opacity;

void main() {
    vec2 pixelCoord = gl_TexCoord[0].xy;
    vec4 texColor = texture2D(texture, pixelCoord);

    // vec3 invertedColor = vec3(1.0) - texColor.rgb;
    // vec3 invertedColor = vec3(1.0) - bgColor.rgb - texColor.rgb;
       
    vec3 resultColor = vec3(1.0) - texColor.rgb;
    // vec4 resultColor = abs(vec4(1.0) - bgColor - texColor);
    // vec4 resultColor = abs(vec4(1.0) - bgColor);
    // mix(texColor, bgColor)
    vec3 fragColor = clamp(resultColor, 0.0, 1.0);
    // vec4 fragColor = abs(resultColor);
    // gl_FragColor = fragColor;

    gl_FragColor = vec4(fragColor, opacity);
    // gl_FragColor = vec4(invertedColor, opacity);
    
    // gl_FragColor = texColor;
}
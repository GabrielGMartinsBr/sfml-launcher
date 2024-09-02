uniform sampler2D texture;
uniform vec4 flash;
uniform float progress;

void main() {
    vec2 pixelCoord = gl_TexCoord[0].xy;
    vec4 spriteColor = texture2D(texture, pixelCoord);

    spriteColor.rgb = mix(spriteColor.rgb, flash.rgb, flash.a * progress);
    gl_FragColor = spriteColor;
}
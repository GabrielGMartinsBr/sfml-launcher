uniform sampler2D texture;
uniform sampler2D frozenTexture;
uniform sampler2D transitionTexture;
uniform float progress;
uniform float vague;

void main() {
    vec2 pixelCoord = gl_TexCoord[0].xy;
    vec2 flippedCoord = vec2(pixelCoord.x, 1.0 - pixelCoord.y);

    float transV = texture2D(transitionTexture, pixelCoord).r;
    float cTransV = clamp(transV, progress, progress + vague);
    float alpha = (cTransV - progress) / vague;

    vec4 spriteColor = texture2D(texture, pixelCoord);
    vec4 frozenColor = texture2D(frozenTexture, flippedCoord);
    
    gl_FragColor =  mix(spriteColor, frozenColor, alpha);
}
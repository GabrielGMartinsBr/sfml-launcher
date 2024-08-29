uniform sampler2D texture;
uniform sampler2D frozenTexture;
uniform float alpha;

void main() {
    vec2 pixelCoord = gl_TexCoord[0].xy;
    vec4 spriteColor = texture2D(texture, pixelCoord);
    
    vec2 flippedCoord = vec2(pixelCoord.x, 1.0 - pixelCoord.y);
    vec4 frozenColor = texture2D(frozenTexture, flippedCoord);
    
    gl_FragColor =  mix(frozenColor, spriteColor, alpha);
}
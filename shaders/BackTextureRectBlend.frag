uniform sampler2D texture;
uniform sampler2D backTexture;
uniform vec2 backTextureSize;
uniform vec2 offset;
uniform vec2 size;
uniform vec2 textureSize;
uniform vec2 texCoordStart;
uniform vec2 texCoordSize;
uniform vec2 texCoordEnd;
uniform float opacity;
uniform float backOpacity;
uniform vec2 spriteSize;

void main() {
    vec2 texCoord = gl_TexCoord[0].xy;
    vec2 flippedUV = vec2(texCoord.x, 1.0 - texCoord.y);
    vec2 coord = gl_FragCoord.xy;
    vec2 coordFlip = vec2(coord.x, coord.y * flippedUV.y);

    vec2 uv = flippedUV * size;

    vec4 color = vec4(1.0);

    color.rgb = vec3(0.0);
    color.r = texCoord.y;
    color.g = texCoord.x;
    color.b = texCoord.y;

    vec4 backColor = texture2D(backTexture, uv);
    vec4 spriteColor = texture2D(texture, texCoord);

    vec2 sprSize = spriteSize / textureSize;
    vec2 scale = textureSize / spriteSize;

    spriteColor = texture2D(texture, texCoord);


    vec2 bts = backTextureSize / textureSize;
    vec2 borderOffset = vec2(0.0);
    vec2 doubleBorderOffset = borderOffset * 2.0;


    // uv = mix(texCoordStart, bts, flippedUV) * 0.5 + 32./textureSize;
    // uv = texCoordStart + flippedUV * (texCoordSize / scale);
    // vec2 scaledOutputSize = spriteSize - doubleBorderOffset;
    // vec2 scaledBTextureSize = backTextureSize - doubleBorderOffset;
    // uv = ((flippedUV - borderOffset) * scaledBTextureSize / scaledOutputSize + borderOffset) / textureSize;
    // uv = mix(texCoordStart, bts, flippedUV) * (bts / spriteSize);
    uv = flippedUV * size;
    // 228  -- 1
    // 192  -- x
    // x = 192/228
    float w = texCoordSize.x * spriteSize.x;
    vec2 bScale = vec2(w, spriteSize.y) / (spriteSize);
    // uv.x *= x;
    uv.x += 32.0 / spriteSize.x;
    // uv.x += 32.0 / spriteSize.x;
    uv.y += 1.0 / spriteSize.y;
    uv.y += 8.0 / spriteSize.y;
    // uv.x += 16.0 / spriteSize.x;
    uv *= bScale;
    vec4 outColor = texture2D(backTexture, uv);
    outColor.a = backOpacity;
    // outColor.a += .5;
    // gl_FragColor = vec4(outColor.rgb + spriteColor.rgb, spriteColor.a);
    // gl_FragColor = outColor.rgba + spriteColor.rgba;
    outColor.rgb = mix(vec3(0.3), outColor.rgb, outColor.a);
    outColor.rgb = outColor.rgb + spriteColor.rgb;
    outColor.a = opacity;
    // outColor = outColor + spriteColor;



    gl_FragColor = outColor;
    return;


    // if (coord.x < spriteSize.x && coord.y < spriteSize.y) {
    // if (flippedUV.x < sprSize.x && flippedUV.y < sprSize.y) {
    // if (flippedUV.x < texCoordEnd.x && flippedUV.y < texCoordEnd.y) {
    // if (coord.x < backTextureSize.x && coord.y < backTextureSize.y) {
    if (texCoord.x < bts.x && flippedUV.y < bts.y) {
        uv = mix(texCoordStart, texCoordEnd, flippedUV) * size + offset;
        // uv.x += .5;
        // uv.y += .5;
        vec4 outColor = texture2D(backTexture, uv);
        gl_FragColor = outColor;

        vec4 c = vec4(1.0);
        c.rgb = vec3(0.0);
        c.r = uv.x;
        c.g = uv.x;
        c.b = uv.x;
        // gl_FragColor = c;
        
        // vec2 prog = flippedUV / texCoordEnd;

        // vec4 outColor = texture2D(backTexture, uv);
        // gl_FragColor = outColor;

        
        return;



        // vec2 sprPos = texCoord * scale;
        // spriteColor = texture2D(texture, prog + 0.50);

        // gl_FragColor = spriteColor;
        return;
    }


    // gl_FragColor = backColor;
    // gl_FragColor = spriteColor;
    color.a = 0.01;
    gl_FragColor = color;

    return;

    // Flip the y-coordinate to correct orientation

    // Map the UV coordinates to the range specified by texCoordStart and texCoordEnd

    // Sample the background texture at the calculated UV coordinates
    // vec4 backColor = texture2D(backTexture, uv);
    backColor.a *= backOpacity; // Apply background opacity

    // Sample the main sprite color using the original texture coordinates
    // vec4 spriteColor = texture2D(texture, texCoord);

    // Apply opacity to the sprite
    spriteColor.a *= opacity;

    // Blend the background and sprite colors based on the sprite's alpha value
    gl_FragColor = mix(backColor, spriteColor, spriteColor.a);
}

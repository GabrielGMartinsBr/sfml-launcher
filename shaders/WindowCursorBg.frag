uniform sampler2D skinTexture;
uniform vec2 textureSize;
uniform vec2 outputSize;

varying vec2 fragTexCoord;

void main() {
    vec2 screenCoord = gl_FragCoord.xy;
    vec2 borderOffset = vec2(1.0);
    vec2 doubleBorderOffset = borderOffset * 2.0;

    vec2 innerBoundaryMin = borderOffset;
    vec2 innerBoundaryMax = outputSize - borderOffset;
    vec2 maxTexBorder = textureSize - doubleBorderOffset;

    vec2 uvCoord;
    
    // Define border areas for stretching
    if (screenCoord.x < borderOffset.x) {
        uvCoord = vec2(0.0, screenCoord.y / outputSize.y);
    } else if (screenCoord.x > innerBoundaryMax.x) {
        uvCoord = vec2(maxTexBorder.x, screenCoord.y / outputSize.y);
    } else if (screenCoord.y < innerBoundaryMin.y) {
        uvCoord = vec2(screenCoord.x / outputSize.x, 0.0);
    } else if (screenCoord.y > innerBoundaryMax.y) {
        uvCoord = vec2(screenCoord.x / outputSize.x, maxTexBorder.y);
    } else {
        // Adjust coordinates for inner section
        vec2 scaledOutputSize = outputSize - doubleBorderOffset;
        vec2 scaledTextureSize = textureSize - doubleBorderOffset;
        uvCoord = ((screenCoord - borderOffset) * scaledTextureSize / scaledOutputSize + borderOffset) / textureSize;
    }

    vec4 color = texture2D(skinTexture, uvCoord);
    color.a = 1.0;

    gl_FragColor = color;
}

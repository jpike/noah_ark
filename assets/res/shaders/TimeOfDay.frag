uniform float color_scale; // Scale factor for current color.
uniform sampler2D texture;  // Current texture being rendered.

void main()
{
    // GET THE CURRENT PIXEL FROM THE TEXTURE.
    vec4 current_pixel_color = texture2D(texture, gl_TexCoord[0].xy);

    // APPLY THE TINTING TO THE PIXEL.
    const float MAX_ALPHA = 1.0f;
    gl_FragColor = vec4(color_scale * current_pixel_color.rgb, MAX_ALPHA);
}

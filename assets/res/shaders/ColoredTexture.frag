uniform vec4 color; // Current color to use to tint texture.
uniform sampler2D texture;  // Current texture being rendered.

void main()
{
    // GET THE CURRENT PIXEL FROM THE TEXTURE.
    vec4 current_pixel_color = texture2D(texture, gl_TexCoord[0].xy);

    // APPLY THE COLOR TO THE PIXEL.
    gl_FragColor = color * current_pixel_color;
}

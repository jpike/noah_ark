uniform sampler2D texture;  // Current texture being rendered.

void main()
{
    // ALTER THE COORDINATE SYSTEM SO THAT (0, 0) IS IN THE CENTER OF THE SCREEN.
    const float SCREEN_WIDTH_IN_PIXELS = 512.0;
    const float HALF_SCREEN_WIDTH_IN_PIXELS = SCREEN_WIDTH_IN_PIXELS / 2.0;
    float normalized_fragment_x_coordinate = gl_FragCoord.x - HALF_SCREEN_WIDTH_IN_PIXELS;

    const float SCREEN_HEIGHT_IN_PIXELS = 384.0f;
    const float HALF_SCREEN_HEIGHT_IN_PIXELS = SCREEN_HEIGHT_IN_PIXELS / 2.0;
    float normalized_fragment_y_coordinate = gl_FragCoord.y - HALF_SCREEN_HEIGHT_IN_PIXELS;

    // COMPUTE THE DISTANCE FROM THE CENTER OF THE SCREEN.
    vec2 normalized_fragment_coordinates = vec2(normalized_fragment_x_coordinate, normalized_fragment_y_coordinate);
    float radius_to_fragment_coordinate = length(normalized_fragment_coordinates);

    const float MAX_RADIUS = SCREEN_WIDTH_IN_PIXELS;
    float normalized_radius_to_fragment_coordinate = radius_to_fragment_coordinate / MAX_RADIUS;

    // COMPUTE SHADING BASED ON DISTANCE FROM THE CENTER.
    const float RAINBOW_COLOR_BAND_COUNT = 7.0;
    const float RELATIVE_WIDTH_PER_COLOR_BAND = 1.0 / RAINBOW_COLOR_BAND_COUNT;
    vec4 current_rainbow_band_color = vec4(1.0, 1.0, 1.0, 1.0);
    if (normalized_radius_to_fragment_coordinate <= RELATIVE_WIDTH_PER_COLOR_BAND)
    {
        const vec4 VIOLET = vec4(0.5, 0.0, 1.0, 1.0);
        current_rainbow_band_color = VIOLET;
    }
    else if (normalized_radius_to_fragment_coordinate <= 2.0 * RELATIVE_WIDTH_PER_COLOR_BAND)
    {
        const vec4 BLUE = vec4(0.0, 0.0, 1.0, 1.0);
        current_rainbow_band_color = BLUE;
    }
    else if (normalized_radius_to_fragment_coordinate <= 3.0 * RELATIVE_WIDTH_PER_COLOR_BAND)
    {
        const vec4 CYAN = vec4(0.0, 1.0, 1.0, 1.0);
        current_rainbow_band_color = CYAN;
    }
    else if (normalized_radius_to_fragment_coordinate <= 4.0 * RELATIVE_WIDTH_PER_COLOR_BAND)
    {
        const vec4 GREEN = vec4(0.0, 1.0, 0.0, 1.0);
        current_rainbow_band_color = GREEN;
    }
    else if (normalized_radius_to_fragment_coordinate <= 5.0 * RELATIVE_WIDTH_PER_COLOR_BAND)
    {
        const vec4 YELLOW = vec4(1.0, 1.0, 0.0, 1.0);
        current_rainbow_band_color = YELLOW;
    }
    else if (normalized_radius_to_fragment_coordinate <= 6.0 * RELATIVE_WIDTH_PER_COLOR_BAND)
    {
        const vec4 ORANGE = vec4(1.0, 0.5, 0.0, 1.0);
        current_rainbow_band_color = ORANGE;
    }
    else if (normalized_radius_to_fragment_coordinate <= 7.0 * RELATIVE_WIDTH_PER_COLOR_BAND)
    {
        const vec4 RED = vec4(1.0, 0.0, 0.0, 1.0);
        current_rainbow_band_color = RED;
    }

    // SHADE THE CURRENT PIXEL BASED ON THE RAINBOW BAND IF ON THE TOP HALF OF THE SCREEN.
    vec4 current_pixel_color = texture2D(texture, gl_TexCoord[0].xy);
    if (normalized_fragment_y_coordinate > 0.0)
    {
        //gl_FragColor = current_rainbow_band_color * current_pixel_color;
        gl_FragColor = current_rainbow_band_color;
    }
    else
    {
        gl_FragColor = current_pixel_color;
    }
}

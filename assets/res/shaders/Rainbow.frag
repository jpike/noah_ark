uniform float alpha_for_rainbow; // Alpha is set for the rainbow to provide a blending type of effect.
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
    // To ensure the rainbow is more of an "ark" rather than just a half-circle, it doesn't start until
    // it's at least a decent amount out of the center.
    const float MIN_RADIUS_BEFORE_RAINBOW = 1.0 / 7.0;
    
    // This width has been chosen to have all bands fit on screen at once.
    const float RELATIVE_WIDTH_PER_COLOR_BAND = 1.0 / 21.0;

    // Main colors of the rainbow.
    const vec4 VIOLET = vec4(0.5, 0.0, 1.0, alpha_for_rainbow);
    const vec4 BLUE = vec4(0.0, 0.0, 1.0, alpha_for_rainbow);
    const vec4 CYAN = vec4(0.0, 1.0, 1.0, alpha_for_rainbow);
    const vec4 GREEN = vec4(0.0, 1.0, 0.0, alpha_for_rainbow);
    const vec4 YELLOW = vec4(1.0, 1.0, 0.0, alpha_for_rainbow);
    const vec4 ORANGE = vec4(1.0, 0.5, 0.0, alpha_for_rainbow);
    const vec4 RED = vec4(1.0, 0.0, 0.0, alpha_for_rainbow);
    // Additional red needed for last interpolation.
    const vec4 DARK_RED = vec4(0.5, 0.0, 0.0, alpha_for_rainbow);

    // Boundaries for each band.
    const float VIOLET_MIN_DISTANCE = MIN_RADIUS_BEFORE_RAINBOW;
    const float VIOLET_MAX_DISTANCE_BLUE_MIN_DISTANCE = VIOLET_MIN_DISTANCE + RELATIVE_WIDTH_PER_COLOR_BAND;
    const float BLUE_MAX_DISTANCE_CYAN_MIN_DISTANCE = VIOLET_MAX_DISTANCE_BLUE_MIN_DISTANCE + RELATIVE_WIDTH_PER_COLOR_BAND;
    const float CYAN_MAX_DISTANCE_GREEN_MIN_DISTANCE = BLUE_MAX_DISTANCE_CYAN_MIN_DISTANCE + RELATIVE_WIDTH_PER_COLOR_BAND;
    const float GREEN_MAX_DISTANCE_YELLOW_MIN_DISTANCE = CYAN_MAX_DISTANCE_GREEN_MIN_DISTANCE + RELATIVE_WIDTH_PER_COLOR_BAND;
    const float YELLOW_MAX_DISTANCE_ORANGE_MIN_DISTANCE = GREEN_MAX_DISTANCE_YELLOW_MIN_DISTANCE + RELATIVE_WIDTH_PER_COLOR_BAND;
    const float ORANGE_MAX_DISTANCE_RED_MIN_DISTANCE = YELLOW_MAX_DISTANCE_ORANGE_MIN_DISTANCE + RELATIVE_WIDTH_PER_COLOR_BAND;
    const float RED_MAX_DISTANCE = ORANGE_MAX_DISTANCE_RED_MIN_DISTANCE + RELATIVE_WIDTH_PER_COLOR_BAND;

    // Use white with no alpha to preserve the current color by default.
    const vec4 WHITE_NO_ALPHA = vec4(1.0, 1.0, 1.0, 0.0);
    vec4 current_rainbow_band_color = WHITE_NO_ALPHA;
    if (normalized_radius_to_fragment_coordinate <= MIN_RADIUS_BEFORE_RAINBOW)
    {
        current_rainbow_band_color = WHITE_NO_ALPHA;
    }
    else  if (normalized_radius_to_fragment_coordinate <= VIOLET_MAX_DISTANCE_BLUE_MIN_DISTANCE)
    {
        float normalized_distance_within_rainbow = (normalized_radius_to_fragment_coordinate - VIOLET_MIN_DISTANCE) / RELATIVE_WIDTH_PER_COLOR_BAND;
        current_rainbow_band_color = mix(VIOLET, BLUE, normalized_distance_within_rainbow);
    }
    else if (normalized_radius_to_fragment_coordinate <= BLUE_MAX_DISTANCE_CYAN_MIN_DISTANCE)
    {
        float normalized_distance_within_rainbow = (normalized_radius_to_fragment_coordinate - VIOLET_MAX_DISTANCE_BLUE_MIN_DISTANCE) / RELATIVE_WIDTH_PER_COLOR_BAND;
        current_rainbow_band_color = mix(BLUE, CYAN, normalized_distance_within_rainbow);
    }
    else if (normalized_radius_to_fragment_coordinate <= CYAN_MAX_DISTANCE_GREEN_MIN_DISTANCE)
    {
        float normalized_distance_within_rainbow = (normalized_radius_to_fragment_coordinate - BLUE_MAX_DISTANCE_CYAN_MIN_DISTANCE) / RELATIVE_WIDTH_PER_COLOR_BAND;
        current_rainbow_band_color = mix(CYAN, GREEN, normalized_distance_within_rainbow);
    }
    else if (normalized_radius_to_fragment_coordinate <= GREEN_MAX_DISTANCE_YELLOW_MIN_DISTANCE)
    {
        float normalized_distance_within_rainbow = (normalized_radius_to_fragment_coordinate - CYAN_MAX_DISTANCE_GREEN_MIN_DISTANCE) / RELATIVE_WIDTH_PER_COLOR_BAND;
        current_rainbow_band_color = mix(GREEN, YELLOW, normalized_distance_within_rainbow);
    }
    else if (normalized_radius_to_fragment_coordinate <= YELLOW_MAX_DISTANCE_ORANGE_MIN_DISTANCE)
    {
        float normalized_distance_within_rainbow = (normalized_radius_to_fragment_coordinate - GREEN_MAX_DISTANCE_YELLOW_MIN_DISTANCE) / RELATIVE_WIDTH_PER_COLOR_BAND;
        current_rainbow_band_color = mix(YELLOW, ORANGE, normalized_distance_within_rainbow);
    }
    else if (normalized_radius_to_fragment_coordinate <= ORANGE_MAX_DISTANCE_RED_MIN_DISTANCE)
    {
        float normalized_distance_within_rainbow = (normalized_radius_to_fragment_coordinate - YELLOW_MAX_DISTANCE_ORANGE_MIN_DISTANCE) / RELATIVE_WIDTH_PER_COLOR_BAND;
        current_rainbow_band_color = mix(ORANGE, RED, normalized_distance_within_rainbow);
    }
    else if (normalized_radius_to_fragment_coordinate <= RED_MAX_DISTANCE)
    {
        float normalized_distance_within_rainbow = (normalized_radius_to_fragment_coordinate - ORANGE_MAX_DISTANCE_RED_MIN_DISTANCE) / RELATIVE_WIDTH_PER_COLOR_BAND;
        current_rainbow_band_color = mix(RED, DARK_RED, normalized_distance_within_rainbow);
    }

    // SHADE THE CURRENT PIXEL BASED ON THE RAINBOW BAND IF ON THE TOP HALF OF THE SCREEN.
    vec4 current_pixel_color = texture2D(texture, gl_TexCoord[0].xy);
    if (normalized_fragment_y_coordinate > 0.0)
    {
        gl_FragColor = current_rainbow_band_color;
    }
    else
    {
        gl_FragColor = current_pixel_color;
    }
}

uniform float scale; // Scale factor to control pulsing size.
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

    // SHIFT THE Y COORDINATE A BIT HIGHER.
    // This is to make the appearance of the light (from God) appear "higher" up on the screen.
    normalized_fragment_y_coordinate -= HALF_SCREEN_HEIGHT_IN_PIXELS;

    // COMPUTE THE DISTANCE FROM THE CENTER OF THE SCREEN.
    vec2 normalized_fragment_coordinates = vec2(normalized_fragment_x_coordinate, normalized_fragment_y_coordinate);
    float radius_to_fragment_coordinate = length(normalized_fragment_coordinates);

    const float MAX_RADIUS = SCREEN_WIDTH_IN_PIXELS;
    float normalized_radius_to_fragment_coordinate = radius_to_fragment_coordinate / MAX_RADIUS;

    // COMPUTE SHADING BASED ON DISTANCE FROM THE CENTER.
    // It should be brighter in the center (hence subtraction from 1).
    float pulsed_radius_to_fragment_coordinate = scale * (1.0 - normalized_radius_to_fragment_coordinate);
    vec4 shading_based_on_pulsing_ball = vec4(
        pulsed_radius_to_fragment_coordinate,
        pulsed_radius_to_fragment_coordinate,
        pulsed_radius_to_fragment_coordinate,
        1.0);

    // SHADE THE CURRENT PIXEL BASED ON THE PULSING.
    vec4 current_pixel_color = texture2D(texture, gl_TexCoord[0].xy);
    gl_FragColor = shading_based_on_pulsing_ball * current_pixel_color;
}

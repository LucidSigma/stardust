SORTING_LAYERS = {
    default = graphics.sorting_layer.new(0.0),

    ground = graphics.sorting_layer.new(0.5),
    moving_platforms = graphics.sorting_layer.new(0.6),
    player = graphics.sorting_layer.new(1.0),
    water = graphics.sorting_layer.new(1.5),

    game_ui = graphics.sorting_layer.new(100.0),
    cursor_ui = graphics.sorting_layer.new(101.0),

    debug = graphics.sorting_layer.new(99.0),
    debug_ui = graphics.sorting_layer.new(150.0),
}

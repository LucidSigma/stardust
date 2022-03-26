ANIMATIONS = { }

ANIMATIONS["colours"] = {
    fps = 24.0,
    length = 32,

    tweens = {
        sprite = {
            frames = {
                [0] = "red",
                [8] = "green",
                [16] = "blue",
                [24] = "yellow",
            },
        },
        position = {
            easing = easings.quint_out,
            frames = {
                [0] = vector2.ZERO,
                [8] = vector2.new(1.0, 0.0),
                [16] = vector2.new(0.0, 1.0),
                [24] = vector2.ONE,
            },
        },
        rotation = {
            easing = easings.bounce_in,
            frames = {
                [0] = 0.0,
                [8] = 90.0,
                [16] = 180.0,
                [24] = 270.0,
            },
        },
        scale = {
            easing = easings.elastic_in_out,
            frames = {
                [0] = vector2.new(0.8, 0.8),
                [24] = vector2.new(0.4, 0.6),
            },
        },
        shear = {
            easing = easings.linear,
            frames = {
                [0] = vector2.ZERO,
                [8] = vector2.new(45.0, 0.0),
            },
        },
        colour = {
            easing = easings.cubic_in,
            frames = {
                [0] = graphics.colour.new(1.0, 1.0, 1.0, 1.0),
                [16] = graphics.colour.new(1.0, 1.0, 1.0, 0.5),
            },
        },
    }
}

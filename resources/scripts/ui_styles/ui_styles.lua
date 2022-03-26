DEBUG_UI_STYLES = {
    ["debug-average-fps"] = {
        anchor = anchor.TOP_RIGHT,
        offset = ivector2.new(-25, 25),

        text_colour = graphics.colour.BLACK,
    },
    ["debug-current-fps"] = {
        anchor = anchor.TOP_RIGHT,
        offset = ivector2.new(-25, 60),

        text_colour = graphics.colour.BLACK,
    },
    ["debug-current-updates"] = {
        anchor = anchor.TOP_RIGHT,
        offset = ivector2.new(-25, 95),

        text_colour = graphics.colour.BLACK,
    },
    ["debug-current-fixed-updates"] = {
        anchor = anchor.TOP_RIGHT,
        offset = ivector2.new(-25, 130),

        text_colour = graphics.colour.BLACK,
    },
}

GAME_UI_STYLES = {
    ["inventory-grid"] = {
        anchor = anchor.TOP_LEFT,
        offset = ivector2.new(24, 16),

        height = 100,
    },
    ["item-name"] = {
        anchor = anchor.TOP_CENTRE,
        offset = ivector2.ZERO,

        font_size = 24.0,

        text_colour = graphics.colour.WHITE,
        outline_thickness = 1.25,
        outline_colour = graphics.colour.BLACK,
    },
    ["inventory-grid-cells"] = {
        anchor = anchor.BOTTOM_CENTRE,
        offset = ivector2.ZERO,

        cell_size = uvector2.new(64, 64),
        cell_spacing = ivector2.new(16, 16),

        item_size = uvector2.new(48, 48),
        item_anchor = anchor.CENTRE,
        item_offset = ivector2.ZERO,

        item_count_anchor = anchor.BOTTOM_RIGHT,
        item_count_offset = ivector2.new(0, -8),
    },
}

CURSOR_UI_STYLES = {
    ["cursor"] = {
        anchor = anchor.TOP_LEFT,
        offset = ivector2.ZERO,

        source = "cursor",
    },
}

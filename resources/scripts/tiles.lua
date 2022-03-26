local INTANGIBLE_TILES <const> = create_set{
    28, 29,
}

local ONE_WAY_TILES <const> = create_set{
    18, 19, 20, 21,
    22, 23,
}

local TOP_HALF_TILES <const> = create_set{
    13,
}

local BOTTOM_HALF_TILES <const> = create_set{
    12,
}

local LEFT_UP_SLOPE_TILES <const> = create_set{
    11,
    22,
}

local RIGHT_UP_SLOPE_TILES <const> = create_set{
    8,
    23,
}

local LEFT_DOWN_SLOPE_TILES <const> = create_set{
    15,
}

local RIGHT_DOWN_SLOPE_TILES <const> = create_set{
    16,
}

local LADDER_TILES <const> = create_set{
    24, 25, 26, 27,
}

local ROPE_TILES <const> = create_set{
    32, 33, 34, 35,
}

local BOUNCY_TILES <const> = create_set{
    6,
}

local STICKY_TILES <const> = create_set{
    30,
}

local WALL_SLIDEABLE_TILES <const> = create_set{
    30, 31,
}

local LEFT_CONVEYOR_TILES <const> = create_set{
    36, 37, 38,
}

local RIGHT_CONVEYOR_TILES <const> = create_set{
    39, 40, 41,
}

local WATER_TILES <const> = create_set{
    42,
}

local WATER_TOP_TILES <const> = create_set{
    43,
}

tile = {
    is_intangible = function(tile)
        return INTANGIBLE_TILES[tile] ~= nil
    end,
    is_one_way = function(tile)
        return ONE_WAY_TILES[tile] ~= nil
    end,
    is_top_half = function(tile)
        return TOP_HALF_TILES[tile] ~= nil
    end,
    is_bottom_half = function(tile)
        return BOTTOM_HALF_TILES[tile] ~= nil
    end,
    is_left_up_slope = function(tile)
        return LEFT_UP_SLOPE_TILES[tile] ~= nil
    end,
    is_right_up_slope = function(tile)
        return RIGHT_UP_SLOPE_TILES[tile] ~= nil
    end,
    is_left_down_slope = function(tile)
        return LEFT_DOWN_SLOPE_TILES[tile] ~= nil
    end,
    is_right_down_slope = function(tile)
        return RIGHT_DOWN_SLOPE_TILES[tile] ~= nil
    end,
    is_ladder = function(tile)
        return LADDER_TILES[tile] ~= nil
    end,
    is_rope = function(tile)
        return ROPE_TILES[tile] ~= nil
    end,
    is_bouncy = function(tile)
        return BOUNCY_TILES[tile] ~= nil
    end,
    is_sticky = function(tile)
        return STICKY_TILES[tile] ~= nil
    end,
    is_wall_slideable = function(tile)
        return WALL_SLIDEABLE_TILES[tile] ~= nil
    end,
    is_left_conveyor = function(tile)
        return LEFT_CONVEYOR_TILES[tile] ~= nil
    end,
    is_right_conveyor = function(tile)
        return RIGHT_CONVEYOR_TILES[tile] ~= nil
    end,
    is_water = function(tile)
        return WATER_TILES[tile] ~= nil
    end,
    is_water_top = function(tile)
        return WATER_TOP_TILES[tile] ~= nil
    end
}

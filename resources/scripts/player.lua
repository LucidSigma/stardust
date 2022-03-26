PLAYER = {
    transform = {
        scale = vector2.new(0.8, 1.8),
    },

    controller = {
        max_slope_climb_angle = math.rad(60.0),
        max_slope_descend_angle = math.rad(75.0),

        drop_down_sensitivity = -0.4,
    },

    raycast_controller = {
        skin_thickness = 0.015,

        ray_counts = {
            horizontal = 8,
            vertical = 8,
        },
    },

    lateral_mover = {
        base_speed = 8.0,
        amble_multiplier = 0.4,
        sprint_multiplier = 1.4,

        stickiness_multiplier = 0.5,

        acceleration_times = {
            grounded = 0.05,
            airborne = 0.1,
        },

        amble_ledge_inset = 0.25,
        y_terminal_velocity = -55.0,
    },

    ladder_climber = {
        ladder_climb_speed = 6.0,
        ladder_descend_speed = 12.0,

        horizontal_movement_sensitivity = 0.3,
    },

    jump_controller = {
        jump_height = 3.5,
        time_to_reach_jump_apex = 0.3,

        stickiness_factor = 0.5,

        max_coyote_time = 0.075,
    },

    wall_slider = {
        max_slide_speed = 3.0,

        climb_jump_force = 7.5,
        off_jump_force = 8.5,
        leap_jump_force = 18.0,

        wall_stick_time = 0.25,

        drop_down_sensitivity = -0.5,
    },

    swimmer = {
        sink_rate = 2.5,
        rise_speed = 7.5,
        trudge_speed = 5.5,

        into_water_acceleration_time = 0.2,
        swim_up_acceleration_time = 0.25,

        voluntary_sink_factor = 3.75,
        vertical_swim_sensitivity = 0.5,
    },

    inventory = {
        size = 10,
    },
}

function are_player_ray_counts_valid()
    return are_ray_counts_valid(PLAYER.raycast_controller.ray_counts)
end

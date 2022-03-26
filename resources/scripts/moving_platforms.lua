MOVING_PLATFORMS = {
    raycast_controller = {
        skin_thickness = 0.015,

        ray_counts = {
            horizontal = 12,
            vertical = 8,
        },
    },

    order_in_sorting_layer = 2,
}

function are_moving_platform_ray_counts_valid()
    return are_ray_counts_valid(MOVING_PLATFORMS.raycast_controller.ray_counts)
end

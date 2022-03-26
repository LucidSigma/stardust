function are_ray_counts_valid(ray_counts)
    local MIN_RAY_COUNT <const> = 2

    return ray_counts.horizontal >= MIN_RAY_COUNT and
        ray_counts.vertical >= MIN_RAY_COUNT
end

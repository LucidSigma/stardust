a = 5
b = 6
c = 7

function get_time()
	return get_elapsed_time()
end

function print_stuff(thing)
	log_trace(thing)
end

function vector_stuff()
	v1 = vec3.new(7.0, 3.0, 4.0)
	v2 = vec3.new(6.0, 1.0, 2.0)
	v3 = v1:cross(v2):normalise()

	log_debug(tostring(v3:length()))
end
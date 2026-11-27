

in vec3 in_vertex_position;
in vec3 in_vertex_color;
in vec2 in_vertex_uv;

out vec3 out_vertex_color;
out vec2 out_vertex_uv;

void
main()
{
	gl_Position = vec4(in_vertex_position.xyz, 1.0);
	out_vertex_color = in_vertex_color;
	out_vertex_uv = in_vertex_uv;
}

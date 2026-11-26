

in vec3 in_vertex_position;
in vec3 in_vertex_color;

out vec3 out_vertex_color;

void
main()
{
	gl_Position = vec4(in_vertex_position.xyz, 1.0);
	out_vertex_color = in_vertex_color;
}

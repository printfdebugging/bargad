

in vec3 out_vertex_color;

out vec4 out_fragment_color;

void
main()
{
	out_fragment_color = vec4(out_vertex_color, 1.0f);
}

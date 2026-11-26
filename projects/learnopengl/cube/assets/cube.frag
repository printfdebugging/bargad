

uniform sampler2D uniform_texture0;

in vec3 out_vertex_color;
in vec2 out_vertex_uv;

out vec4 out_fragment_color;

void
main()
{
	out_fragment_color = texture(uniform_texture0, out_vertex_uv);
}

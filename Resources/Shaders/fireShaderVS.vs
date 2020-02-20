attribute vec3 a_posL;
attribute vec2 a_uv;
varying vec2 v_uv;

attribute vec3 a_color;
varying vec3 v_color;
uniform mat4 u_MVP;

varying vec3 v_pos;
uniform mat4 M;

void main() {
	v_color = a_color;
	v_uv = a_uv;

	vec4 posL = u_MVP * vec4(a_posL, 1.0);


	v_pos = (M * vec4(a_posL, 1.0)).xyz;

	gl_Position = posL;

}
   
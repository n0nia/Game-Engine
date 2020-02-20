attribute vec3 a_posL;
attribute vec3 a_normL;
attribute vec3 a_binromL;
attribute vec3 a_tgtL;
attribute vec2 a_uv;

varying vec2 v_uv;

attribute vec3 a_color;

varying vec3 v_color;

uniform mat4 u_MVP;

varying vec3 v_pos;
varying vec3 v_norm;

uniform mat4 u_Model, u_View, u_Projection;

void main() {
	v_color = a_color;
	v_uv = a_uv;

	v_pos  = (u_Model * vec4(a_posL, 1.0)).xyz;
	v_norm = (normalize(u_Model * vec4(a_normL, 1.0)).xyz);

	vec4 posL = u_MVP * vec4(a_posL, 1.0);

	gl_Position = posL;
}
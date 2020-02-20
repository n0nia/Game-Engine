attribute vec3 a_posL, a_normL;
attribute vec2 a_uv, a_uvt;
varying vec2 v_uv, v_uvt;

attribute vec3 a_color;
varying vec3 v_color;
uniform mat4 u_MVP;
uniform float nrCel;

uniform vec3 u_height;

uniform sampler2D u_texture_3;

uniform mat4 u_Model;

varying vec3 v_pos;
varying vec3 v_norm;

void main() {
	v_color = a_color;
	v_uv = a_uv;
	v_uvt = a_uvt;

	vec3 blendMap = (texture2D(u_texture_3, v_uvt)).rgb;

	vec4 posLNew = vec4(a_posL, 1.0);
	posLNew.y += blendMap.r * u_height.r + blendMap.g * u_height.g + blendMap.b * u_height.b;
	v_pos		= (u_Model * posLNew).xyz;

	vec4 posL	= u_MVP * posLNew;

	gl_Position = posL;

}

  
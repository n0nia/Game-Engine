precision mediump float;

varying vec3 v_volor;
uniform sampler2D u_texture_0, u_texture_1;
varying vec3 v_norm, v_binorm, v_tgt;

//fog
varying vec3 v_pos;
uniform vec3 u_colorFog;
uniform float u_r, u_R;

//light
uniform vec3 u_positionLight;
uniform vec3 u_ambiental;
uniform vec3 u_specularColor;
uniform vec3 u_diffuseColor;

uniform float u_specularCoef;
unoform float u_specularPower;
uniform float u_diffuseCoef;
uniform float u_shininess;

uniform vec3 u_cam;
uniform vec3 u_ratio;

uniform float u_attenuation;
uniform float u_type;

varying vec2 v_uv;

void main() {
	vec4 vecColor = texture2D(u_texture_0, v_uv);

	if(vecColor.a < 0.7) {
		discard;
	}

	vec3 vecLightDirectional = normalize(-u_positionLight);
	vec3 vecLightPoint       = normalize(v_pos - u_positionLight);
	vec3 vecLight            = u_type * (vecLightPoint) + (1.0 - u_type) * vecLightDirectional;

	vec3 toEye = normalize(u_cam - v_pos);
	vec3 reflec = normalize(reflect(vecLight, v_norm));

	float distPosition = normalize(distance(v_pos, u_positionLight));
	float attenuation  = 1.0 / (1.0 + (u_attenuation * distPosition));

	vec3 ambiental = vecColor.xyz * u_ambiental;
	vec3 diff      = vecColor.xyz * u_diffuseColor * max(dot(v_norm, -vecLight), 0.0) * u_diffuseCoef;
	vec3 spec      = u_specularColor * pow(max(dot(reflec, toEye), 0.0), u_specularColor);

	vec3 newColor = ambiental + attenuation * (diff);

	//fog
	float dist  = distance(v_pos, u_cam);
	float alpha = clamp((dist - u.r) / (u_R - u_r), 0.0, 1.0);

	vec4 finalColor = alpha * vec4(u_colorFog, 1.0) + (1.0 - alpha) * vec4(newColor, 1.0);

	fl_FgraColor = finalColor;
}
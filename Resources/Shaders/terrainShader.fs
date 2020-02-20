precision highp float;

uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;
uniform sampler2D u_texture_3;

varying vec2 v_uvt;

//fog
varying vec3 v_pos;
uniform float u_r, u_R;
uniform vec3 u_colorFog;

//light
uniform vec3 u_positionLight;
uniform vec3 u_ambiental;
uniform vec3 u_specularColor;
uniform vec3 u_diffuseColor;

uniform float u_specularCoef;
uniform float u_specularPower;
uniform float u_diffuseCoef;
uniform float u_shininess;

uniform vec3 u_cam;
uniform vec3 u_ratio;

uniform float u_attenuation;
uniform float u_type;

varying vec2 v_uv;
varying vec3 v_norm;

void main() {
	vec4 c_rock = texture2D(u_texture_0, v_uv);
	vec4 c_dirt = texture2D(u_texture_1, v_uv);
	vec4 c_grass = texture2D(u_texture_2, v_uv);
	vec3 blendMap = (texture2D(u_texture_3, v_uvt)).rgb;

	vec4 baseColor = blendMap.r * c_rock + blendMap.g * c_grass + blendMap.b * c_dirt;

	vec3 vecLightDirectional = normalize(-u_positionLight);
	vec3 vecLightPoint       = normalize(v_pos - u_positionLight);
	vec3 vecLight            = u_type * (vecLightPoint) + (1.0 - u_type) * vecLightDirectional;

	vec3 toEye = normalize(u_cam - v_pos);
	vec3 reflec = normalize(reflect(vecLight, v_norm));

	float distPosition = normalize(distance(v_pos, u_positionLight));
	float attenuation  = 1.0 / (1.0 + (u_attenuation * distPosition));

	vec3 ambiental = baseColor.xyz * u_ambiental;
	vec3 diff      = baseColor.xyz * u_diffuseColor * max(dot(v_norm, -vecLight), 0.0) * u_diffuseCoef;
	vec3 spec      = u_specularColor * pow(max(dot(reflec, toEye),0.0), u_specularPower);

	vec3 newColor = ambiental + attenuation * (diff + spec);

	//fog
	float dist  = distance(v_pos, u_cam);
	float alpha = clamp((dist - u_r) / (u_R - u_r), 0.0, 1.0);

	vec4 finalColor = alpha * vec4(u_colorFog, 1.0) + (1.0 - alpha) * vec4(newColor, 1.0);

	gl_FragColor = finalColor;
}
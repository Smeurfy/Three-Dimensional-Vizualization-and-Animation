#version 330

uniform sampler2D texmap;
uniform sampler2D texmap1;
uniform sampler2D texmap2;
uniform sampler2D texmap3;
uniform sampler2D lensflare;
uniform int texMode;

out vec4 colorOut;

struct Materials {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec4 emissive;
	float shininess;
	int texCount;
};

uniform Materials mat;

struct LightProperties {
	bool isEnabled;	// true to apply this light in this invocation
	bool isLocal;	// true for a point light or a spotlight,
			// false for a positional light

	bool isSpot;	// true if the light is a spotlight
	vec4 ambient;	// light’s contribution to ambient light
	vec4 diffuse;
	vec4 specular;

	vec3 position_point;
	vec4 direction;
	vec4 position;	// location of light, if is Local is true,
			// otherwise the direction toward the light

	vec3 halfVector;	// direction of highlights for directional light
	vec3 coneDirection; 	// spotlight attributes

	float spotCosCutoff;
	float spotExponent;

	float constantAttenuation;	// local light attenuation coefficients
	float linearAttenuation;
	float quadraticAttenuation;

	// other properties you may desire
};

uniform vec4 l_dir;	   // camera space

in Data {
	vec3 normal;
	vec3 eye;
	vec4 pos;
	vec2 tex_coord;
} DataIn;

const int nrLights = 9;
uniform LightProperties lights[nrLights];
float dist;
uniform bool isFog;

uniform int uGhosts = 9; // number of ghost samples
uniform float uGhostDispersal = 0.3; // dispersion factor
uniform int cameraID;

void main() {

	vec3 scatteredLight = vec3(0.0); // or, to a global ambient light
	vec3 reflectedLight = vec3(0.0);
	vec4 texel, texel1;
	
	// loop over all the lights
	for (int light = 0; light < nrLights; light++) {
		if (!lights[light].isEnabled) continue;

		vec3 halfVector;
		vec3 lightDirection = vec3(lights[light].position);
		float attenuation = 1.0;
		
		if(lights[light].isLocal) {
			lightDirection = lightDirection - vec3(DataIn.pos);
			float lightDistance = length(lightDirection);
			lightDirection = lightDirection / lightDistance;
			attenuation = 1.0 / (lights[light].constantAttenuation
					+ lights[light].linearAttenuation * lightDistance
					+ lights[light].quadraticAttenuation * lightDistance
					* lightDistance);

			if (lights[light].isSpot){
				float spotCos = dot(lightDirection, normalize(vec3(-lights[light].direction)));
				if (spotCos < 0.9)
					attenuation = 0.0;
				else
					attenuation *= pow(spotCos, lights[light].spotExponent);
			}

			halfVector = normalize(lightDirection + DataIn.eye);
		}
		else {
			lightDirection = normalize(-lightDirection);
			halfVector = normalize((lightDirection + DataIn.eye) / 2);
		}
	
		float diffuse = max(0.0, dot(DataIn.normal, lightDirection));
		float specular = max(0.0, dot(DataIn.normal, halfVector));
		if (diffuse == 0.0) specular = 0.0;
		else specular = pow(specular, mat.shininess);

		scatteredLight += lights[light].diffuse.rgb * mat.ambient.rgb * attenuation + lights[light].ambient.rgb * mat.diffuse.rgb * diffuse * attenuation;
		reflectedLight += lights[light].ambient.rgb * mat.specular.rgb * specular * attenuation;
	}
	vec3 rgb = min((mat.emissive.rgb + scatteredLight + reflectedLight), vec3(1.0));
	vec4 cout = vec4(rgb, mat.diffuse.a);

	

	if (texMode == 0) {
		texel = texture(texmap2, DataIn.tex_coord);  // texel from lighwood.tga
		colorOut = max(cout * mat.diffuse * texel, 0.1 * texel);
	}
	else if (texMode == 1) // diffuse color is replaced by texel color, with specular area or ambient (0.1*texel)
	{
		texel = texture(texmap, DataIn.tex_coord);  // texel from stone.tga
		colorOut = max(cout*texel, 0.1*texel);
	}
	else if (texMode == 2) {
		texel = texture(texmap2, DataIn.tex_coord);  // texel from lighwood.tga
		texel1 = texture(texmap, DataIn.tex_coord);  // texel from checker.tga
		colorOut = max(cout * mat.diffuse * texel * texel1, 0.1 * texel * texel1);
	}
	else if(texMode == 3){
		texel = texture(texmap3, DataIn.tex_coord);  		
		if(texel.a == 0.0) 
			discard;
		else
			colorOut = vec4(max(cout.rgb*texel.rgb, 0.1*texel.rgb), texel.a);
	}
	else {
		//vec3 stuff = min(mat.diffuse.rgb, vec3(1.0));
		colorOut = cout * mat.diffuse;
		//colorOut = vec4(stuff, mat.diffuse.a);
	}

	// fog input
	if(isFog){
		dist = length(DataIn.pos);
		float fogAmount = exp(-dist * 0.05);
		vec3 fogColor = vec3(0.5,0.6,0.7);
		vec3 finalColor = mix(fogColor, colorOut.rgb, fogAmount);
		colorOut = vec4(finalColor,1);
	}

	if (lights[0].isEnabled && cameraID == 3) {
		vec2 texcoord = -vec2(DataIn.pos / length(DataIn.pos)) - vec2(1.0);
		vec2 texelSize = 1.0 / vec2(textureSize(lensflare, 0));
 
		// ghost vector to image centre:
		vec2 ghostVec = (vec2(0.5) - texcoord) * uGhostDispersal;
   
		// sample ghosts:  
		vec4 result = vec4(0.0);
		for (int i = 0; i < uGhosts; ++i) { 
			vec2 offset = fract(texcoord + ghostVec * float(i));
			float weight = length(vec2(0.5) - offset) / length(vec2(0.5));
			weight = pow(1.0 - weight, 10.0);
			result += texture(lensflare, offset) * weight;
		}
			colorOut += result;
	}

}
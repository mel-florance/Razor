#version 330 core

in vec3 fPosition;
in vec2 fUvs;
out vec4 FragColor;

uniform vec3 uSunPos;
uniform vec2 limits;
uniform vec4 clearColor;
uniform float sun_intensity;

uniform float sun_factor;
uniform vec3 ray_origin;
uniform float planet_radius;
uniform float atmosphere_radius;
uniform vec3 rayleigh_scattering;
uniform float mie_scattering;
uniform float rayleigh_scale_height;
uniform float mie_scale_height;
uniform float mie_scattering_direction;

#define PI 3.141592
#define iSteps 4
#define jSteps 2

vec2 rsi(vec3 r0, vec3 rd, float sr) 
{
    // ray-sphere intersection
    float a = dot(rd, rd);
    float b = 2.0 * dot(rd, r0);
    float c = dot(r0, r0) - (sr * sr);
    float d = (b * b) - 4.0 * a * c;

    if (d < 0.0)
	return vec2(1e5,-1e5);

    return vec2(
        (-b - sqrt(d)) / (2.0 * a),
        (-b + sqrt(d)) / (2.0 * a)
    );
}

vec3 atmosphere(vec3 r, vec3 r0, vec3 pSun, float iSun, float rPlanet, float rAtmos, vec3 kRlh, float kMie, float shRlh, float shMie, float g)
{
    pSun = normalize(pSun);
    r = normalize(r);

    // Step size
    vec2 p = rsi(r0, r, rAtmos);
    if (p.x > p.y) return vec3(0,0,0);
    p.y = min(p.y, rsi(r0, r, rPlanet).x);
    float iStepSize = (p.y - p.x) / float(iSteps);

    float iTime = 0.0;
    vec3 totalRlh = vec3(0,0,0);
    vec3 totalMie = vec3(0,0,0);
    float iOdRlh = 0.0;
    float iOdMie = 0.0;

    // Rayleigh and Mie phases
    float mu = dot(r, pSun);
    float mumu = mu * mu;
    float gg = g * g;
    float pRlh = 3.0 / (16.0 * PI) * (1.0 + mumu);
    float pMie = 3.0 / (8.0 * PI) * ((1.0 - gg) * (mumu + 1.0)) / (pow(1.0 + gg - 2.0 * mu * g, 1.5) * (2.0 + gg));

	// Primary ray
    for (int i = 0; i < iSteps; i++)
	{
        // Primary ray sample position.
        vec3 iPos = r0 + r * (iTime + iStepSize * 0.5);

        // Height of the sample.
        float iHeight = length(iPos) - rPlanet;

        // Optical depth of the Rayleigh and Mie scattering
        float odStepRlh = exp(-iHeight / shRlh) * iStepSize;
        float odStepMie = exp(-iHeight / shMie) * iStepSize;

        // Accumulate optical depth.
        iOdRlh += odStepRlh;
        iOdMie += odStepMie;

        // Step size of the secondary ray.
        float jStepSize = rsi(iPos, pSun, rAtmos).y / float(jSteps);
        float jTime = 0.0;
        float jOdRlh = 0.0;
        float jOdMie = 0.0;

        // Secondary ray.
        for (int j = 0; j < jSteps; j++) 
		{
            // Secondary ray sample position.
            vec3 jPos = iPos + pSun * (jTime + jStepSize * 0.5);

            // Height of the sample.
            float jHeight = length(jPos) - rPlanet;

            // Accumulate optical depth.
            jOdRlh += exp(-jHeight / shRlh) * jStepSize;
            jOdMie += exp(-jHeight / shMie) * jStepSize;

            // Increment secondary ray
            jTime += jStepSize;
        }

        // Attenuation.
        vec3 attn = exp(-(kMie * (iOdMie + jOdMie) + kRlh * (iOdRlh + jOdRlh)));

        // Accumulate scattering.
        totalRlh += odStepRlh * attn;
        totalMie += odStepMie * attn;

        // Increment primary ray
        iTime += iStepSize;
    }

    // Calculate and return the final color.
    return iSun * (pRlh * kRlh * totalRlh + pMie * kMie * totalMie);
}

void main() 
{
    vec3 color = atmosphere(
        normalize(fPosition),           // normalized ray direction
        ray_origin * 1000.0f,           // ray origin
        uSunPos,                        // position of the sun
        sun_intensity * sun_factor,     // intensity of the sun
        planet_radius * 1000.0f,        // radius of the planet in meters
        atmosphere_radius * 1000.0f,    // radius of the atmosphere in meters
        rayleigh_scattering,            // Rayleigh scattering coefficient
        mie_scattering,                 // Mie scattering coefficient
        rayleigh_scale_height,          // Rayleigh scale height
        mie_scale_height,               // Mie scale height
        mie_scattering_direction        // Mie preferred scattering direction
    );

    // Exposure.
    color = 1.0 - exp(-1.0 * color);

	float factor = (fPosition.y - limits.x) / (limits.y - limits.x);
	factor = clamp(factor, 0.0, 1.0);

    FragColor = mix(clearColor, vec4(color, 1.0f), factor);
}
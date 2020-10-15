#version 330 core

uniform sampler2D mask;
uniform vec2 pixelSize;
uniform vec4 color;
uniform int width;
in vec2 f_uvs;

out vec4 FragColor;

void main()
{
    bool isInside = false;
    int count = 0;
    float coverage = 0.0;
    float dist = 1e10;

    for (int y = -width;  y <= width;  ++y) 
	{
        for (int x = -width;  x <= width;  ++x)
		{
            vec2 dUV = vec2(float(x) * pixelSize.x, float(y) * pixelSize.y);
            float mask = texture(mask, vec2(f_uvs.x, 1.0 - f_uvs.y) + dUV).r;
            coverage += mask;

            if (mask >= 0.5)
                dist = min(dist, sqrt(float(x * x + y * y)));
        
            if (x == 0 && y == 0)
                isInside = (mask > 0.5);
        
            count += 1;
        }
    }

    coverage /= float(count);
    float a;

    if (isInside)
        a = min(1.0, (1.0 - coverage) / 0.75);
    else 
    {
        float solid = 0.3 * float(width);
        float fuzzy = float(width) - solid;
        a = 1.0 - min(1.0, max(0.0, dist - solid) / fuzzy);
    }

	FragColor = color;
    FragColor.a = a;
//	FragColor = vec4(1.0f, 0.65f, 0.0f, 0.8f);
}
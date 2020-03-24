#version 410

layout(location = 0) in vec2 texture_coord;

uniform sampler2D textureImage;
uniform ivec2 screenSize;
uniform int flipVertical;

// 0 - original
// 1 - grayscale
// 2 - blur
// 3 - median
// 4 - frontier
uniform int outputMode = 2;

// Flip texture horizontally when
vec2 textureCoord = vec2(texture_coord.x, (flipVertical != 0) ? 1 - texture_coord.y : texture_coord.y);

layout(location = 0) out vec4 out_color;

vec4 grayscale()
{
	vec4 color = texture(textureImage, textureCoord);
	float gray = 0.21 * color.r + 0.71 * color.g + 0.07 * color.b; 
	return vec4(gray, gray, gray,  0);
}

vec4 blur(int blurRadius)
{
	vec2 texelSize = 1.0f / screenSize;
	vec4 sum = vec4(0);
	for(int i = -blurRadius; i <= blurRadius; i++)
	{
		for(int j = -blurRadius; j <= blurRadius; j++)
		{
			sum += texture(textureImage, textureCoord + vec2(i, j) * texelSize);
		}
	}
		
	float samples = pow((2 * blurRadius + 1), 2);
	return sum / samples;
}

vec4 median()
{
    vec2 texelSize = 1.0f / screenSize;
    vec4[9] toSort;
	for (int i = 0 ; i < 9; i++) {
		toSort[i] = vec4(0);
	}
    vec4 sum = vec4(0);

    for(int i = -1; i <= 1; i++)
	{
		for(int j = -1; j <= 1; j++)
		{
            toSort[(i + 1) * 3 + (j + 1)] = texture(textureImage, textureCoord + vec2(i, j) * texelSize);
		}
	}

    int i, j;  
    for (i = 0; i < 8; i++)         
        for (j = 0; j < 8 - i; j++) {
            float v1 =  0.21 * toSort[j].r + 0.71 * toSort[j].g + 0.07 * toSort[j].b;
            float v2 =  0.21 * toSort[j+1].r + 0.71 * toSort[j+1].g + 0.07 * toSort[j+1].b;
            if (v1 > v2) {
                vec4 temp = toSort[j];  
                toSort[j] = toSort[j+1];  
                toSort[j+1] = temp;
            }

        }
	return toSort[4];
}

vec4 frontier()
{
    mat3 Gx = mat3(-1, 0, 1, -2, 0, 2, -1, 0, 1);
	mat3 Gy = mat3(-1, -2, -1, 0, 0, 0, 1, 2, 1);
    vec2 texelSize = 1.0f / screenSize;
    float[9] pixel_mat;
    float gx_sum = 0;
    float gy_sum = 0;
    float threshold = 0.2f;
	vec4 auxaux = vec4(0);

    for(int i = -1; i <= 1; i++)
	{
		for(int j = -1; j <= 1; j++)
		{
            vec4 aux = texture(textureImage, textureCoord + vec2(i, j) * texelSize);
            pixel_mat[(i + 1) * 3 + (j + 1)] =  0.21 * aux.r + 0.71 * aux.g + 0.07 * aux.b;
		}
	}


    for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
            gx_sum += pixel_mat[i * 3 + j] * Gx[i][j];
            gy_sum += pixel_mat[i * 3 + j] * Gy[i][j];
		}
	}

    float aux = sqrt(gx_sum * gx_sum + gy_sum * gy_sum);
    
    return texture(textureImage, textureCoord);
}

void main()
{
	switch (outputMode)
	{
		case 1:
		{
			out_color = grayscale();
			break;
		}

		case 2:
		{
			out_color = blur(3);
			break;
		}

        case 3:
        {
            out_color = median();
			break;
        }

        case 4:
        {
            out_color = frontier();
			break;
        }


		default:
			out_color = texture(textureImage, textureCoord);
			break;
	}
}
//Isaac Lovy
vec4 clipPixel(vec4 pixel_color, float threshold)
{
	return vec4(pixel_color.rgb, step(pixel_color.a, threshold) * pixel_color.a);
}

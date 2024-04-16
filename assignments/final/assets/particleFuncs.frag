//Isaac Lovy
vec4 clipPixel(vec4 pixel_color, float threshold)
{
	return vec4(pixel_color.rgb, 1.0 - step(pixel_color.a, threshold));
}

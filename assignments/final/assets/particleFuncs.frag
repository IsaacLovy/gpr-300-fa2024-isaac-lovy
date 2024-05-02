//Isaac Lovy
vec4 clipPixelTest(vec4 pixel_color, float threshold)
{
	return vec4(pixel_color.rgb, step(pixel_color.a, threshold) * pixel_color.a);
}

//Henry Foley
float clipPixel(float alpha, float threshold)
{
	if(alpha < threshold)
		discard;
	return alpha;
}

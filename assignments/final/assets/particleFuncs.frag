//Henry Foley
float clipPixel(float alpha, float threshold)
{
	if(alpha < threshold)
		discard;
	return alpha;
}


//Isaac Lovy
float discardAtWorldY(float alpha, float y, float killY)
{
	if (y < killY)
		discard;
	return alpha;
}
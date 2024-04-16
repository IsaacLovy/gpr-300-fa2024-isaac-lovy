float attenuateExponential(float dist, float radius){
	float i = clamp(1.0 - pow(dist/radius,4.0),0.0,1.0);
	return i * i;	
}
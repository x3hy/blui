const double vectors[][3] = {
	// Base triangle
	{ -0.5, -0.5, -0.5 },   // 0
	{  0.5, -0.5, -0.5 },   // 1
	{  0.0,  0.5, -0.5 },   // 2
	
	// Apex
	{  0.0,  0.0,  0.5 }    // 3
};

// Connecting points
const int edges[][2] = {
	// Base
	{0,1}, {1,2}, {2,0},
	
	// Sides
	{0,3}, {1,3}, {2,3}
};

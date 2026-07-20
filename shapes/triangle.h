const double vectors[][3] = {
	// Top triangle (z = 0.5)
	{  0.0,  0.5,  0.5 },   // 0
	{ -0.5, -0.5,  0.5 },   // 1
	{  0.5, -0.5,  0.5 },   // 2
	
	// Bottom triangle (z = -0.5)
	{  0.0,  0.5, -0.5 },   // 3
	{ -0.5, -0.5, -0.5 },   // 4
	{  0.5, -0.5, -0.5 },   // 5
};

// Connecting points
const int edges[][2] = {
	// Top triangle
	{0,1}, {1,2}, {2,0},
	
	// Bottom triangle
	{3,4}, {4,5}, {5,3},
	
	// Vertical edges
	{0,3}, {1,4}, {2,5}
};

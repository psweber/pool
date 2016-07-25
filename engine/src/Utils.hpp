#pragma once

namespace engine
{

// Make sure that fabs(v) >= tol
double stabilize(double v,double tol=1e-13)
{
	if ((v>0) && (v<tol)) { return tol; }
	if ((v<0) && (v>-tol)) { return -tol; }
	
	return v;
}

}

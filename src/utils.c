/****************************************
 * Nolan Mullins
 * 0939720
 * 2020-01-15
 * Game programing - Utils
 ****************************************/

#include <math.h>

void getUnitVector(float unitVec[3], float vec[3])
{
    float l = sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
    for (int i = 0; i < 3; i++)
        unitVec[i] = vec[i]/l; 
}

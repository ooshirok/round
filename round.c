#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void put_round(double x, int n);
double my_round( double x, const int n );

int main(int argc, char **argv)
{
	int		ret;

	double	x;
	int		n;

 	put_round(1534, 0);
 	put_round(1535, 0);
 	put_round(1535, 1);
 	put_round(1565, 1);
 	put_round(1234, 2);
 	put_round(1534, 2);

	exit(0);
}

void put_round(double x, int n)
{
	printf( "x=%.0f, n=%d, round(x)=%.0f\n", x, n, my_round(x,n) );
	return;
}

/* x を 10^n の桁で四捨五入 */
/* ex.)
 * round(1534, 0) -> 1530
 * round(1535, 0) -> 1540
 * round(1535, 1) -> 1500
 * round(1565, 1) -> 1600
 * round(1234, 2) -> 1000
 * round(1534, 2) -> 2000
 */
double my_round( double x, const int n )
{
	double zoom = pow(10,n+1);
	x /= zoom;
	x += 0.5;
	return (floor(x) * zoom);
}

#include <iostream>
#include <complex>
#include <chrono>
using namespace std;

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#define N 200
#define eps 0.00001
#define d 0.01
double a[N][2];
int n;

double dist(const double first[2], const double second[2])
{
	double r=(first[0] - second[0])*(first[0] - second[0]) + (first[1] - second[1])*(first[1] - second[1]);
	return sqrt(r);
}

double circle(double center[2], double r)
{
	double sum = 0;
	double first[2], second[2];
	complex<double> f, s, rotator;
	for (int i = 1; i < n; i++)
	{
		first[0] = a[i - 1][0]; first[1] = a[i - 1][1];
		second[0] = a[i][0]; second[1] = a[i][1];
		first[0] -= center[0];
		second[0] -= center[0];
		first[1] -= center[1];
		second[1] -= center[1];
		f = complex<double>(first[0],first[1]);
		s = complex<double>(second[0],second[1]);
		rotator = s-f;
		rotator /= abs(rotator);
		rotator = conj(rotator);
		f *= rotator; s *= rotator;
		//intersection(a[i - 1], a[i]), circle
		// segment entirely inside the circle
		if (dist(a[i - 1], center) < r + eps && dist(a[i], center) < r + eps)
			sum += dist(a[i - 1], a[i]);
		else if (dist(a[i - 1], center) < r + eps && dist(a[i], center)+eps > r)
		{
			// first in, second out
			if (r*r - f.imag()*f.imag()>0)
			sum+=sqrt(r*r - f.imag()*f.imag());
		}
		else if (dist(a[i - 1], center) + eps > r && dist(a[i], center) < r + eps)
		{
			//first out, second in
			if (r*r - s.imag()*s.imag()>0)
			sum += sqrt(r*r - s.imag()*s.imag());
		}
		else
		{
			//endpoints out, intersection depends on the distance from the line
			if (fabs(s.imag()) < r)
			{
				sum += (2 * sqrt(r*r - s.imag()*s.imag()));
			}
		}
	}
	//cerr << "* " << center[0] << " " << center[1] << " " << r << " " << sum << endl;
	return sum;
}

int main()
{
	cin >> n;
	for (int i = 0; i < n; i++)
		cin >> a[i][0] >> a[i][1];
	auto start = chrono::high_resolution_clock::now();
	double relative = 0;
	double sol1[2], r1=0;
	for (int i = 0; i < n; i++)
	for (int j = i+1; j < n; j++)
	{
		double r = dist(a[i], a[j]);
		if (relative < eps+(circle(a[i], r) / r))
		{
			relative = circle(a[i], r) / r;
			r1 = r;
			sol1[0] = a[i][0]; sol1[1] = a[i][1];
		}
	}
	auto finish = chrono::high_resolution_clock::now();
	chrono::duration<double, std::milli> ms_double = finish - start;
	std::cerr << ms_double.count() << " ms\n";
	cout <<sol1[0] << " "<< sol1[1] << " " << r1 << " " << relative << endl;

	return 0;
}
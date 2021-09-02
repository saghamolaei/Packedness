#include<iostream>
#include<algorithm>
#include<complex>
#include<vector>
#include <chrono>
using namespace std;

#define point complex<double>
#define circle pair<double,point>
#define eps 1e-10

//compute a circle with a and b on its boundary
circle circle2(point a, point b)
{
	point center = 0.5*(a + b);
	double r = abs(a - center);
	return make_pair(r, center);
}
//compute a circle with a,b,c on its boundary
circle circle3(point a, point b, point w)
{
	//center= the middle of the bisector of the angle until the boundary
	double theta=arg(a-w)-arg(b-w);
	double r = abs(b - w) / (2 * cos(theta / 2));
	point center = w+polar(r,theta/2+arg(b-w));
	return make_pair(r, center);
}
//collinear detection
bool collinear(point a, point b, point c)
{
	//cross product ac x ab=0?
	if ((conj(c - a)*(b - a)).imag() < eps)
		return true;
	return false;
}
vector<point> path;

double length(circle q)
{
	double r = q.first;
	point center = q.second;
	double sum = 0;
	point first, second;
	int n = path.size();
	complex<double> f, s, rotator;
	for (int i = 1; i < n; i++)
	{
		first = path[i-1];
		second = path[i];
		first -= center;
		second-= center;
		f = complex<double>(first.real(), first.imag());
		s = complex<double>(second.real(), second.imag());
		rotator = s - f;
		rotator /= abs(rotator);
		rotator = conj(rotator);
		f *= rotator; s *= rotator;
		//intersection(path[i - 1], path[i]), circle
		// segment entirely inside the circle
		if (abs(path[i - 1]-center) < r + eps && abs(path[i]- center) < r + eps)
			sum += abs(path[i - 1]- path[i]);
		else if (abs(path[i - 1]- center) < r + eps && abs(path[i]- center) + eps > r)
		{
			// first in, second out
			if (r*r - f.imag()*f.imag()>0)
				sum += sqrt(r*r - f.imag()*f.imag());
		}
		else if (abs(path[i - 1]- center) + eps > r && abs(path[i]- center) < r + eps)
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
	//cerr << "* " << center.real() << " " << center.imag() << " " << r << " " << sum << endl;
	return sum;
}

int main()
{
	point p;
	while (cin >> p)
	{
		path.push_back(p);
	}
	auto start = chrono::high_resolution_clock::now();
	for (int i = 0; i < path.size(); i++)
	for (int j = i + 1; j < path.size(); j++)
	{
		//same point (radious 0)
		if (abs(path[i] - path[j]) < eps)
			continue;
		circle cur = circle2(path[i], path[j]);
		cout << cur.first << " " << cur.second << " "  << length(cur) << "\n";

		for (int k = j + 1; k < path.size(); k++)
		{
			if (collinear(path[i], path[j], path[k]) == false)
			{
				cur = circle3(path[i], path[j], path[k]);
				cout << cur.first << " " << cur.second << " " << length(cur) << "\n";
			}
		}
	}
	auto finish = chrono::high_resolution_clock::now();
	chrono::duration<double, std::milli> ms_double = finish - start;
	std::cerr << ms_double.count() << " ms\n";

	return 0;
}
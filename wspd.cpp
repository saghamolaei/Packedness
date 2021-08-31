#include <iostream>
#include <vector>
#include <complex>
#include <algorithm>
//#include <thread>
using namespace std;
#define eps 0.000001
#define max(a,b)    (((a+eps) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b+eps)) ? (a) : (b))

//#define EPSILON 1
//#define s ceil(8.0/EPSILON)

#define s 0.001

void f(vector<complex<double> > in, vector<complex<double> > ni, int depth)
{
	double r1 = abs(in[0] - in.back());
	double r2 = abs(ni[0]- ni.back());
	double d = min(abs(in[0]-ni.back()),
					min(abs(in[0]-ni[0]), 
						min(abs(ni[0]-in.back()),abs(ni.back()-in.back()))
					)	);
	cerr << r1 << " " << r2 << " " << d << " " << max(r1, r2)*s + r1 + r2 - d << endl;
	if (max(r1, r2)*s < d + eps)
	{
		//cerr << depth << endl;
		// it is a wspd
		complex<double> pointA, pointB;
		pointA = { min(in[0].real(), min(ni[0].real(), min(in.back().real(), ni.back().real()))), min(in[0].imag(), min(ni[0].imag(), min(in.back().imag(), ni.back().imag()))) };
		pointB = { max(in[0].real(), max(ni[0].real(), max(in.back().real(), ni.back().real()))), max(in[0].imag(), max(ni[0].imag(), max(in.back().imag(), ni.back().imag()))) };
		pair<complex<double>, complex<double> > wspd;
		wspd = make_pair(pointA, pointB);
		if (abs(wspd.first - wspd.second) > eps)
			cout << wspd.first << " " << wspd.second << endl;
		return;
	}
	vector<complex<double> > a[4], b[4];
	double midx1, midy1, midx2, midy2;
	midx1 = (in[0].real() + in.back().real()) / 2;
	midy1 = (in[0].imag() + in.back().imag()) / 2;
	midx2 = (ni[0].real() + ni.back().real()) / 2;
	midy2 = (ni[0].imag() + ni.back().imag()) / 2;
	//[1 3; 0 2]
	for (int i = 0; i < in.size(); i++)
	{
		if (in[i].real() < midx1)
		{
			if (in[i].imag() < midy1)
			{
				a[0].push_back(in[i]);
			}
			else
			{
				a[1].push_back(in[i]);
			}
		}
		else
		{
			if (in[i].imag() < midy1)
			{
				a[2].push_back(in[i]);
			}
			else
			{
				a[3].push_back(in[i]);
			}
		}
	}
	for (int i = 0; i < ni.size(); i++)
	{
		if (ni[i].real() < midx2)
		{
			if (ni[i].imag() < midy2)
			{
				b[0].push_back(ni[i]);
			}
			else
			{
				b[1].push_back(ni[i]);
			}
		}
		else
		{
			if (ni[i].imag() < midy2)
			{
				b[2].push_back(ni[i]);
			}
			else
			{
				b[3].push_back(ni[i]);
			}
		}
	}
	//vector<thread> threads;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		if (a[i].size()>0 && b[j].size()>0)
			f(a[i], b[j],depth+1);
	//			threads.push_back(thread (f,a[i], b[j]));
	//for (auto& th : threads) th.join();
	return;
}

bool comp(complex<double> A, complex<double> B)
{
	if (fabs(A.real() - B.real()) < eps && fabs(A.imag() - B.imag()) < eps) // A=B
		return false;
	if (fabs(A.real() - B.real()) < eps)
	{
		if (A.imag() < B.imag() + eps)
			return true;
		else
			return false;
	}
	if (A.real() < B.real() + eps)
		return true;
	return false;
}

int main()
{
	vector<complex<double> > points;
	complex<double> point;

	while (cin >> point)
	{
		points.push_back(point);
	}
	sort(points.begin(), points.end(), comp);

	f(points, points,0);

	/*for (int i = 0; i < wspd.size(); i++)
	{
		if (abs(wspd[i].first - wspd[i].second) > eps)
			cout << wspd[i].first << " " << wspd[i].second << endl;
	}*/

	return 0;
}
#include <iostream>
#include <algorithm>
#include <fstream>
#include <complex>
#include <vector>
using namespace std;

#define point complex<double>
#define segment pair<complex<double>,complex<double> >
#define eps 1e-12
//1+epsilon factor:
#define steps 50
#define sample 50

double minx, maxx, miny, maxy;

double Distance(point a)
{
	double ratio = (maxy - miny) / (maxx - minx);
	return max(ratio*fabs(a.real()) , fabs(a.imag()));
}

point inrange(point a)
{
	a = point(max(minx, min(a.real(), maxx)), max(miny, min(a.imag(), maxy)));
	return a;
}

segment query(point center, double r)
{
	double ratio = fabs((maxy-miny)/(maxx - minx));
	point a, b;
	a = center - point(r, r*ratio);
	b = center + point(r, r*ratio);
	return make_pair(inrange(a), inrange(b));
}

bool comp(point a, point b)
{
	pair<double, double> aa(a.real(), a.imag());
	pair<double, double> bb(b.real(), b.imag());
	return aa < bb;
}

bool compf(double a, double b)
{
	if (a < b && fabs(a-b)>eps)
		return true;
	return false;
}

bool compx(point a, point b)
{
	return compf(a.real(), b.real());
}

bool compy(point a, point b)
{
	return compf(a.imag() , b.imag());
}

bool compp(point a, point b)
{
	if (compx(a, b) && !compx(b,a))
		return true;
	if (!compx(a, b) && !compx(b, a) && compy(a, b) && !compy(b,a))
		return true;
	return false;
}

bool comps(segment a, segment b)
{
	if (compp(a.first, b.first) && !compp(b.first, a.first))
		return true;
	if (!compp(a.first, b.first) && !compp(b.first, a.first) && compp(a.second, b.second) && !compp(b.second, a.second))
		return true;
	return false;
}

bool eqp(point a, point b)
{
	if (abs(a - b) < eps)
		return true;
	return false;
}

bool eqs(segment a, segment b)
{
	if (eqp(a.first, b.first) && eqp(a.second, b.second))
		return true;
	return false;
}

int main()
{
	vector<point> xs, ys;
	vector<segment> queries;
	point temp;
	while (cin >> temp)
	{
		xs.push_back(temp);
		ys.push_back(point(temp.imag(), temp.real()));
	}
	sort(xs.begin(), xs.end(),comp);
	sort(ys.begin(), ys.end(),comp);
	minx = xs[0].real();
	maxx = xs.back().real();
	miny = ys[0].real();
	maxy = ys.back().real();
	for (int i = 0; i < xs.size(); i+=sample)
	{
		//initialize
		int ind = lower_bound(ys.begin(), ys.end(), xs[i].imag(), comp) - ys.begin();
		if (ind == ys.size())
			continue;
		double r_min=1;
		bool assigned = false;
		if (i>0 && (assigned == false || r_min > Distance(xs[i - 1] - xs[i])))
			r_min = Distance(xs[i - 1] - xs[i]);
		if (i<xs.size() - 1 && (assigned == false || r_min > Distance(xs[i + 1] - xs[i])))
			r_min = Distance(xs[i + 1] - xs[i]);
		if (ind > 0 && (assigned == false || r_min > Distance(ys[ind] - ys[ind - 1])))
			r_min = Distance(ys[ind] - ys[ind - 1]);
		if (ind<ys.size() - 1 && (assigned == false || r_min > Distance(ys[ind + 1] - ys[ind])))
			r_min = Distance(ys[ind + 1] - ys[ind]);
		//find radii
		double r = r_min;
		for (int j = i + 1; j < xs.size();r*=steps)
		{
			queries.push_back(query(xs[i], Distance(xs[i] - xs[j])));
			j = max(j+1,lower_bound(xs.begin()+i, xs.end(), point(r,0)+xs[i], compx) - xs.begin());
		}
		r = r_min;
		for (int j = i-1; j >=0; r *= steps)
		{
			queries.push_back(query(xs[i], Distance(xs[i] - xs[j])));
			j = min(j-1,lower_bound(xs.begin(), xs.begin()+i, xs[i]-point(r,0), compx) - xs.begin());
		}
		r = r_min;
		for (int j = ind + 1; j < ys.size(); r *= steps)
		{
			queries.push_back(query(xs[i], Distance(ys[ind] - ys[j])));
			j = max(j+1,lower_bound(ys.begin() + ind, ys.end(), ys[ind]+point(r,0), compx) - ys.begin());
		}
		r = r_min;
		for (int j = ind-1; j >=0; r *= steps)
		{
			queries.push_back(query(xs[i], Distance(ys[ind]-ys[j])));
			j = min(j-1,lower_bound(ys.begin() , ys.begin()+ind, ys[ind]-point(r,0), compx) - ys.begin());
		}
	}
	sort(queries.begin(), queries.end(), comps);
	queries.resize(unique(queries.begin(), queries.end(), eqs)-queries.begin());
	for (int i = 0; i < queries.size(); i++)
	{
		cout << queries[i].first << " " << queries[i].second << endl;
	}

	return 0;
}
#include <iostream>
#include <algorithm>
#include <fstream>
#include <complex>
#include <vector>
#include <chrono>
using namespace std;

#define point complex<double>
#define segment pair<complex<double>,complex<double> >
#define eps 1e-10

bool between(double x, double y, double q)
{
	if (min(x, y) < q+eps && q < max(x, y)+eps)
		return true;
	return false;
}

int intersect(segment query, point a, point b)
{
	//a & b inside the query
	if (between(query.first.real(), query.second.real(), a.real()
		&& between(query.first.imag(), query.second.imag(), a.imag()))
		&& between(query.first.real(), query.second.real(), b.real()
		&& between(query.first.imag(), query.second.imag(), b.imag())))
		return 3;
	//a inside the query
	if (between(query.first.real(), query.second.real(), a.real()
		&& between(query.first.imag(), query.second.imag(), a.imag())))
		return 2;
	// b inside the query
	if (between(query.first.real(), query.second.real(), b.real()
		&& between(query.first.imag(), query.second.imag(), b.imag())))
		return 1;
	//query is inside ab
	if (
		(between(a.real(), b.real(), query.first.real()) &&
		between(a.real(), b.real(), query.second.real()) &&
		between(a.imag(), b.imag(), query.first.imag())) ||
		(between(a.real(), b.real(), query.first.real()) &&
		between(a.real(), b.real(), query.second.real()) &&
		between(a.imag(), b.imag(), query.second.imag())) ||
		(between(a.real(), b.real(), query.first.real()) &&
		between(a.imag(), b.imag(), query.first.imag()) &&
		between(a.imag(), b.imag(), query.second.imag())) ||
		(between(a.real(), b.real(), query.second.real()) &&
		between(a.imag(), b.imag(), query.first.imag()) &&
		between(a.imag(), b.imag(), query.second.imag()))
		)
		return 4;
	return 0;
}

double length(double x, double y, double x2, double y2, segment query)
{
	if (fabs(x2 - x) < eps)
	{
		//query inside
		if (between(y, y2, query.first.imag()) && between(y, y2, query.second.imag()) &&
			between(query.first.real(), query.second.real(), x))
			return fabs(query.second.imag()-query.first.imag());
		//segment inside
		if (between(query.first.imag(), query.second.imag(), y) && between(query.first.imag(), query.second.imag(), y2) &&
			between(query.first.real(), query.second.real(), x))
			return fabs(y2-y);
		//intersect query
		if ((between(y, y2, query.first.imag()) || between(y, y2, query.second.imag())) &&
			between(query.first.real(), query.second.real(), x))
			return fabs(max(min(y, y2), query.first.imag()) - min(max(y, y2), query.second.imag()));
		else
			return 0;
	}
	if (fabs(y2 - y) < eps)
	{
		//query inside
		if (between(x, x2, query.first.real()) && between(x, x2, query.second.real()) &&
			between(query.first.imag(), query.second.imag(), y))
			return fabs(query.second.real() - query.first.real());
		//segment inside
		if (between(query.first.real(), query.second.real(), x) && between(query.first.real(), query.second.real(), x2) &&
			between(query.first.imag(), query.second.imag(), y))
			return fabs(x2 - x);
		//intersect query
		if ((between(x, x2, query.first.real()) || between(x, x2, query.second.real())) &&
			between(query.first.imag(),query.second.imag(),y))
			return fabs(max(min(x, x2), query.first.real()) - min(max(x, x2), query.second.real()));
		else
			return 0;
	}
	double m;
	m= (y2 - y) / (x2 - x);
	switch (intersect(query, point(x, y), point(x2, y2)))
	{
	case 0:
		return 0;
		break;
	case 1:
		if (m > 0)
			return min(fabs(x2-query.first.real())*sqrt(1+m*m),
			fabs(y2 - query.first.imag())*sqrt(1 + m*m)/m);
		else
			return min(fabs(x2 - query.first.real())*sqrt(1 + m*m), 
			fabs(y2 - query.second.imag())*sqrt(1 + m*m) / fabs(m));
		break;
	case 2:
		if (m > 0)
			return min(fabs(x - query.second.real())*sqrt(1 + m*m),
			fabs(y - query.second.imag())*sqrt(1 + m*m) / m);
		else
			return min(fabs(x - query.second.real())*sqrt(1 + m*m),
			fabs(y - query.first.imag())*sqrt(1 + m*m) / fabs(m));
		break;
	case 3:
		return abs(point(x, y) - point(x2, y2));
		break;
	case 4:
		return min(fabs(min(max(query.first.real(),query.second.real()),max(x,x2)) - max(min(query.first.real(),query.second.real()),min(x,x2))*sqrt(1 + m*m)),
			fabs(min(max(query.second.imag(),query.first.imag()),max(y,y2))-max(min(query.first.imag(),query.second.imag()),min(y,y2))*sqrt(1+m*m)/fabs(m)));
		break;
	}
	//cerr << point(x,y) << " " << point(x2,y2) << " " << query.first << " " << query.second << endl;
}

int main()
{
	vector<segment> queries;
	ifstream fqueries("queries.txt");
	point p1, p2;
	segment q;
	while (fqueries >> p1 >> p2)
	{
		q.first = point(min(p1.real(), p2.real()), min(p1.imag(), p2.imag()));
		q.second = point(max(p1.real(), p2.real()), max(p1.imag(), p2.imag()));
		queries.push_back(q);
	}
	fqueries.close();
	auto start = chrono::high_resolution_clock::now();
	vector<double > mp(queries.size(),0);
	ifstream inputs("input.txt");
	double x, y, x2, y2;
	point temp;
	inputs >> temp;
	x = temp.real();
	y = temp.imag();
	while (inputs >> temp)
	{
		x2 = temp.real();
		y2 = temp.imag();
		for (int i = 0; i < queries.size(); i++)
		{
			if (fabs(x - x2) < eps)
				mp[i] += length(x, min(y, y2), x2, max(y, y2), queries[i]);
			else if (fabs(y - y2) < eps)
				mp[i] += length(min(x, x2), y, max(x, x2), y2, queries[i]);
			else
			if ((x < x2 + eps && fabs(x-x2)>eps) || (fabs(x-x2)<eps && y < y2+eps))
				mp[i] += length(x, y, x2, y2, queries[i]);
			else
				mp[i] += length(x2, y2, x, y, queries[i]);
			//cerr << mp[i] << endl;
		}
		x = x2;
		y = y2;
	}
	inputs.close();
	auto finish = chrono::high_resolution_clock::now();
	chrono::duration<double, std::milli> ms_double=finish - start;
	std::cerr << ms_double.count() << " ms\n";

	for (int i = 0; i < queries.size(); i++)
	{
		cout << queries[i].first << " " << queries[i].second << " " << mp[i] << " ";
		cout << mp[i] /((fabs(queries[i].second.imag() - queries[i].first.imag())
			+ fabs(queries[i].second.real() - queries[i].first.real()) )/ 2) << endl;
	}


	return 0;
}
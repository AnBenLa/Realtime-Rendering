// author: Anton Lammert
// authot: Philipp Tornow 118332

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>

std::string INPUT_FORMAT = 	"Example values:\n" \
							"3\n" \
							"0 1\n" \
							"1 0\n" \
							"1 1\n" \
							"for the points (0,1), (1,0), (1,1) as one problem instance.\n" \
							"You can create multiple instances and quit with 'quit' .\n";

struct point2D{
	double x, y, z;
};

struct segment2D{
	point2D a, b;
};

struct vec3{
	double x, y, z;	
};

vec3 cross(point2D const& p1, point2D const& p2)
{
	vec3 v{0.0,0.0,0.0};
	v.x = p1.y*p2.z-p1.z*p2.y;
	v.y = p1.z*p2.x-p1.x*p2.z;
	v.z = p1.x*p2.y-p1.y*p2.x;
	return v;
}

vec3 cross(vec3 const& v1, vec3 const& v2)
{
	vec3 v{0.0,0.0,0.0};
	v.x = v1.y*v2.z-v1.z*v2.y;
	v.y = v1.z*v2.x-v1.x*v2.z;
	v.z = v1.x*v2.y-v1.y*v2.x;
	return v;
}

bool operator ==(point2D const& p1,point2D const& p2)
{
	return (p1.x == p2.x && p1.y == p2.y && p1.z == p2.z);
}

int ORI(point2D const& a, point2D const& b, point2D const& c){
	double dx21 = b.x-a.x;
	double dy21 = b.y-a.y;
	double dx31 = c.x-a.x;
	double dy31 = c.y-a.y;
	
	if (dx21*dy31 < dy21*dx31)
		return -1;//ccw

	if(dx21*dy31 > dy21*dx31)
		return +1;//cw

	if(dx21*dx31 < 0 || dy21*dy31<0)
		return -1;//ccw

	if(dx31*dx31 + dy31*dy31 > dx21*dx21 + dy21*dy21)
		return +1; //cw

	return 0;//collinear 
}

float distanceFromSegment(segment2D const& segment, point2D const& point)
{
	float divident = (segment.b.x-segment.a.x)*(segment.a.y-point.y)-(segment.a.x-point.x)*(segment.b.y-segment.a.y);
	float divisor = std::pow((segment.b.x-segment.a.x),2)+std::pow((segment.b.y-segment.a.y),2);
	return std::fabs(divident)/std::sqrt(divisor);
}

void merge(std::vector<segment2D>& target, std::vector<segment2D> const& to_merge)
{
	//temporary wrong solution
	target.insert(target.end(), to_merge.begin(),to_merge.end());
}

std::vector<segment2D> hull(point2D const& a, point2D const& b, std::vector<point2D> const& points)
{
	if(points.empty())
	{
		return std::vector<segment2D>{{a,b}};
	}
	std::vector<point2D> left_most;

	for (point2D p : points)
	{
		if(ORI(a,b,p)==1)
		{
		 left_most.push_back(p);
		}
	}
		
	if (left_most.empty())
	{
		return std::vector<segment2D>{{a,b}};
	}

	std::sort(left_most.begin(),left_most.end(),[a,b](point2D const& p1, point2D const& p2){
		return distanceFromSegment({a,b},p1)>distanceFromSegment({a,b},p2);
	});

	point2D p_far = *left_most.begin();

	std::vector<segment2D>subhull = hull(a,p_far,left_most);
	std::vector<segment2D>mergehull = hull(p_far,b,left_most);
	merge(subhull,mergehull);
	return subhull;
}


int main(int argc, char const *argv[])
{

	uint number_of_points = 0;
	std::vector<point2D> points;
	std::vector<point2D> convex_hull;

	if(argc > 1)
	{
		if(std::string(argv[1]).compare("-h")==0){
			std::cout<<"executable needs no arguments!\n\n";
			std::cout<<"use -f option for file input";
			std::cout<<"excercise21 -f <input.txt>\n\n";
			std::cout<<INPUT_FORMAT;
			return(0);
		}

		else if (std::string(argv[1]).compare("-f")==0){

			try
			{
				std::ifstream file(argv[2]);
				file>>number_of_points;
				if(number_of_points<3)
				{
					std::cout<<"\nNot enough points given!";
					return 0;
				}
				double x,y = 0;
				uint inputCount = 0;
				while(inputCount<number_of_points&&file>>x>>y)
				{
					points.push_back(point2D{x,y,1});
					inputCount++;
				}
				file.close();
			}
			catch(const std::ifstream::failure& e)
			{
				std::cout<<"Can't open file.";
			}
		}

		else{
			std::cout<<"Executable needs no arguments to start.";
			std::cout<<"Use exercise11.cpp -h to get help";
			return(0);
		}
	}
	else
	{
		std::cin>>number_of_points;
		if(number_of_points<3)
		{
			std::cout<<"\nNot enough points given!";
			return 0;
		}

		double x,y = 0;
		uint inputCount = 0;
		while(inputCount<number_of_points&&std::cin>>x>>y)
		{
			points.push_back(point2D{x,y,1});
			inputCount++;
		}
	}

	point2D left_most{0,0};
	point2D right_most{0,0};
	point2D top_most{0,0};
	point2D bottom_most{0,0};

	/*
	std::sort(points.begin(),points.end(),[](point2D const& a, point2D const& b){
		return a.x<b.x;
	});
	left_most = *points.begin();
	right_most = *points.end()--;

	std::sort(points.begin(),points.end(),[](point2D const& a, point2D const& b){
		return a.y<b.y;
	});
	bottom_most = *points.begin();
	top_most = *points.end()--;
	*/

	//find extreme points
	for(point2D const& p : points)
	{
		if(p.x < left_most.x)
		{
		 left_most = p;
		}
		else
		{
			if(p.x>right_most.x)
			{
				right_most = p;
			}
		}

		if(p.y < bottom_most.y)
		{
			bottom_most = p;
		}
		else
		{
			if(p.y > top_most.y)
			{
			 top_most = p;
			}
		}
	}

	//merge result subsets together
	std::vector<segment2D> top_right = hull(top_most,right_most,points);
	std::vector<segment2D> bottom_right = hull(right_most,bottom_most,points);
	std::vector<segment2D> bottom_left = hull(bottom_most,left_most,points);
	std::vector<segment2D> top_left = hull(left_most,top_most,points);
	std::vector<segment2D> result_hull;
	merge(result_hull,top_right);
	merge(result_hull,bottom_right);
	merge(result_hull,bottom_left);
	merge(result_hull,top_left);

	//output the convex hull points
	for(segment2D const& seg : result_hull)
	{
		std::cout<<seg.a.x<<" "<<seg.a.y<<"\n";
	}

	while(true)
	{

	}
	return 0;
}
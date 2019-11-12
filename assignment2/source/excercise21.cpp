// author: Anton Lammert
// authot: Philipp Tornow 118332

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

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
	return 0;
}

segment2D quickhull(segment2D const& segment, std::vector<point2D> points)
{
	if(points.empty())
	{
		return segment;
	}
	std::vector<points> left;
	for (point2D p : points)
	{
		if(segment.a,segment.b,p)
		{
			left.push_back(p);
		}
		
		if(left.empty())
		{
			return segment;
		}

		point2D p_far = left[0];
		for (point2D p : left)
		{
			if(p_far>distanceFromSegment(segment,p))
			{
				p_far = p;
			}
		}
	}
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

	std::cout<<points.size();

	for (point2D p : points)
	{
		std::cout<<"\n"<<p.x<<" "<<p.y;
	}
	return 0;
}
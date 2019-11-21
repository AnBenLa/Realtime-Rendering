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

bool operator == (point2D const& p1,point2D const& p2)
{
	return (p1.x == p2.x && p1.y == p2.y && p1.z == p2.z);
}

//returns -1 for ccw, +1 for cw and 0 if b is collinear with the line through a and c
int ORI(point2D const& a, point2D const& b, point2D const& c){
    double dx21 = b.x - a.x;
    double dy21 = b.y - a.y;
    double dx31 = c.x - a.x;
    double dy31 = c.y - a.y;
    
    if(dy31*dx21 > dx31*dy21)
        return - 1;
    if(dy31*dx21 < dx31*dy21)
        return +1;
    return 0;
}

//returns the perpendicular distance of a point to segment
float distanceFromSegment(segment2D const& segment, point2D const& point)
{
	//uses the formula ax + by + c = 0
	float divident = (segment.b.x-segment.a.x)*(segment.a.y-point.y)-(segment.a.x-point.x)*(segment.b.y-segment.a.y);
	float divisor = std::pow((segment.b.x-segment.a.x),2)+std::pow((segment.b.y-segment.a.y),2);
	return std::fabs(divident)/std::sqrt(divisor);
}

//recursive hull function
std::vector<point2D> hull(point2D const& a, point2D const& b, std::vector<point2D> const& points)
{
	//check if two extreme points are the same
	if(a == b)
	{
		return {};
	}

	if(points.empty())
	{
		return std::vector<point2D>{b};
	}
	std::vector<point2D> left;

	//search for left points
	for (point2D p : points)
	{
		if(ORI(a,b,p)==-1)
		{
		 	left.push_back(p);
		}
	}
		
	if (left.empty())
	{
		return std::vector<point2D>{b};
	}

	std::sort(left.begin(),left.end(),[a,b](point2D const& p1, point2D const& p2){
		return distanceFromSegment({a,b},p1)>distanceFromSegment({a,b},p2);
	});

	point2D p_far = *left.begin();
	left.erase(left.begin());

	//recursive call for segments a-c and c-b 
	std::vector<point2D>subhull = hull(a,p_far,left);
	std::vector<point2D>mergehull = hull(p_far,b,left);
	subhull.insert(subhull.end(),mergehull.begin(),mergehull.end());
	return subhull;
}

std::vector<point2D> quickHull(std::vector<point2D> const& points_input)
{
	std::vector<point2D> points = points_input;
	std::vector<point2D> result_hull;

	point2D left_most{0,0,1};
	point2D right_most{0,0,1};
	point2D top_most{0,0,1};
	point2D bottom_most{0,0,1};

	if(points.size()<3)
	{
		std::cout<<"\nNot enough points given!";				
		return result_hull;
	}

	//sort horizontal
	std::sort(points.begin(),points.end(),[](point2D const& a, point2D const& b){
			if(a.x==b.x)
			{
				return a.y < b.y;
			}
			return a.x < b.x;
		});

	left_most = *points.begin();
	right_most = *(points.end()-1);

	//special case 3 points
	if(points.size()==3)
	{
		point2D mid = *(points.begin()+1);
		int ori = ORI(left_most,right_most,mid);
		switch (ori)
		{
		    //TODO ist die result hull wirklich leer wenn alle punkte coolinear sind?
			case 0:
			{
				return result_hull;
			}
			case 1:
			{
				result_hull = {left_most,right_most,mid};
				return result_hull;
			}
			case -1:
			{
				result_hull = {left_most,mid,right_most};
				return result_hull;
			}
		}
	}
	//TODO wieso nochmal aufrufen?
	left_most = *points.begin();
	right_most = *(points.end()-1);

	//sort vertical
	std::sort(points.begin(),points.end(),[](point2D const& a, point2D const& b){
		if(a.y == b.y){
			return a.x<b.x;
		}
		return a.y < b.y;
	});
	bottom_most = *points.begin();
	top_most = *(points.end()-1);

	//calculate all 4 subhulls
	std::vector<point2D> top_left_hull = hull(left_most,top_most,points);
	result_hull.insert(result_hull.end(), top_left_hull.begin(),top_left_hull.end());

	std::vector<point2D> top_right_hull = hull(top_most,right_most,points);
	result_hull.insert(result_hull.end(), top_right_hull.begin(),top_right_hull.end());
	
	std::vector<point2D> bottom_right_hull = hull(right_most,bottom_most,points);
	result_hull.insert(result_hull.end(), bottom_right_hull.begin(),bottom_right_hull.end());
	
	std::vector<point2D> bottom_left_hull = hull(bottom_most,left_most,points);
	result_hull.insert(result_hull.end(), bottom_left_hull.begin(),bottom_left_hull.end());


	//sort to start with the bottom left 
	point2D bottom_left_most = result_hull[0];
	uint position_bottom_left_most = 0;

	for (uint i = 0; i<result_hull.size(); i++)
	{
		if(result_hull[i].x<=bottom_left_most.x&&result_hull[i].y<=bottom_left_most.y)
		{
			bottom_left_most = result_hull[i];
			position_bottom_left_most = i;
		}
	}

	uint result_hull_size = result_hull.size();

	for(uint j = 0; j < result_hull_size-position_bottom_left_most; j++)
	{
		result_hull.insert(result_hull.begin(),*(result_hull.end()-1));
		result_hull.pop_back();
	}

	return result_hull;
}


int main(int argc, char const *argv[])
{
    //TODO was ist wenn alle Punkte identisch sind???
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

				double x,y = 0;
				uint inputCount = 0;
				while(inputCount<number_of_points&&file>>x>>y)
				{
					if((x<-350||x>350)||(y<-350||y>350))
					{
						file.close();
						std::cout<<"\npoint in line "<<inputCount+1<<" is invalid!";
						return 0;
					}
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

		double x,y = 0;
		uint inputCount = 0;
		while(inputCount<number_of_points&&std::cin>>x>>y)
		{
			if((x<-350||x>350)||(y<-350||y>350))
			{
				std::cout<<"\npoint "<<inputCount+1<<" is invalid!";
				return 0;
			}
			points.push_back(point2D{x,y,1});
			inputCount++;
		}
	}

	std::vector<point2D> result_hull = quickHull(points);

	std::cout<<result_hull.size()<<"\n";

	//output the convex hull points
	for(auto const& point : result_hull)
	{
		std::cout<<point.x<<" "<<point.y<<"\n";
	}
}
// author: Anton Lammert
// authot: Philipp Tornow 118332

#include <iostream>
#include <string>
#include <vector>

std::string INPUT_FORMAT = 	"Enter values: 0 0 0 1 1 0 1 1 for the vec4 (0 0 0 1) and vec4 (1 0 1 1) as one problem instance.\n " 
							"You can press ENTER for multiple instances and end your input with a 0 0 0 0 0 0 0 0 instance\n";

struct point2D
{
	double x, y, z=1.0;
};

struct segment2D
{
	point2D a, b;
};

struct vec2
{
	double x, y, z;	
};

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

vec2 cross(point2D const& p1, point2D const& p2)
{
	vec2 v{0.0,0.0,0.0};
	v.x = p1.y*p1.z-p1.z*p2.y;
	v.y = p1.z*p2.x-p1.x-p2.z;
	v.z = p1.x*p2.y-p1.y*p2.x;
	return v;
}

vec2 cross(vec2 const& v1, vec2 const& v2)
{
	vec2 v{0.0,0.0,0.0};
	v.x = v1.y*v1.z-v1.z*v2.y;
	v.y = v1.z*v2.x-v1.x-v2.z;
	v.z = v1.x*v2.y-v1.y*v2.x;
	return v;
}

int main(int argc, char const *argv[])
{
	if(argc > 1 && std::string(argv[1]).compare("-h")==1)
	{
		std::cout<<"Executable needs no arguments to start.";
		return(0);
	}
	else
	{
		std::cout<<"Type "<<argv[0]<<" -h to get help for input format.\n\n";

		while (true){
			double x_0,x_1,x_2,x_3,y_0,y_1,y_2,y_3 = 0.0;
			std::cin>>x_0>>y_0>>x_1>>y_1>>x_2>>y_2>>x_3>>y_3;

			if(x_0 == 0&&y_0 == 0&&x_1 == 0&&y_1 == 0&&x_2 == 0&&y_2 == 0&&x_3 == 0&&y_3 == 0)
			{
				break;
			}

			//init data
			point2D p1{x_0,y_0};
			point2D p2{x_1,y_1};
			segment2D seg1{p1,p2};
			segment2D seg2{{x_2,y_2},{x_3,y_3}};

			// check if they are collinear
			if((ORI(seg1.a, seg1.b, seg2.a) == 0 && ORI(seg1.a,seg1.b,seg2.b) == 0)||(ORI(seg2.a,seg2.b,seg1.a) == 0 && ORI(seg2.a,seg2.b,seg1.b)==0))
			{
				std::cout<<"segment intersection";
			}
			else
			{
				// check if they intersect
				if(ORI(seg1.a, seg1.b, seg2.a)*ORI(seg1.a,seg1.b,seg2.b)<=0 && ORI(seg2.a,seg2.b,seg1.a)*ORI(seg2.a,seg2.b,seg1.b)<=0)
				{
					vec2 line1 = cross(seg1.a,seg2.b);
					vec2 line2 = cross(seg2.a,seg2.b);
					vec2 intersection_vector = cross(line1,line2);
					if(intersection_vector.z == 0){
						std::cout<<"no intersection";
					}
					else{
						point2D intersection{intersection_vector.x/intersection_vector.z,intersection_vector.y/intersection_vector.z};
						std::cout<<intersection.x<<" "<<intersection.y<<"\n";
					}
				}
				else
				{
					std::cout<<"no intersection";
				}
			}
		}
		return(0);
	}
}
// author: Anton Lammert
// authot: Philipp Tornow 118332

#include <iostream>
#include <string>
#include <vector>

std::string INPUT_FORMAT = 	"Example values: 0 0 0 1 1 0 1 1 for the points (0,0), (0,1), (0,0), (0,1) as one problem instance.\n " 
							"You can create multiple instances and quit with an 0 0 0 0 0 0 0 0 instance.\n";

struct point2D{
	double x, y, z;
};

struct segment2D{
	point2D a, b;
};

struct vec3{
	double x, y, z;	
};

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

int intersect(segment2D const& seg1, segment2D const& seg2)
{
	if(ORI(seg1.a, seg1.b, seg2.a)*ORI(seg1.a,seg1.b,seg2.b)<=0 && ORI(seg2.a,seg2.b,seg1.a)*ORI(seg2.a,seg2.b,seg1.b)<=0)
	{
		return 1; //point intersection
	}

	if((ORI(seg1.a, seg1.b, seg2.a) == 0 && ORI(seg1.a,seg1.b,seg2.b) == 0)||(ORI(seg2.a,seg2.b,seg1.a) == 0 && ORI(seg2.a,seg2.b,seg1.b) == 0))
	{
		return -1; //one segment lies collinear beween the points of the other segment 
	}

	if((ORI(seg1.a, seg1.b, seg2.a) == 0 && ORI(seg1.a, seg1.b, seg2.b) != 0) || (ORI(seg2.a, seg2.b, seg1.a) == 0 && ORI(seg2.a, seg2.b, seg1.b)))
	{
		return -1; // one segment lies partially in the other
	}

	return 0; // no intersection
}

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

bool isPoint(segment2D seg)
{
	vec3 result = cross(seg.a,seg.b);
	return (result.x == 0 && result.y == 0 && result.z == 0);
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
		std::cout<<"Use exercise11.cpp -h to get help for input format.\n\n";

		while (true){
			double x_0,x_1,x_2,x_3,y_0,y_1,y_2,y_3 = 0.0;
			std::cin>>x_0>>y_0>>x_1>>y_1>>x_2>>y_2>>x_3>>y_3;

			if(x_0 == 0&&y_0 == 0&&x_1 == 0&&y_1 == 0&&x_2 == 0&&y_2 == 0&&x_3 == 0&&y_3 == 0)
			{
				break;
			}

			//init data
			point2D p1{x_0,y_0,1.0};
			//p1.x = x_0;
			//p1.y = y_0;
			point2D p2{x_1,y_1,1.0};
			//p2.x= x_1;
			//p2.y = y_1;

			segment2D seg1{p1,p2};

			point2D p3{x_2,y_2,1.0};
			//p3.x = x_2
			//p3.y = y_2;
			point2D p4{x_3,y_3,1.0};
			//p4.x = x_3;
			//p4.y = y_3;

			segment2D seg2{p3,p4};

			//check if segment is only one point
			if(isPoint(seg1))
			{
				//check if other segment is only one point
				if(isPoint(seg2))
				{
					//check if all 4 points have the same position
					if (p1 == p3)
					{
						std::printf("%.2f", p1.x);
						std::printf(" %.2f\n\n",p1.y);
					}
					else
					{
						std::cout<<"no intersection\n\n";
					}
				}
				else
				{
					//check if point/seg 1 lay on the segment 2
					if(ORI(seg2.a,seg2.b,p1) == 0)
					{
						std::printf("%.2f", p1.x);
						std::printf(" %.2f\n\n", p1.y);
					}
					else
					{
						std::cout<<"no intersection\n\n";
					}
				}
			}
			else
			{
				//check if then the other segment is only one point
				if(isPoint(seg2))
				{
					//check if point/seg 2 lay on the segment 1
					if(ORI(seg1.a,seg1.b,p3) == 0)
					{
						std::printf("%.2f", p3.x);
						std::printf(" %.2f\n\n", p3.y);
					}
					else
					{
						std::cout<<"no intersection\n\n";
					}
				}
				else
				{
					int intersection = intersect(seg1, seg2);

					//check if both segments are colinear
					if(intersection<0)
					{
						std::cout<<"segment intersection\n\n";
					}
					else
					{
						if(intersection>0)
						{
							// get line representation in implicit form (ax + bx +c = 0)
							vec3 line1 = cross(seg1.a,seg1.b);
							vec3 line2 = cross(seg2.a,seg2.b);

							// use line x line = intersection
							vec3 intersection_vector = cross(line1,line2);

							//check if the lines are paralell
							if(intersection_vector.z == 0){
								std::cout<<"no intersection\n\n";
							}
							else{
								point2D intersection;
								intersection.x = intersection_vector.x/intersection_vector.z;
								intersection.y = intersection_vector.y/intersection_vector.z;
								intersection.z = 1.0;
								std::printf("%.2f",intersection.x);
								std::printf(" %.2f\n\n",intersection.y);
								std::cout<<"\n";
							}
						}
						else
						{
							std::cout<<"no intersection\n\n";
						}

					}

				}
			}
		}
		return(0);
	}
}
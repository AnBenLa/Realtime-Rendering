//
// author: Anton Lammert 118592
// authot: Philipp Tornow 118332
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

struct point{
    double x,y;
};

struct triangle{
    point p1;
    point p2;
    point p3;    
};

struct circle{
    point center;
    double radius;
};

bool operator == (point const &a, point const& b) {
    return (abs(a.x - b.x) < 0.0001f) && (abs(a.y-b.y) < 0.0001f);
}

circle calculateCircle(point const& A, point const& B, point const& C) {

    /////////////// calculate circle center //////////////
    point center;
    
    //construct two lines with the given points (A,B) and (B,C)
    double deltaX_a = B.x-A.x;
    double deltaY_a = B.y-A.y;
    double deltaX_b = C.x-B.x;
    double deltaY_b = C.y-B.y;

    double AB_mid_y = (A.y+B.y)/2;
    double BC_mid_y = (B.y+C.y)/2;
    double AB_mid_x = (A.x+B.x)/2;
    double BC_mid_x = (B.x+C.x)/2;

    //use the idea that the intersection of two lines, perpendicular to these lines and passing through the midpoints, is the center of the searched circle

    //except vertical lines
    if(deltaX_a==0) { 
        center.y = AB_mid_y;
        if(deltaY_b==0) {
            center.x = BC_mid_x;
        }
        else {
            double mb = deltaY_b/deltaX_b;
            center.x = mb*(BC_mid_y-center.y)+BC_mid_x;
        }
    }
    else if(deltaX_b==0) {
        center.y = BC_mid_y; //check point in circle
        if(deltaY_a==0) {
            center.x = AB_mid_x;
        }
        else {
            double ma = deltaY_a/deltaX_a;
            center.x = ma*(AB_mid_y-center.y)+AB_mid_x;
        }
    }

    //except horizontal lines
    else if(deltaY_a==0) {
        center.x = AB_mid_x;
        double mb = deltaY_b/deltaX_b; 
        center.y = (BC_mid_x-center.x)/mb + BC_mid_y;
    }
    else if(deltaY_b==0){
        center.x = BC_mid_x;
        double ma = deltaY_a/deltaX_a;
        center.y = (AB_mid_x-center.x)/ma + AB_mid_y;
    }
    else {
        //set the equations for the perpendiculars equal
        //solve equation for x
        double ma = deltaY_a/deltaX_a;
        double mb = deltaY_b/deltaX_b; 

        center.x = (ma*mb*(AB_mid_y-BC_mid_y)+mb*AB_mid_x - ma*BC_mid_x)/(mb-ma);
        center.y = AB_mid_y - (center.x - AB_mid_x)/ma;
    }   

    /////////////// calculate radius //////////////
    double radius = sqrt(pow(center.x-A.x,2)+pow(center.y-A.y,2));

    return circle{center,radius};
}

bool checkCircleCriterion(vector<triangle> const& triangles, vector<point> const& points) {

    for(triangle t : triangles) {

        //calculate circle
        circle circle = calculateCircle(t.p1,t.p2,t.p3);
        for(point p : points) {

            //check if point is part of the triangle 
            if(p==t.p1||p==t.p2||p==t.p3)
                continue;
            else
            {
                //use circle equation to check if point is inside
                if(pow(p.x-circle.center.x,2)+pow(p.y-circle.center.y,2) < floor(pow(circle.radius,2)))
                    return false;
            }
        }
    }
    return true;
}

int main() {
    ifstream cin ("./input-3.txt");
    int num_points, num_triangle_indices;
    double x_tmp, y_tmp;

    cin >> num_points >> num_triangle_indices;
    vector<point> points;
    vector<triangle> triangles;

    if(num_triangle_indices/3 > 1000) {
        std::cout<<"NO";
        return 0;
    }

    while(num_points--){
        cin >> x_tmp >> y_tmp;
        points.push_back(point{x_tmp, y_tmp});
    }
    while(num_triangle_indices--){
        int p1,p2,p3;
        cin >> p1 >> p2 >> p3;

        triangle t = triangle{points[p1],points[p2],points[p3]};
        triangles.push_back(t);
    }

    //evaluate triangulation
    if(checkCircleCriterion(triangles, points))
        std::cout<<"YES";
    else
        std::cout<<"NO";
    return 0;
}
// author: Anton Lammert
// authot: Philipp Tornow 118332

#include <iostream>
#include <algorithm>

using namespace std;

struct point{
    double x, y;
};

struct triangle{
    point a, b, c;
};

point operator * (double a, point const& b){
    return {b.x*a, b.y*a};
}

point operator + (point const& a, point const& b){
    return {a.x + b.x, a.y + b.y};
}

bool operator == (point const& a, point const& b){
    return a.x == b.x && a.y == b.y;
}

//returns -1 for ccw, +1 for cw and 0 if b is collinear with the line through a and c
int orientation(point const& a, point const& b, point const& c){
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

//returns the area of the triangle using the implicit line equation
double area(triangle const&  t){
    double x1y2 = t.a.x*t.b.y;
    double x3y1 = t.c.x*t.a.y;
    double x2y3 = t.b.x*t.c.y;
    double x3y2 = t.c.x*t.b.y;
    double x2y1 = t.b.x*t.a.y;
    double x1y3 = t.a.x*t.c.y;
    return 0.5f * (x1y2 + x3y1 + x2y3 - x3y2 - x2y1 - x1y3);
}

//returns true if the point is inside the triangle
bool inside_triangle(triangle const& t, point const& p){

    //if triangle is a point check if p is the same point
    if(t.a == t.b && t.b == t.c)
        return p == t.a;

    double area_t = area(t);

    //if triangle is a segment check if p is on the segment
    if(area_t == 0){
        //check if p is on the line going through the segment
        if(orientation(t.a, p, t.b) == 0){
            //check if point is on segment
            int min_x = min(min(t.a.x, t.b.x), t.c.x);
            int max_x = max(max(t.a.x, t.b.x), t.c.x);
            int min_y = min(min(t.a.y, t.b.y), t.c.y);
            int max_y = max(max(t.a.y, t.b.y), t.c.y);
            return p.x <= max_x && p.x >= min_x && p.y <= max_y && p.y >= min_y;
        }
        return false;
    }




    double area_p2 = area({t.c, t.a, p});
    double area_p3 = area({t.a, t.b, p});
    double area_p1 = area({t.b, t.c, p});

    if(area_t != 0) {
        double f_a = area_p1 / area_t;
        double f_b = area_p2 / area_t;
        double f_c = area_p3 / area_t;

        if (f_a >= 0 && f_b >= 0 && f_c >= 0 && f_a <= 1 && f_b <= 1 && f_c <= 1)
            return true;

    }
    return false;
}

int main(int argc, char const *argv[])
{
    double x_0,x_1,x_2,y_0,y_1,y_2;

	while(true){
        cin >> x_0 >> y_0 >> x_1 >> y_1 >> x_2 >> y_2;
        if(x_0 == y_0 && y_0 == x_1 && x_1 == y_1 && y_1 == x_2 && x_2 == y_2 && y_2 == 0){
            break;
        }
        triangle t{{x_0,y_0},{x_1,y_1},{x_2,y_2}};
        //what if triangle is only one point?
        //what if triangle is only one line?
        for(int i = 0; i <= 20; ++i){
            for(int j = 0; j <= 20; ++j){
                if(inside_triangle(t, {static_cast<double>(j),20.0f - i }))
                    cout << "*";
                else
                    cout << ".";
            }
            cout << endl;
        }
        cout << endl;
	}
	return 0;
}
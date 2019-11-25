//
// Created by Mortiferum on 25.11.2019.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

struct point{
    int x,y;
};

struct segment{
    point a,b;
};

bool operator == (point const& a, point const& b){
    return a.x == b.x && a.y == b.y;
}

//returns -1 for ccw, +1 for cw and 0 if b is collinear with the line through a and c
int ORI(point const& a, point const& b, point const& c){
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
bool compare(point const& a, point const& b){
    if(a.y < b.y)
        return true;
    if(a.y == b.y)
        return a.x < b.x;
    return false;
}

double dist_p_s(point const& p, segment const& s){
    return ((s.b.y-s.a.y)*p.x-(s.b.x-s.a.x)*p.y+s.b.x*s.a.y-s.b.y*s.a.x)/(sqrt(pow(s.b.y-s.a.y,2) + pow(s.b.x-s.a.x,2)));
}

point find_max_dis_p(vector<point> const& points, segment const& s){
    double dist = 0, tmp_dist;
    point max_dist_p{};
    for(int i = 0; i < points.size(); ++i){
        tmp_dist = abs(dist_p_s(points.at(i),s));
        if(tmp_dist > dist) {
            max_dist_p = points.at(i);
            dist = tmp_dist;
        }
    }
    return max_dist_p;
}

vector<point> compute_con_hull(vector<point> const& points, bool first){
    point a = *points.begin();
    point b = *points.rbegin();
    point d{};
    segment ab{a,b};
    vector<point> left,right;

    if(points.size() == 2){
        return points;
    }

    if(first) {
        left.push_back(a);
        right.push_back(b);
        for(int i = 1; i < points.size() - 1; ++i){
            int ori = ORI(a, b, points.at(i));
            if(ori == -1){
                left.push_back(points.at(i));
            }else if(ori == 1){
                right.push_back(points.at(i));
            }
        }
        left.push_back(b);
        right.push_back(a);
    } else {
        d = find_max_dis_p(points, ab);
        left.push_back(a);
        right.push_back(d);
        for(int i = 1; i < points.size() - 1; ++i){
            int ori = ORI(a, d, points.at(i));
            if(ori == -1){
                left.push_back(points.at(i));
            }
            ori = ORI(d, b, points.at(i));
            if(ori == -1){
                right.push_back(points.at(i));
            }
        }
        left.push_back(d);
        right.push_back(b);
    }
    left = compute_con_hull(left, false);
    right = compute_con_hull(right, false);
    left.insert(left.end(), right.begin(), right.end());
    if(first){
        left.erase(unique(left.begin(),left.end()),left.end());
        if(left.at(0) == left.at(left.size()-1)) {
            left.pop_back();
        }
    }
    return left;
}

int main() {
    int n,x,y;
    vector<point> con_hull;

    cin >> n;

    while(n--){
        cin >> x >> y;
        if(x < -350 || x > 350 || y < -350 || y > 350){
            cout << "Error in the input";
            continue;
        }
        con_hull.push_back(point{x,y});
    }

    sort(con_hull.begin(), con_hull.end(), compare);

    if(con_hull.size() >= 3){
        con_hull = compute_con_hull(con_hull, true);
    }

    if(con_hull.size() >= 3) {
        cout << con_hull.size() << "\n";
        for (int i = 0; i < con_hull.size(); ++i) {
            cout << con_hull.at(i).x << " " << con_hull.at(i).y << "\n";
        }
    }
}

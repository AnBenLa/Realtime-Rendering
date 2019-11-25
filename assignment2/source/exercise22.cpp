//
// Created by Mortiferum on 25.11.2019.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
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

bool operator < (point const& a, point const& b){
    if(a.y < b.y)
        return true;
    else if(a.y == b.y)
        return a.x < b.x;
    else
        return false;
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

//returns the distance of point p to the segment s
double dist_p_s(point const& p, segment const& s){
    return ((s.b.y-s.a.y)*p.x-(s.b.x-s.a.x)*p.y+s.b.x*s.a.y-s.b.y*s.a.x)/(sqrt(pow(s.b.y-s.a.y,2) + pow(s.b.x-s.a.x,2)));
}

point find_max_dis_p(vector<point> const& points, segment const& s){
    double dist = 0, tmp_dist;
    point max_dist_p{};
    for(int i = 0; i < points.size(); ++i){
        tmp_dist = abs(dist_p_s(points.at(i),s));
        if(tmp_dist >= dist) {
            max_dist_p = points.at(i);
            dist = tmp_dist;
        }
    }
    return max_dist_p;
}

//finds the convex hull without any coolinear points on the outer lines
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

//finds the convex hull with all coolinear points on the outer lines
vector<point> compute_con_hull_co(vector<point> const& points, bool first){
    point a = *points.begin();
    point b = *points.rbegin();
    point d{};
    segment ab{a,b};
    vector<point> left{},right{};

    if(points.size() == 2){
        return points;
    }

    if(first) {
        left.push_back(a);
        right.push_back(b);
        for(int i = 1; i < points.size() - 1; ++i){
            int ori = ORI(a, b, points.at(i));
            if(ori == -1 || ori == 0){
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
            if(ori == -1 || ori == 0){
                left.push_back(points.at(i));
            }
            ori = ORI(d, b, points.at(i));
            if(ori == -1 || ori == 0){
                right.push_back(points.at(i));
            }
        }
        left.push_back(d);
        right.push_back(b);
    }
    if(left.size() == points.size()) {
        bool coolinear = true;
        for(int i = 0; i < left.size(); ++i){
            if(ORI(left.at(0), left.at(left.size()-1), left.at(i)) != 0) {
                coolinear = false;
                break;
            }
        }
        if(coolinear) {
            return left;
        }
    }
    if(right.size() == points.size()) {
        bool coolinear = true;
        for(int i = 0; i < right.size(); ++i){
            if(ORI(right.at(0), right.at(right.size()-1), right.at(i)) != 0) {
                coolinear = false;
                break;
            }
        }
        if(coolinear) {
            return right;
        }
    }

    left = compute_con_hull_co(left, false);
    right = compute_con_hull_co(right, false);
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
    int n,x,y,layers = 0;
    vector<point> con_hull,con_hull_r,con_hull_co;
    multiset<point> point_set;
    cin >> n;

    while(n--){
        cin >> x >> y;
        if(x < -350 || x > 350 || y < -350 || y > 350){
            cout << "Error in the input";
            continue;
        }
        con_hull.push_back(point{x,y});
        point_set.insert(point{x,y});
    }

    bool finished = false;
    while(!finished) {
        sort(con_hull.begin(), con_hull.end(), compare);

        if (con_hull.size() >= 3) {
            con_hull_r = compute_con_hull(con_hull, true);
            con_hull_co = compute_con_hull_co(con_hull, true);
        }


        //only if it really is a convex hull continue
        if (con_hull_r.size() >= 3) {
            layers += 1;
            cout << con_hull_r.size() << " # onion layer " << layers << " contains " << con_hull_r.size() << " points\n";
            for (int i = 0; i < con_hull_r.size(); ++i) {
                cout << con_hull_r.at(i).x << " " << con_hull_r.at(i).y << "\n";
            }
            for(int i = 0; i < con_hull_co.size(); ++i) {
                point_set.erase(con_hull_co.at(i));
            }
            con_hull.clear();
            con_hull_r.clear();
            con_hull_co.clear();
            for(auto p : point_set){
                con_hull.push_back(p);
            }
        } else {
            finished = true;
        }
    }
}

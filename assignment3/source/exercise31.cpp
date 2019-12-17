//
// Created by Anton on 17.12.2019.
//

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

#define us unsigned  short

using namespace std;

struct point{
    short x,y;
};

struct segment{
    char id;
    point a,b;
};

bool operator > (segment const& s1, segment const& s2){
    return (s1.a.y > s2.a.y && s1.a.y > s2.b.y) || (s1.b.y > s2.a.y && s1.b.y > s2.b.y);
}

int main (){
    us n;
    short x0,y0,x1,y1;
    char id;
    vector<segment> segments;
    ifstream cin ("C:\\Users\\Anton\\CLionProjects\\Realtime-Rendering\\assignment3\\source\\input.txt");
    cin >> n;
    while(n--){
        cin >> id >> x0 >> y0 >> x1 >> y1;
        segments.push_back({id,{x0,y0},{x1,y1}});
    }
    sort(segments.begin(), segments.end(),[](segment const& a, segment const& b) -> bool{return a > b;});
    return 0;
}
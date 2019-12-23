//
// Created by Anton on 17.12.2019.
//

#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <iomanip>

#define us unsigned  short

using namespace std;

struct point{
    double x,y;
    string id;
};

bool operator == (point const& a, point const& b){
    return a.x == b.x && a.y == b.y && a.id == b.id;
}

struct segment{
    string id;
    point a,b;
};

struct event{
    bool intersection;
    string id_a, id_b;
    point p;
};

bool operator > (segment const& s1, segment const& s2){
    return (s1.a.y > s2.a.y && s1.a.y > s2.b.y) || (s1.b.y > s2.a.y && s1.b.y > s2.b.y);
}

ostream& operator << (ostream& os, vector<event>const& events){
    cout << setprecision(1) << fixed;
    for(auto const& elem : events){
        os << "("<< elem.p.x << " , " << elem.p.y << ") ";
    }
    return os;
}

ostream& operator << (ostream& os, vector<segment>const& segments){
    for(auto const& seg : segments){
        os << seg.id << " ";
    }
    return os;
}

//siehe ORI2 function from slide 48
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
    if(dx31*dx21 < 0 || dy21*dy31 < 0)
        return -1;
    if(dx31*dx31 + dy31*dy31 > dx21*dx21 + dy21*dy21)
        return +1;
    return 0;
}

//http://www.softwareandfinance.com/Visual_CPP/VCPP_Intersection_Two_lines_EndPoints.html
void intersection_check(segment const& a, segment const& b, vector<event>& events){
    bool intersection = orientation(a.a, a.b, b.a)*orientation(a.a,a.b,b.b) <= 0
                        && orientation(b.a,b.b,a.a)*orientation(b.a,b.b,a.b) <= 0;
    if(intersection){
        double dx1,dx2,dy1,dy2,m1,m2,c1,c2;
        dx1 = a.b.x - a.a.x;
        dy1 = a.b.y - a.a.y;
        m1 = dy1 / dx1;
        c1 = a.a.y - m1 * a.a.x;
        dx2 = b.b.x - b.a.x;
        dy2 = b.b.y - b.a.y;
        m2 = dy2 / dx2;
        c2 = b.a.y - m2 * b.a.x;
        point intersect{(c2-c1)/(m1-m2), m1*(c2-c1)/(m1-m2)+c1};
        events.push_back(event{true,a.id,b.id,intersect});
        sort(events.begin(), events.end(),[](event const& a, event const& b) -> bool{return a.p.y > b.p.y;});
    }
}

void linesweep(map<string,segment>& segments, vector<point>& points){
    sort(points.begin(), points.end(),[](point const& a, point const& b) -> bool{return a.y > b.y;});
    vector<segment> active_segments;
    vector<event> event_queue;
    event_queue.reserve(points.size());

    //add all the points to the event queue
    for(auto const& p : points){
        event_queue.push_back(event{false,"","", p});
    }
#
    //process all elements in the queue
    while(!event_queue.empty()){
        event current_event = event_queue.front();
        if(current_event.intersection){
            //TODO implement this in the correct way
            //swap the two intersecting nodes in the active_segment list check for intersections while doing so
            vector<segment>::iterator intersecting_segment_1_it;
            vector<segment>::iterator intersecting_segment_2_it;

            for(auto it = active_segments.begin(); it != active_segments.end(); it++){
                if(it->id == current_event.id_a){
                    intersecting_segment_1_it = it;
                } else if(it->id == current_event.id_b){
                    intersecting_segment_2_it = it;
                }
            }
            //most likely not the correct way to do it!
            swap(intersecting_segment_1_it, intersecting_segment_2_it);

        } else {
            segment current_segment = segments[current_event.p.id];
            if(current_segment.b == current_event.p){
                //if the point in the queue is the lower point of the segment remove the segment from the list of active segments
                vector<segment>::iterator segment_it;
                for(auto it = active_segments.begin(); it != active_segments.end(); it++){
                    if(it->id == current_event.p.id){
                        segment_it = it;
                        break;
                    }
                }
                active_segments.erase(segment_it);
            } else {
                /*
                    add the segment to the active segment list in the right position and check for intersections with neighbours
                    the right position is defined by the orientation of the point that should be processed in relation
                    to the active segments. If the point p is left of an segment a the segment of the point p should be left of the segment a
                    if an intersection exists create an intersection event with the intersecting segment ids and push ist to the queue
                */
                if(!active_segments.empty()) {
                    bool segment_added = false;
                    int index = 0;
                    for (auto it = active_segments.begin(); it != active_segments.end(); it++) {
                        if (orientation(it->a, current_event.p, it->b) == 1) {
                            index += 1;
                            continue;
                        } else {
                            active_segments.insert(it, current_segment);
                            segment_added = true;
                            break;
                        }
                    }
                    if(!segment_added)
                        active_segments.push_back(current_segment);
                    if(index - 1 >= 0)
                        intersection_check(current_segment, active_segments[index - 1], event_queue);
                    if(index + 1 < active_segments.size())
                        intersection_check(current_segment, active_segments[index + 1], event_queue);
                } else {
                    active_segments.push_back(current_segment);
                }
            }
        }
        event_queue.erase(event_queue.begin());
        cout << active_segments << "\n";
        cout << event_queue << "\n";
    }
}

int main (){
    us n;
    double x0,y0,x1,y1;
    string id;
    map<string,segment> segments;
    vector<point> points;
    ifstream cin ("C:\\Users\\Elke\\Desktop\\Anton\\Realtime-Rendering\\assignment3\\source\\input.txt");
    cin >> n;
    while(n--){
        cin >> id >> x0 >> y0 >> x1 >> y1;
        points.push_back({x0,y0,id});
        points.push_back({x1,y1,id});
        if(y0 > y1){
            segments.insert({id,{id,{x0,y0,id},{x1,y1,id}}});
        } else {
            segments.insert({id,{id,{x1,y1,id},{x0,y0,id}}});
        }
    }
    linesweep(segments, points);
    return 0;
}
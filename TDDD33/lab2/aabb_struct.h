#ifndef AABB_H
#define AABB_H

struct AABB { // struct for rectangle AABB
	int top;
	int left;
	int bottom;
	int right;
};

struct Point { // struct for Point definition with coordinates
	int x;
	int y;
};

AABB construct(int, int, int, int); // construct the rectangle
bool contain(AABB, int, int); // checks for existence of point in rectangle
bool contain(AABB &, Point &); // checks for existence of point in rectangle using struct
bool intersect(AABB, AABB); // checks for rectangle intersection
AABB min_bounding_box(AABB, AABB);
bool may_collide(AABB, AABB, AABB);
bool collision_point(AABB, AABB, AABB, Point &);

#endif

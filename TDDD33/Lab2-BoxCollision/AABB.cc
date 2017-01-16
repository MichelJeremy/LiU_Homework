#include <iostream>
#include <cstdlib>
#include <limits>

#include "aabb_struct.h"

using namespace std;

AABB construct(int top, int left, int bottom, int right) {
	struct AABB my_aabb;
	int temp; // temp int for swaps if needed

	if (top > bottom) { // swap bottom and top if needed
		temp = bottom;
		bottom = top;
		top = temp;
		cout << "Swapped bottom and top\n";
	}

	if (left > right) { // swap left and right if needed
		temp = left;
		left = right;
		right = temp;
		cout << "Swapped left and right\n";
	}

	my_aabb.top = top;
	my_aabb.left = left;
	my_aabb.bottom = bottom;
	my_aabb.right = right;

	return my_aabb;
}

bool contain(AABB aabb, int x, int y) {
	if ((x >= aabb.left) && (x <= aabb.right)) {
		if ((y <= aabb.bottom) && (y >= aabb.top)) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}


bool contain(AABB &aabb, Point &pt) {
	if (contain(aabb, pt.x, pt.y)) { // if true
		return true;
	} else {
		return false;
	}
}

bool intersect(AABB aabb1, AABB aabb2) {
	Point pt;
	// first, obtain amount of points wihin each
	int nbPoint1 = (aabb1.right - aabb1.left) * (aabb1.top - aabb1.bottom);
	int nbPoint2 = (aabb2.right - aabb2.left) * (aabb2.top - aabb2.bottom);

	if (nbPoint1 < nbPoint2) {
		// aabb1 has less points so we will work with it (less processing)
		// now, we are going to use the contain function define above to check
		// each coordinate of the rectangle

		// process the x axis of aabb1
		for (int i = aabb1.left; i <= aabb1.right; i++) {
			// process the y axis of aabb1
			for (int j = aabb1.top; j <= aabb1.bottom; j++) {
				pt.x = i; // assign x to the struct Point
				pt.y = j; // assign y to the struct Point
				 // compare each point to the second rectangle
				if (contain(aabb2, pt)) {
					return true;
				}
			}
		}
		// if code goes here, it means contain never returned true,
		// so the rectangle do not have any intersect point
		return false;
	} else {
		// same as above, except with other rectangle if he has less points
		// process the x axis of aabb2
		for (int i = aabb2.top; i < aabb2.bottom; i++) {
			// process the y axis of aabb2
			for (int j = aabb2.left; j < aabb2.right; j++) {
				pt.x = i; // assign x to the struct Point
				pt.y = j; // assign y to the struct Point
				// compare each point to the first rectangle
				if (contain(aabb1, pt)) {
					return true;
				}
			}
		}
		return false;
	}
}

AABB min_bounding_box(AABB aabb1, AABB aabb2) {
	AABB new_aabb;

	// find lowest top
	if (aabb1.top <= aabb2.top) {
		new_aabb.top = aabb1.top;
	} else {
		new_aabb.top = aabb2.top;
	}

	// find biggest bottom
	if (aabb1.bottom >= aabb2.bottom) {
		new_aabb.bottom = aabb1.bottom;
	} else {
		new_aabb.bottom = aabb2.bottom;
	}

	// find lowest left
	if (aabb1.left <= aabb2.left) {
		new_aabb.left = aabb1.left;
	} else {
		new_aabb.left = aabb2.left;
	}

	// find biggest right
	if (aabb1.right >= aabb2.right) {
		new_aabb.right = aabb1.right;
	} else {
		new_aabb.right = aabb2.right;
	}
	cout 	<< "New bounding box: " << new_aabb.top << " " << new_aabb.left << " "
				<< new_aabb.bottom << " " << new_aabb.right << endl;
	return new_aabb;
}

// use line equation to go from to origin coordinate (x_orig,y_orig) to the
// target coordinate(x_target, y_target). If any of the point is contained
// in the rectangle, orig is modified to hold coordinates of top left corner
bool follow_line(Point &orig, const Point &target, const AABB &collisionRect) {
	double m; // rise
	double c; // y = mx + c
	double y_float; // contain y's double value
	int y; // contains y's rounded value
	int startX;
	int startY;
	int endX;
	int endY;

	if (target.x < orig.x) {
		endX = orig.x;
		startX = target.x;
		endY = orig.y;
		startY = target.y;
	} else {
		endX = target.x;
		startX = orig.x;
		endY = target.y;
		startY = orig.y;
	}
	if (endX == startX || endY == startY) {
		// do nothing, keeping this just in case it is needed
	} else {
		m = static_cast<double>((target.y - orig.y))
			/ static_cast<double>((target.x - orig.x));
		c = orig.y - (m * orig.x); // c = y - mx
		cout << fixed << "m: " << m << " c: " << c << endl;
		for (int x = startX; x <= endX; x++) {
			y_float = (m * x) + c; // get the float value of y
			cout << "x: " << x;
			// round to int, and round up every values to match the scale
			y = static_cast<int>(y_float + 1);
			cout << " // y float: " << y_float << endl;
			if (contain(collisionRect, x, y)) {
				orig.x = x;
				orig.y = y;
				return true;
			}
		}
	}
	return false;
}


bool may_collide(AABB collideWith, AABB initialAABB, AABB finalAABB) {
	// get the min_bounding_box of the entire movement
	cout 	<< "Para 1: " << collideWith.top << " " << collideWith.left
				<< " " << collideWith.bottom << " " << collideWith.right << endl;
	cout 	<< "Para 2: " << initialAABB.top << " " << initialAABB.left << " "
				<< initialAABB.bottom << " " << initialAABB.right << endl;
	cout 	<< "Para 3: " << finalAABB.top << " " << finalAABB.left << " "
				<< finalAABB.bottom << " " << finalAABB.right << endl;

	AABB movementBox = min_bounding_box(initialAABB, finalAABB);
	/*Variables for all the corners of our initial and final AABBs*/
	Point bl1; // bottom left
	Point bl2;
	Point br1; // bottom right
	Point br2;
	Point tl1; // top left
	Point tl2;
	Point tr1; // top right
	Point tr2;


	/* Assignation of coordinates to the points defined above */
	bl1.x = initialAABB.left;
	bl1.y = initialAABB.bottom;
	bl2.x = finalAABB.left;
	bl2.y = finalAABB.bottom;

	br1.x = initialAABB.right;
	br1.y = initialAABB.bottom;
	br2.x = finalAABB.right;
	br2.y = finalAABB.bottom;

	tl1.x = initialAABB.left;
	tl1.y = initialAABB.top;
	tl2.x = finalAABB.left;
	tl2.y = finalAABB.top;

	tr1.x = initialAABB.right;
	tr1.y = initialAABB.top;
	tr2.x = finalAABB.right;
	tr2.y = finalAABB.top;


	/* The objective here is to calculate the rise of the lines formed by the
	 * junction of the original's corners and the final position's corners.
	 * The x coordinate is incremented by 1 each time and its y coordinate
 	 * on the line is deduced using y = mx + c (where c is calculated earlier).
	 * This y float value is then truncated and compared to the possible
	 * collision AABB (the AABB uses only integers).
	 *
	 * Here, I have to pay attention to the way C++ cast values during operations
	 * so that I can use the results in the right type.
	 *
	 * I will only comment the first block of code, the others are basically
	 * the same, but with different points
	*/

	cout.precision(5);
	// if movementBox and collideWith are touching each other,
	// it means that the movement may be colliding
	if (intersect(movementBox, collideWith)) {
		// this case is for handling rectangle moving in diagonals
		for (int i = 0; i <= 3; i++) {
			// Check line formed by bottom left corners
			if (i == 0) {
				if (follow_line(bl1, bl2, collideWith) == true) {
					return true;
				}
			}
			/*
			 * do it again for the next line
			 * theorically, we could do it for only two line (optimal case)
			 * however, it is easier to check every lines
			 * this way, we do not have to check the relative position
			 * of the initial/final position of the moving rectangle
			 * (i.e. above, at the right, left, ...)
			 */

			if (i == 1) {
				if (follow_line(br1, br2, collideWith) == true) {
					return true;
				}
			}

			// Check line formed by top left corners
			if (i == 2) {
				if (follow_line(tl1, tl2, collideWith) == true) {
					return true;
				}
			}

			// Check line formed by top right corners
			if (i == 3) {
				if (follow_line(tr1, tr2, collideWith) == true) {
					return true;
				}
			}
		}
		// if code goes here, none of the points above matched, so return false
		return false;
	} else {
		return false;
	}
}

/*
 * This function basically does the same thing than may_collide, except that
 * some code is added to obtain the top left corner of the sliding AABB
 * Some other parts are modified too, taking into account may_collide's return.
 *
 * we could use const AABB &collideWith and same for other parameters except
 * the last one but the test file is not built for it
*/
bool collision_point(AABB collideWith, AABB initialAABB, AABB finalAABB,
										Point &topleftCorner) {
	if (may_collide(collideWith, initialAABB, finalAABB	)) { // collision
		/*Variables for all the corners of our initial and final AABBs*/
		Point bl1; // bottom left
		Point bl2;
		Point br1; // bottom right
		Point br2;
		Point tl1; // top left
		Point tl2;
		Point tr1; // top right
		Point tr2;

		/* Assignation of coordinates to the points defined above */
		bl1.x = initialAABB.left;
		bl1.y = initialAABB.bottom;
		bl2.x = finalAABB.left;
		bl2.y = finalAABB.bottom;

		br1.x = initialAABB.right;
		br1.y = initialAABB.bottom;
		br2.x = finalAABB.right;
		br2.y = finalAABB.bottom;

		tl1.x = initialAABB.left;
		tl1.y = initialAABB.top;
		tl2.x = finalAABB.left;
		tl2.y = finalAABB.top;

		tr1.x = initialAABB.right;
		tr1.y = initialAABB.top;
		tr2.x = finalAABB.right;
		tr2.y = finalAABB.top;


		// this case is for handling rectangle moving in diagonals
		// now, get all the coordinates for line bl1 -> bl2
		for (int i = 0; i <= 3; i++) {
			// Check line formed by bottom left corners
			if (i == 0) {
				if (follow_line(bl1, bl2, collideWith) == true) {
					topleftCorner.x = bl1.x;
					topleftCorner.y = bl1.y - (initialAABB.bottom - initialAABB.top);
					return true;
				}
			}

		 	// Check line formed by bottom right corners
			if (i == 1) {
				if (follow_line(br1, br2, collideWith) == true) {
					topleftCorner.x = br1.x - (initialAABB.right - initialAABB.left);
					topleftCorner.y = br1.y - (initialAABB.bottom - initialAABB.top);
					return true;
				}
			}

			// Check line formed by top left corners
			if (i == 2) {
				if (follow_line(tl1, tl2, collideWith) == true) {
					topleftCorner.x = tl1.x;
					topleftCorner.y = tl1.y;
					return true;
				}
			}

			if (i == 3) {
				if (follow_line(tr1, tr2, collideWith) == true) {
					topleftCorner.x = tr1.x - (initialAABB.right - initialAABB.left);
					topleftCorner.y = tr1.y;
					return true;
				}
			}
		}
		// this return is not mandatory, as this case can't happen
		return false;
	} else { // no collision
		return false;
	}
}

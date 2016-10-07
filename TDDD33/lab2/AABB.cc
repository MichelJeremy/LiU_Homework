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
		// now, we are going to use the contain function define above to check each coordinate of the rectangle
		for (int i = aabb1.left; i <= aabb1.right; i++) { // process the x axis of aabb1
			for (int j = aabb1.top; j <= aabb1.bottom; j++) { // process the y axis of aabb1
				pt.x = i; // assign x to the struct Point
				pt.y = j; // assign y to the struct Point
				if (contain(aabb2, pt)) { // compare each point to the second rectangle
					return true;
				}
			}
		}
		return false; // if code goes here, it means contain never returned true, so the rectangle do not have any intersect point
	} else {
		// aabb 2 has less points so we will work with it (less processing)
		// now, we are going to use the contain function define above to check each coordinate of the rectangle
		for (int i = aabb2.top; i < aabb2.bottom; i++) { // process the x axis of aabb2
			for (int j = aabb2.left; j < aabb2.right; j++) { // process the y axis of aabb2
				pt.x = i; // assign x to the struct Point
				pt.y = j; // assign y to the struct Point
				if (contain(aabb1, pt)) { // compare each point to the first rectangle
					return true;
				}
			}
		}
		return false; // if code goes here, it means contain never returned true, so the rectangle do not have any intersect point
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
	cout << "New bounding box: " << new_aabb.top << " " << new_aabb.left << " " << new_aabb.bottom << " " << new_aabb.right << endl;
	return new_aabb;
}

bool may_collide(AABB collideWith, AABB initialAABB, AABB finalAABB) {
	// get the min_bounding_box of the entire movement
	cout << "Para 1: " << collideWith.top << " " << collideWith.left << " " << collideWith.bottom << " " << collideWith.right << endl;
	cout << "Para 2: " << initialAABB.top << " " << initialAABB.left << " " << initialAABB.bottom << " " << initialAABB.right << endl;
	cout << "Para 3: " << finalAABB.top << " " << finalAABB.left << " " << finalAABB.bottom << " " << finalAABB.right << endl;
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


	/* variables for the coordinates processing below */
	// start/end x and y are mostly used for visibility and code understanding
	int startX; // x coordinate of the line's start
	int startY; // y coordinate of the line's end
	int endX; // x coordinate of the line's end
	int endY; // y coordinate of the line's end

	double m; // rise
	double c; // y = mx + c
	double y_float; // contain y's double value
	int y; // contains y's rounded value

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


	/* The objective here is to calculate the rise of the lines formed by the junction
	 * of the original's corners and the final position's corners.
	 * The x coordinate is incremented by 1 each time and its y coordinate
 	 * on the line is deduced using y = mx + c (where c is calculated earlier).
	 * This y float value is then truncated and compared to the possible collision AABB
	 * (the AABB uses only integers).
	 *
	 * Here, I have to pay attention to the way C++ cast values during operations
	 * so that I can use the results in the right type.
	*/
	cout.precision(5);
	if (intersect(movementBox, collideWith)) { // if movementBox and collideWith are touching each other, it means that the movement may be colliding
		// this case is for handling rectangle moving in diagonals
		for (int i = 0; i <= 3; i++) {
			// Check line formed by bottom left corners
			if (i == 0) {
				cout << "case 0" << endl;
				if (bl2.x < bl1.x) {
					endX = bl1.x;
					startX = bl2.x;
					endY = bl1.y;
					startY = bl2.y;
				} else {
					endX = bl2.x;
					startX = bl1.x;
					endY = bl2.y;
					startY = bl1.y;
				}
				if (endX == startX || endY == startY) { // if we reach the end, continue to next line
					// do nothing, keeping this just in case it is needed
				} else {
					m = static_cast<double>((bl2.y - bl1.y)) / static_cast<double>((bl2.x - bl1.x)); // calculate the rise, may need to check if bl2 gt bl1 to avoid negative value
					c = bl1.y - (m * bl1.x); // c = y - mx
					cout << fixed << "m: " << m << " c: " << c << endl;
					for (int x = startX; x <= endX; x++) {
						y_float = (m * x) + c; // get the float value of y
						cout << "x: " << x;
						y = static_cast<int>(y_float + 1); // round to int, and round up every values to match the scale
						cout << " // y float: " << y_float << endl;
						if (contain(collideWith, x, y)) {
							return true;
						}
					}
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
		 	// Check line formed by bottom right corners
			if (i == 1) {
				cout << "case 1" << endl;
				if (br2.x < br1.x) {
					endX = br1.x;
					startX = br2.x;
					endY = br1.y;
					startY = br2.y;
				} else {
					endX = br2.x;
					startX = br1.x;
					endY = br2.y;
					startY = br1.y;
				}
				if (endX == startX || endY == startY) { // if we reach the end, continue to next line
					// do nothing
				} else {
					m = static_cast<double>((br2.y - br1.y)) / static_cast<double>((br2.x - br1.x)); // calculate the rise, may need to check if bl2 gt bl1 to avoid negative value
					c = br1.y - (m * br1.x); // c = y - mx
					cout << "m: " << m << " c: " << c << endl;
					for (int x = startX; x <= endX; x++) {
						y_float = (m * x) + c; // get the float value of y
						cout << "x: " << x;
						y = static_cast<int>(y_float + 1); // round to int
						cout << " // y float: " << y_float << endl;
						if (contain(collideWith, x, y)) {
							return true;
						}
					}
				}
			}

			// Check line formed by top left corners
			if (i == 2) {
				cout << "case 2" << endl;
				if (tl2.x < tl1.x) {
					endX = tl1.x;
					startX = tl2.x;
					endY = tl1.y;
					startY = tl2.y;
				} else {
					endX = tl2.x;
					startX = tl1.x;
					endY = tl2.y;
					startY = tl1.y;
				}
				if (endX == startX || endY == startY) { // if we reach the end, continue to next line
					// do nothing
				} else {
					m = static_cast<double>((tl2.y - tl1.y)) / static_cast<double>((tl2.x - tl1.x));
					c = tl1.y - (m * tl1.x); // c = y - mx
					//cout << "m: " << m << "c: " << c << endl;
					for (int x = startX; x <= endX; x++) {
						y_float = (m * x) + c; // get the float value of y
						cout << "x: " << x;
						y = static_cast<int>(y_float + 1); // round to int
						cout << " // y float: " << y_float << endl;
						if (contain(collideWith, x, y)) {
							return true;
						}
					}
				}
			}

			// Check line formed by top right corners
			if (i == 3) {
				cout << "case 3" << endl;
				if (tr2.x < tr1.x) {
					endX = tr1.x;
					startX = tr2.x;
					endY = tr1.y;
					startY = tr2.y;
				} else {
					endX = tr2.x;
					startX = tr1.x;
					endY = tr2.y;
					startY = tr1.y;
				}
				if (endX == startX || endY == startY) { // if we reach the end, continue to next line
					// do nothing
				} else {
					m = static_cast<double>((tr2.y - tr1.y)) / static_cast<double>((tr2.x - tr1.x)); // calculate the rise, may need to check if bl2 gt bl1 to avoid negative value
					c = tr1.y - (m * tr1.x); // c = y - mx
					//cout << "m: " << m << "c: " << c << endl;
					for (int x = startX; x <= endX; x++) {
						y_float = (m * x) + c; // get the float value of y
						cout << "x: " << x;
						y = static_cast<int>(y_float + 1); // round to int
						cout << " // y float: " << y_float << endl;
						if (contain(collideWith, x, y)) {
							return true;
						}
					}
				}
			}
		}
		return false; // if code goes here, none of the points above matched, so return false
	} else {
		return false;
	}
}

/*
 * This function basically does the same thing than may_collide, except that some code
 * is added to obtain the top left corner of the sliding AABB
 * Some other parts are modified too, taking into account may_collide's return, between
 * things.
*/
bool collision_point(AABB collideWith, AABB initialAABB, AABB finalAABB, Point &topleftCorner) {
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


		/* variables for the coordinates processing below */
		// start/end x and y are mostly used for visibility and code understanding
		int startX; // x coordinate of the line's start
		int startY; // y coordinate of the line's end
		int endX; // x coordinate of the line's end
		int endY; // y coordinate of the line's end

		double m; // rise
		double c; // y = mx + c
		double y_float; // contain y's double value
		int y; // contains y's rounded value

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


		/*
		 * The objective here is to calculate the rise of the line formed by the junction
		 * of the original's corner and the final position's corner.
		 * The x coordinate is incremented by 1 each time and its y coordinate
	 	 * on the line is deduced using y = mx + c (where c is calculated earlier).
		 * This y float value is then truncated and compared to the possible collision AABB
		 * (the AABB uses only integers).
		*/

		// this case is for handling rectangle moving in diagonals
		// now, get all the coordinates for line bl1 -> bl2
		for (int i = 0; i <= 3; i++) {
			// Check line formed by bottom left corners
			if (i == 0) {
				cout << "case 0" << endl;
				if (bl2.x < bl1.x) {
					endX = bl1.x;
					startX = bl2.x;
					endY = bl1.y;
					startY = bl2.y;
				} else {
					endX = bl2.x;
					startX = bl1.x;
					endY = bl2.y;
					startY = bl1.y;
				}
				if (endX == startX || endY == startY) { // if we reach the end, continue to next line
					// do nothing, keeping this just in case it is needed
				} else {
					m = static_cast<double>((bl2.y - bl1.y)) / static_cast<double>((bl2.x - bl1.x)); // calculate the rise, may need to check if bl2 gt bl1 to avoid negative value
					c = bl1.y - (m * bl1.x); // c = y - mx
					cout << fixed << "m: " << m << " c: " << c << endl;
					for (int x = startX; x <= endX; x++) {
						y_float = (m * x) + c; // get the float value of y
						cout << "x: " << x;
						y = static_cast<int>(y_float + 1); // round to int, and round up every values to match the scale
						cout << " // y float: " << y_float << endl;
						if (contain(collideWith, x, y)) {
							topleftCorner.x = x;
							topleftCorner.y = y - (initialAABB.bottom - initialAABB.top);							
							return true;
						}
					}
				}
			}

			/*
			 * do it again for the next line
			 * theorically, we could do it for only two line (optimal case)
			 * however, it is easier to check every lines
			 * this way, we do not have to check the relative position
			 * of the initial/final position of the moving rectangle
			 */
		 	// Check line formed by bottom right corners
			if (i == 1) {
				cout << "case 1" << endl;
				if (br2.x < br1.x) {
					endX = br1.x;
					startX = br2.x;
					endY = br1.y;
					startY = br2.y;
				} else {
					endX = br2.x;
					startX = br1.x;
					endY = br2.y;
					startY = br1.y;
				}
				if (endX == startX || endY == startY) { // if we reach the end, continue to next line
					// do nothing
				} else {
					m = static_cast<double>((br2.y - br1.y)) / static_cast<double>((br2.x - br1.x)); // calculate the rise, may need to check if bl2 gt bl1 to avoid negative value
					c = br1.y - (m * br1.x); // c = y - mx
					cout << "m: " << m << " c: " << c << endl;
					for (int x = startX; x <= endX; x++) {
						y_float = (m * x) + c; // get the float value of y
						cout << "x: " << x;
						y = static_cast<int>(y_float + 1); // round to int
						cout << " // y float: " << y_float << endl;
						if (contain(collideWith, x, y)) {
							topleftCorner.x = x - (initialAABB.right - initialAABB.left);
							topleftCorner.y = y - (initialAABB.bottom - initialAABB.top);
							return true;
						}
					}
				}
			}

			// Check line formed by top left corners
			if (i == 2) {
				cout << "case 2" << endl;
				if (tl2.x < tl1.x) {
					endX = tl1.x;
					startX = tl2.x;
					endY = tl1.y;
					startY = tl2.y;
				} else {
					endX = tl2.x;
					startX = tl1.x;
					endY = tl2.y;
					startY = tl1.y;
				}
				if (endX == startX || endY == startY) { // if we reach the end, continue to next line
					// do nothing
				} else {
					m = static_cast<double>((tl2.y - tl1.y)) / static_cast<double>((tl2.x - tl1.x));
					c = tl1.y - (m * tl1.x); // c = y - mx
					//cout << "m: " << m << "c: " << c << endl;
					for (int x = startX; x <= endX; x++) {
						y_float = (m * x) + c; // get the float value of y
						cout << "x: " << x;
						y = static_cast<int>(y_float + 1); // round to int
						cout << " // y float: " << y_float << endl;
						if (contain(collideWith, x, y)) {
							topleftCorner.x = x;
							topleftCorner.x = y;
							return true;
						}
					}
				}
			}

			if (i == 3) {
				cout << "case 3" << endl;
				if (tr2.x < tr1.x) {
					endX = tr1.x;
					startX = tr2.x;
					endY = tr1.y;
					startY = tr2.y;
				} else {
					endX = tr2.x;
					startX = tr1.x;
					endY = tr2.y;
					startY = tr1.y;
				}
				if (endX == startX || endY == startY) { // if we reach the end, continue to next line
					// do nothing
				} else {
					m = static_cast<double>((tr2.y - tr1.y)) / static_cast<double>((tr2.x - tr1.x)); // calculate the rise, may need to check if bl2 gt bl1 to avoid negative value
					c = tr1.y - (m * tr1.x); // c = y - mx
					//cout << "m: " << m << "c: " << c << endl;
					for (int x = startX; x <= endX; x++) {
						y_float = (m * x) + c; // get the float value of y
						cout << "x: " << x;
						y = static_cast<int>(y_float + 1); // round to int
						cout << " // y float: " << y_float << endl;
						if (contain(collideWith, x, y)) {
							topleftCorner.x = x - (initialAABB.right - initialAABB.left);
							topleftCorner.y = y;
							return true;
						}
					}
				}
			}
		}
		// this return is not mandatory, as this case can't happen
		return false;
	} else { // no collision
		return false;
	}
}

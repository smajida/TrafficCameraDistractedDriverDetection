/*
 * averageCoordinates.cpp
 *
 *  Created on: Aug 3, 2015
 *      Author: Vidur
 */

//include opencv library files
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/gpu/gpu.hpp"
#include <opencv2/nonfree/ocl.hpp>
#include <opencv/cv.h>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/core/core.hpp>
#include "bgfg_vibe.hpp"

//include c++ files
#include <iostream>
#include <fstream>
#include <ctime>
#include <time.h>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <limits>
#include <math.h>
#include <algorithm>
#include <vector>
#include <pthread.h>
#include <cstdlib>

#include "sortCoordinates.h"

//defining contant PI
#define PI 3.14159265

//namespaces for convenience
using namespace cv;
using namespace std;

//average multiple points
Point averagePoints(vector<Point> coordinates) {

	//if there is more than 1 coordinate
	if (coordinates.size() > 1) {

 		//variables to sum x and y coordinates
		double xCoordinate = 0;
		double yCoordinate = 0;

		//cycling through all coordinates and summing
		for (int v = 0; v < coordinates.size(); v++) {
			xCoordinate += coordinates[v].x;
			yCoordinate += coordinates[v].y;
		}

		//creating average point
		Point tmpPoint(xCoordinate / coordinates.size(),
				yCoordinate / coordinates.size());
		//returning average point
		return tmpPoint;
	}

	//if one point
	else if (coordinates.size() == 1) {
		cout << "ONLY  POINT " << coordinates.size() << endl;

		//return 1 point
		return coordinates[0];
	}

	//if no points
	else {
		cout << "ONLY POINT " << coordinates.size() << endl;

		//create point
		return Point(0, 0);
	}
}


//average car points
vector<Point> averageCoordinates(vector<Point> coordinates, int distanceThreshold) {

	//if more than 1 point
	if (coordinates.size() > 1)
	{
		//vectors of points
		vector<Point> destinationCoordinates;
		vector<Point> pointsToAverage;

		//sort coordinates
		coordinates = sortCoordinates(coordinates);

		//saving tmp point
		Point tmpPoint = coordinates[0];

		//control boolean
		bool enteredOnce = false;

		//cycling through all coordinates
		for (int v = 0; v < coordinates.size(); v++) {

			//if distance is above threshold
			if (sqrt(
					(abs(tmpPoint.y - coordinates[v].y)
							* (abs(tmpPoint.x - coordinates[v].x))))
					> distanceThreshold) {

				//save averaged cordinates
 				destinationCoordinates.push_back(averagePoints(pointsToAverage));

 				//read new tmp point
				tmpPoint = coordinates[v];

				//erase vector of points to average
				pointsToAverage.erase(pointsToAverage.begin(), pointsToAverage.end());

				//control boolean
				bool enteredOnce = true;
			}

			//if distance is below threshold
			else
			{
				//begin filling points to average
 				pointsToAverage.push_back(coordinates[v]);
			}
		}

		//if not entered once
		if (!enteredOnce) {
			//average all coordinates
			destinationCoordinates.push_back(averagePoints(pointsToAverage));
		}

		//if only 1 point
		else if (pointsToAverage.size() == 1) {
			//save point
			destinationCoordinates.push_back(pointsToAverage[0]);
		}

		//if more than 1 point
		else if (pointsToAverage.size() > 0) {
			//average points
			destinationCoordinates.push_back(averagePoints(pointsToAverage));
		}

		//return processed coordinates
		return destinationCoordinates;
	}

	//if 1 coordinate
	else if (coordinates.size() > 0) {
		cout << " RETURNING 1 POINT COOORDINATE" << endl;

		//return coordinates
		return coordinates;
	}

	//if no coordinates
	else {
		cout << " FIRST EMPTY POINT MADE ONE UP " << endl;

		//create point
		coordinates.push_back(Point(0, 0));

		//return vector
		return coordinates;
	}
}




#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <cstdio>

#include "derivative.hpp"
#include "motion.hpp"

int main( int argc, const char** argv )
{
	// Capture device
	cv::VideoCapture cap;

	// Frames
	cv::Mat prev, frame, grey_frame;
	
	// Motion tracker
	LKTracker motionTracker;
	
	if(argc > 1)
	{
		cap.open(std::string(argv[1]));
	}
	else
	{
		cap.open(CV_CAP_ANY);
	}
	
	if(!cap.isOpened())
	{
		std::cerr << "Error: could not load a camera or video." << std::endl;
	}
	
	// Create windows
	cv::namedWindow("Video", 1);

	// Get first two farmes
	do
	{
		cv::waitKey(100);
		cap >> frame;
		cv::cvtColor(frame, grey_frame, CV_BGR2GRAY);
		prev = grey_frame.clone();
	}
	while(frame.cols == 0);

	// Add region to tracker
	motionTracker.AddRegion(cv::Vec2i(0, 0), cv::Size(150, 150), prev, grey_frame);
	motionTracker.AddRegion(cv::Vec2i(150, 50), cv::Size(100, 100), prev, grey_frame);

	for(;;)
	{
		// Get new frame, remember previous		
		cv::waitKey(20);
		prev = grey_frame.clone();
		cap >> frame;
		
		if(!frame.data)
		{
			std::cerr << "Error: no frame data." << std::endl;
			break;
		}
		
		// Convert frame to grey-scale
		cv::cvtColor(frame, grey_frame, CV_BGR2GRAY);

		// Update region
		motionTracker.Update(prev, grey_frame);
		
		// Show current frame
		motionTracker.ShowAll();
	}
}

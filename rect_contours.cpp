#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src;
Mat src_gray;
int thresh = 100;
int max_thresh = 150;

void thresh_callback(int, void* );

int main(int argc, char ** argv) {

  src = imread(argv[1], 1); //read image given to program

  cvtColor(src,src_gray, CV_BGR2GRAY);//convert image to grayscale
  blur(src_gray,src_gray,Size(3,3));//blur image


  char* source_window = "Source";
  namedWindow(source_window, CV_WINDOW_AUTOSIZE);
  imshow( source_window, src );//open window showing source image

  createTrackbar("Threshold: ",  "Source", &thresh, max_thresh, thresh_callback);
  thresh_callback(0,0);

  waitKey(0);
  return 0;
}


void thresh_callback(int, void* )
{
  Mat threshold_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  threshold( src_gray, threshold_output, thresh, 175, THRESH_BINARY );  // Detect edges using Threshold
  findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );//findContours
  RNG& rng = rng;
  /// Find the rotated rectangles and ellipses for each contour
  vector<RotatedRect> minRect( contours.size() );
  vector<RotatedRect> minEllipse( contours.size() );
  for( int i = 0; i < contours.size(); i++ )
       { minRect[i] = minAreaRect( Mat(contours[i]) );
         if( contours[i].size() > 5 )
           { minEllipse[i] = fitEllipse( Mat(contours[i]) ); }
       }
     /// Draw contours + rotated rects + ellipses
         Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
         for( int i = 0; i< contours.size(); i++ )
            {
              Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
              // contour
              drawContours( drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
              // ellipse
              ellipse( drawing, minEllipse[i], color, 2, 8 );
              // rotated rectangle
              Point2f rect_points[4]; minRect[i].points( rect_points );
              for( int j = 0; j < 4; j++ )
                 line( drawing, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );
            }

         /// Show in a window
         namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
         imshow( "Contours", drawing );


}

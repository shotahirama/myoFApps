#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "opencv2/opencv.hpp"
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <cv_bridge/cv_bridge.h>

class ImageTrans {
public:
  ImageTrans() : it(n), rate(100) {}

  void convertImage(sensor_msgs::ImagePtr &msg, cv::Mat &img,
                    std::string encoding = "rgb8") {
    msg = cv_bridge::CvImage(std_msgs::Header(), encoding, img).toImageMsg();
  }

  void sleep() { rate.sleep(); }

private:
  ros::NodeHandle n;
  image_transport::ImageTransport it;
  ros::Rate rate;
};

class ofApp : public ofBaseApp {

public:
  void setup();
  void update();
  void draw();

  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y);
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void mouseEntered(int x, int y);
  void mouseExited(int x, int y);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);

  cv::Mat toCv(ofPixels &pix);

  ofVideoGrabber vidGrabber;
  ofxCvColorImage colorImg;
  ofxCvGrayscaleImage grayImage;
  ofImage draggedImages;

  cv::Mat src;
  ofImage dstImage;
  cv::Mat pubimg;
  cv::Mat colorimg;

  bool newImage;

  ros::NodeHandle n;
  ros::Publisher image_pub;
  ros::Publisher color_pub;

  ImageTrans ip;

  //  ros::Rate rate;
};

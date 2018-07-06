#pragma once

#include "ofMain.h"
#include "imagetransport4of.h"

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

private:
  cv::Mat src;
  ofImage dstImage;
  cv::Mat pubimg;
  cv::Mat colorimg;
  ofImage draggedImages;

  bool newImage;
  ros::Publisher gray_image_pub;
  ros::Publisher color_image_pub;

  ros::NodeHandle nh;
  cv::Mat toCv(ofPixels &pix);
  ImageTransport4oF image_tr;
};

#include "ofMain.h"
#include "ofApp.h"
#include <ros/ros.h>

//========================================================================
int main(int argc, char *argv[]) {
  ros::init(argc, argv, "oF_ros");
  ofSetupOpenGL(1024, 768, OF_WINDOW); // <-------- setup the GL context

  // this kicks off the running of my app
  // can be OF_WINDOW or OF_FULLSCREEN
  // pass in width and height too:
  ofRunApp(new ofApp());
}

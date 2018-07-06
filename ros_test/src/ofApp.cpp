#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
  newImage = false;
  color_image_pub = nh.advertise<sensor_msgs::Image>("oF_image/color", 1);
  gray_image_pub = nh.advertise<sensor_msgs::Image>("oF_image/gray", 1);
}

//--------------------------------------------------------------
void ofApp::update() {
  if (newImage) {
    cv::Mat gray;
    colorimg = src.clone();
    dstImage.allocate(src.cols, src.rows, OF_IMAGE_GRAYSCALE);
    cv::Mat dst = toCv(dstImage);
    cv::cvtColor(src, dst, cv::COLOR_RGB2GRAY);
    dstImage.update();
    pubimg = dst.clone();
    newImage = false;
    sensor_msgs::ImagePtr graymsg, colormsg;
    image_tr.convertImage(graymsg, pubimg, "mono8");
    image_tr.convertImage(colormsg, colorimg, "rgb8");
    color_image_pub.publish(colormsg);
    gray_image_pub.publish(graymsg);
    image_tr.sleep();
  }
}

//--------------------------------------------------------------
void ofApp::draw() {
  if (draggedImages.isAllocated() && dstImage.isAllocated()) {
    draggedImages.draw(0, 0);
    dstImage.draw(0, draggedImages.getHeight());
  }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
  if (draggedImages.load(dragInfo.files[0])) {
    draggedImages.setImageType(OF_IMAGE_COLOR);
    cout << draggedImages.getWidth() << ", " << draggedImages.getHeight()
         << endl;
    src = toCv(draggedImages);
    newImage = true;
  }
}

cv::Mat ofApp::toCv(ofPixels &pix) {
  return cv::Mat(pix.getHeight(), pix.getWidth(),
                 CV_MAKETYPE(CV_8U, pix.getNumChannels()), pix.getData(), 0);
}

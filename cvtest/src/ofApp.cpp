#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
  newImage = false;
  image_pub = n.advertise<sensor_msgs::Image>("oF_image/gray", 1);
  color_pub = n.advertise<sensor_msgs::Image>("oF_image/color", 1);
  //    vidGrabber.setVerbose(true);
  //    vidGrabber.setup(640, 480);
  //    draggedImages = ofImage();
  //    colorImg.allocate(320, 240);
  //    grayImage.allocate(320, 240);
}

//--------------------------------------------------------------
void ofApp::update() {
  ofBackground(100, 100, 100);
  if (newImage) {
    cv::Mat resimg, gray;
    //    if (src.cols > 640 || src.rows > 480) {
    //      float rate = float(src.rows) / src.cols;
    //      cv::resize(src, resimg, cv::Size(640, 640 * rate));
    //    } else {
    //      resimg = src.clone();
    //    }
    //    cout << resimg.cols << ", " << resimg.rows << endl;
    resimg = src.clone();
    colorimg = src.clone();
    dstImage.allocate(resimg.cols, resimg.rows, OF_IMAGE_GRAYSCALE);
    cv::Mat dst = toCv(dstImage);
    cv::cvtColor(resimg, dst, cv::COLOR_BGR2GRAY);
    dstImage.update();
    pubimg = dst.clone();
    newImage = false;
  }
  //  bool bNewFrame = false;
  //  vidGrabber.update();
  //  bNewFrame = vidGrabber.isFrameNew();

  //  if (bNewFrame) {
  //    colorImg.setFromPixels(vidGrabber.getPixels());
  //  }

  if (!colorimg.empty() && !pubimg.empty()) {
    sensor_msgs::ImagePtr msg, colormsg;
    ip.convertImage(msg, pubimg, sensor_msgs::image_encodings::MONO8);
    ip.convertImage(colormsg, colorimg, sensor_msgs::image_encodings::RGB8);
    image_pub.publish(msg);
    color_pub.publish(colormsg);
    ip.sleep();
  }

  ros::spinOnce();
}

//--------------------------------------------------------------
void ofApp::draw() {
  ofSetHexColor(0xffffff);
  if (dstImage.isAllocated()) {
    dstImage.draw(20, 20);
  }
  ofFill();
  //  ofSetHexColor(0x333333);
  //  ofDrawRectangle(360, 540, 320, 240);
  //  ofSetHexColor(0xffffff);

  //  ofSetColor(255);
  //  ofSetHexColor(0xffffff);
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
  if (dragInfo.files.size() > 0) {
    draggedImages.load(dragInfo.files[0]);
    draggedImages.setImageType(ofImageType::OF_IMAGE_COLOR);
    //    colorImg.allocate(draggedImages.getWidth(),
    //    draggedImages.getHeight());
    //    colorImg.setFromPixels(draggedImages.getPixels());
    src = toCv(draggedImages);
    newImage = true;
  }
}

cv::Mat ofApp::toCv(ofPixels &pix) {
  int depth;
  switch (pix.getBytesPerChannel()) {
  case 4:
    depth = CV_32F;
    break;
  case 2:
    depth = CV_16U;
    break;
  case 1:
  default:
    depth = CV_8U;
    break;
  }
  return cv::Mat(pix.getHeight(), pix.getWidth(),
                 CV_MAKETYPE(depth, pix.getNumChannels()), pix.getData(), 0);
}

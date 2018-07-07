#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
  //  ofSetWindowShape(480, 480);
  colors.push_back(ofColor::red);
  colors.push_back(ofColor::blue);
  colors.push_back(ofColor::green);
  colors.push_back(ofColor::yellow);
  colors.push_back(ofColor::magenta);
  menohvgg16.set_model_words("data/VGG16.onnx", "data/synset_words.txt");
}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
  ofBackground(ofColor::black);

  if (img.isAllocated()) {
    int maxtextsize = 0;
    for (auto pi : predict_indecies) {
      int textsize = pi.second.size() * 11;
      if (maxtextsize < textsize) {
        maxtextsize = textsize;
      }
    }
    auto bp = 20;
    //    ofSetColor(ofColor::white);
    //    ofBoxPrimitive boxt;
    //    boxt.setPosition(maxtextsize / 2, bp, 0);
    //    boxt.set(maxtextsize, 20, 0);
    //    boxt.draw();
    //    bp += 30;
    for (int i = 0; i < 5; i++) {
      //    for (auto pi : predict_indecies) {
      ofBoxPrimitive box;
      //      ofDrawBitmapString(predict_indecies[i].second + ": " +
      //                             ofToString(predict_indecies[i].first),
      //                         10, bp + 5);
      //      //  box.set(200);
      //      box.setPosition(maxtextsize + (100 * predict_indecies[i].first) /
      //      2, bp,
      //                      0);
      //      box.set(100 * predict_indecies[i].first, 20, 0);
      ofSetColor(colors[i]);
      box.set(100 * predict_indecies[i].first, 20, 0);
      box.setPosition((100 * predict_indecies[i].first) / 2, bp, 0);
      box.draw();
      ofSetColor(ofColor::white);
      ofDrawBitmapString(predict_indecies[i].second + ": " +
                             ofToString(predict_indecies[i].first),
                         100 * predict_indecies[i].first + 20, bp + 5);
      bp += 30;
    }
    //    int text_pos = 20;
    //    ofSetColor(ofColor::white);
    //    for (auto pi : predict_indecies) {
    //      ofDrawBitmapString(pi.second + ": " + ofToString(pi.first), 10,
    //      text_pos);
    //      text_pos += 20;
    //    }
    ofSetColor(ofColor::white);
    img.draw(10, bp + 10);
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
  img.load(dragInfo.files[0]);
  predict_indecies = menohvgg16.predict(img);
  for (auto pi : predict_indecies) {
    std::cout << pi.first << ", " << pi.second << std::endl;
  }
  //  crop_and_resize(img);
}

void ofApp::crop_and_resize(ofImage &img) {
  auto short_edge = std::min(img.getWidth(), img.getHeight());
  int x = (img.getWidth() - short_edge) / 2;
  int y = (img.getHeight() - short_edge) / 2;
  int w = short_edge;
  int h = short_edge;
  img.crop(x, y, w, h);
  img.resize(224, 224);
}

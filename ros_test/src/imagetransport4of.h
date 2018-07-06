#ifndef IMAGETRANSPORT4OF_H
#define IMAGETRANSPORT4OF_H

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

class ImageTransport4oF {
public:
  ImageTransport4oF() : it(nh), rate(100) {}

  void convertImage(sensor_msgs::ImagePtr &msg, cv::Mat &img,
                    std::string encoding = "rgb8") {
    msg = cv_bridge::CvImage(std_msgs::Header(), encoding, img).toImageMsg();
  }
  void sleep() { rate.sleep(); }

private:
  ros::NodeHandle nh;
  image_transport::ImageTransport it;
  ros::Rate rate;
};

#endif // IMAGETRANSPORT4OF_H

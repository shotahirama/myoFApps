#ifndef MENOHVGG16_H
#define MENOHVGG16_H

#include <menoh/menoh.hpp>
#include <opencv2/opencv.hpp>
#include "ofImage.h"

class MenohVGG16 {
public:
  MenohVGG16() {}
  void set_model_words(std::string modelpath, std::string wordspath);
  std::vector<std::pair<float, std::string>> predict(ofImage &img,
                                                     int top_k = 5);

private:
  cv::Mat crop_and_resize(cv::Mat mat, cv::Size const &size);
  std::vector<float> reorder_to_chw(cv::Mat const &mat);
  template <typename InIter>
  std::vector<typename std::iterator_traits<InIter>::difference_type>
  extract_top_k_index_list(
      InIter first, InIter last,
      typename std::iterator_traits<InIter>::difference_type k);
  std::vector<std::string>
  load_category_list(std::string const &synset_words_path);
  cv::Mat toCv(ofPixels &pix);
  std::unique_ptr<menoh::model> model;

  std::string input_name;
  std::string output_name;
  std::vector<std::string> categories;
};

#endif // MENOHVGG16_H

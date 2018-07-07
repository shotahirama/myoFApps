#include "menohvgg16.h"
#include <queue>

void MenohVGG16::set_model_words(std::string modelpath, std::string wordspath) {
  input_name = "140326425860192";
  output_name = "140326200803680";
  const int batch_size = 1;
  const int channel_num = 3;
  const int height = 224;
  const int width = 224;
  auto model_data = menoh::make_model_data_from_onnx(modelpath);
  categories = load_category_list(wordspath);
  menoh::variable_profile_table_builder vpt_builder;
  vpt_builder.add_input_profile(input_name, menoh::dtype_t::float_,
                                {batch_size, channel_num, height, width});
  vpt_builder.add_output_profile(output_name, menoh::dtype_t::float_);
  auto vpt = vpt_builder.build_variable_profile_table(model_data);
  menoh::model_builder model_builder(vpt);
  model.reset(
      new menoh::model(model_builder.build_model(model_data, "mkldnn")));
}

std::vector<std::pair<float, std::string>> MenohVGG16::predict(ofImage &img,
                                                               int top_k) {
  if (nullptr == model.get()) {
    std::cerr << "model not found" << std::endl;
    return std::vector<std::pair<float, std::string>>();
  }
  img.setImageType(ofImageType::OF_IMAGE_COLOR);
  cv::Mat cvImg = toCv(img);
  cv::Mat bgrImg;
  cv::cvtColor(cvImg, bgrImg, cv::COLOR_RGB2BGR);
  cv::Mat resImg = crop_and_resize(bgrImg, cv::Size(224, 224));
  auto image_data = reorder_to_chw(resImg);
  auto input_var = model->get_variable(input_name);
  float *input_buff = static_cast<float *>(input_var.buffer_handle);
  std::copy(image_data.begin(), image_data.end(), input_buff);
  model->run();
  auto output_var = model->get_variable(output_name);
  float *output_buff = static_cast<float *>(output_var.buffer_handle);
  auto top_k_indices = extract_top_k_index_list(
      output_buff, output_buff + output_var.dims.at(1), top_k);
  std::vector<std::pair<float, std::string>> predict_indices;
  for (auto ki : top_k_indices) {
    std::pair<float, std::string> p;
    p.first = *(output_buff + ki);
    std::string category_name = categories.at(ki);
    int sub = category_name.find(" ") + 1;
    p.second = category_name.substr(sub, category_name.size());
    predict_indices.push_back(p);
  }
  return predict_indices;
}

cv::Mat MenohVGG16::crop_and_resize(cv::Mat mat, cv::Size const &size) {
  auto short_edge = std::min(mat.size().width, mat.size().height);
  cv::Rect roi;
  roi.x = (mat.size().width - short_edge) / 2;
  roi.y = (mat.size().height - short_edge) / 2;
  roi.width = roi.height = short_edge;
  cv::Mat cropped = mat(roi);
  cv::Mat resized;
  cv::resize(cropped, resized, size);
  return resized;
}

std::vector<float> MenohVGG16::reorder_to_chw(cv::Mat const &img) {
  assert(img.channels() == 3);
  cv::Mat mat = img.clone();
  std::vector<float> data(mat.channels() * mat.rows * mat.cols);
  for (int y = 0; y < mat.rows; ++y) {
    for (int x = 0; x < mat.cols; ++x) {
      for (int c = 0; c < mat.channels(); ++c) {
        data[c * (mat.rows * mat.cols) + y * mat.cols + x] =
            static_cast<float>(mat.data[y * mat.step + x * mat.elemSize() + c]);
      }
    }
  }
  return data;
}

template <typename InIter>
std::vector<typename std::iterator_traits<InIter>::difference_type>
MenohVGG16::extract_top_k_index_list(
    InIter first, InIter last,
    typename std::iterator_traits<InIter>::difference_type k) {
  using diff_t = typename std::iterator_traits<InIter>::difference_type;
  std::priority_queue<
      std::pair<typename std::iterator_traits<InIter>::value_type, diff_t>>
      q;
  for (diff_t i = 0; first != last; ++first, ++i) {
    q.push({*first, i});
  }
  std::vector<diff_t> indices;
  for (diff_t i = 0; i < k; ++i) {
    indices.push_back(q.top().second);
    q.pop();
  }
  return indices;
}

std::vector<std::string>
MenohVGG16::load_category_list(std::string const &synset_words_path) {
  std::ifstream ifs(synset_words_path);
  if (!ifs) {
    throw std::runtime_error("File open error: " + synset_words_path);
  }
  std::vector<std::string> categories;
  std::string line;
  while (std::getline(ifs, line)) {
    categories.push_back(std::move(line));
  }
  return categories;
}

cv::Mat MenohVGG16::toCv(ofPixels &pix) {
  return cv::Mat(pix.getHeight(), pix.getWidth(),
                 CV_MAKETYPE(CV_8U, pix.getNumChannels()), pix.getData(), 0);
}

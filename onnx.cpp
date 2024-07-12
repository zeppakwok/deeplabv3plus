#include "torch/script.h"
#include "torch/torch.h"
#include <iostream>
#include <vector>
#include <onnxruntime_cxx_api.h>
#include <opencv2/opencv.hpp>
#include <opencv2\imgproc\types_c.h>
 
using namespace std;
 
int main()
{
	string mobileFile = "models.onnx";
	string imageFile;
	cout << "请输入图像文件名：";
	cin >> imageFile;
 
	cv::dnn::Net net = cv::dnn::readNetFromONNX(mobileFile);
	cv::Mat imageBGR = cv::imread(imageFile, cv::ImreadModes::IMREAD_COLOR);
	int w = imageBGR.cols;
	int h = imageBGR.rows;
	cv::Mat resizedImageRGB, resizedImage, preprocessedImage;
	cv::resize(imageBGR, resizedImage, cv::Size(512, 512), cv::INTER_AREA);
	cv::cvtColor(resizedImage, resizedImageRGB, cv::ColorConversionCodes::COLOR_BGR2RGB);
	resizedImageRGB.convertTo(resizedImage, CV_32F, 1.0 / 255);
	cv::Mat channels[3];
	cv::split(resizedImage, channels);
	cv::merge(channels, 3, resizedImage);
	cv::dnn::blobFromImage(resizedImage, preprocessedImage);
 
	net.setInput(preprocessedImage);
	cv::Mat result = net.forward();
	int size[3] = { result.size[1], result.size[2], result.size[3] };
	cv::Mat output(3, size, CV_32F, result.ptr<float>());
	auto output_tensor = torch::from_blob(output.data, {result.size[1], result.size[2], result.size[3]});
	output_tensor = output_tensor.permute({ 1, 2, 0 });
	output_tensor = torch::softmax(output_tensor, -1);
	output_tensor = output_tensor.argmax(-1) * (255/(result.size[1]-1));
	output_tensor = output_tensor.to(torch::kU8);
	cv::Mat predict(512, 512, CV_8UC1, output_tensor.data_ptr<uchar>());
	cv::resize(predict, predict, cv::Size(w, h), cv::INTER_AREA);
	cv::imshow("分割图", predict);
	cv::imwrite("分割图.jpg", predict);
 
	return 0;
}
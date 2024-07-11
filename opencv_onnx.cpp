#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <onnxruntime_cxx_api.h>
#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;

int main()
{
  String modelFile = "./torch.onnx";
  String imageFile = "./1.jpg";

  dnn::Net net = cv::dnn::readNetFromONNX(modelFile); //读取网络和参数
   
   // step 1: Read an image in HWC BGR UINT8 format.
    cv::Mat imageBGR = cv::imread(input_path, cv::ImreadModes::IMREAD_COLOR);
    // step 2: Resize the image.
    cv::Mat resizedImageRGB, resizedImage, preprocessedImage;
	resize(imageBGR , resizedImage, Size(500, 500), INTER_AREA)
    // step 3: Convert the image to HWC RGB UINT8 format.
    cv::cvtColor(resizedImage, resizedImageRGB,
        cv::ColorConversionCodes::COLOR_BGR2RGB);
    // step 4: Convert the image to HWC RGB float format by dividing each pixel by 255.
    resizedImageRGB.convertTo(resizedImage, CV_32F, 1.0 / 255);
    // step 5: Split the RGB channels from the image.   
    cv::Mat channels[3];
    cv::split(resizedImage, channels);
    //step 7: Merge the RGB channels back to the image.
    cv::merge(channels, 3, resizedImage);
    // step 8: Convert the image to CHW RGB float format.
    // HWC to CHW
    cv::dnn::blobFromImage(resizedImage, preprocessedImage);
    
   net.setInput(inputBolb); //输入图像
    Mat result = net.forward(); //前向计算
    cout << result << endl;
    return 0;
}

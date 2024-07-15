#include <stdio.h>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv_modules.hpp>

using namespace cv;
using namespace std;


Mat GetMagnitude(Mat f) {

    Mat M;
    std::vector<Mat> channels;
    split(f, channels);//imaginary와 real로 나눈다
    magnitude(channels[0], channels[1], M);

    return M;
}

Mat filter(Mat f, Mat h, float K) {
    Mat output;
    Mat H, result;
    Mat d, D, G;
    std::vector<Mat> Area;


    f.convertTo(f, CV_32FC2, 1 / 255.f); //input
    h.convertTo(h, CV_32FC2, 1 / 255.f); //Kernel

    h /= sum(h);
    dft(f, G, DFT_COMPLEX_OUTPUT);
    dft(h, H, DFT_COMPLEX_OUTPUT);

    mulSpectrums(G, H, output, 0, true); //분자 G * H*

    d = GetMagnitude(H);//d = |H|
    multiply(d, d, D);//분모 D =|H|^2
   
    split(output, Area);//one channel인 D를 이용한 나눗셈을 위해 complex와 real로 나눈다
    divide(Area[0], D + K, Area[0]);
    divide(Area[1], D + K, Area[1]);
    merge(Area, output);//나눗셈 후 다시 병합

    idft(output, result, DFT_SCALE | DFT_REAL_OUTPUT);

    return result;
}



int main()
{
    float K = 0;
   
     
    Mat f = imread("C:\\HWpic\\Wiener_Input2.png", 0);
    Mat f2 = imread("C:\\HWpic\\Wiener_Input2.png", 0);
    Mat h= imread("C:\\HWpic\\Wiener_Kernel.png", 0);
    
    Mat inverse, wiener;

    inverse=filter(f, h, K);

    K = 0.001;
    wiener = filter(f, h, K);
    
   
    imshow("wiener", wiener);
    waitKey(0);
   
   
    return 0;
}

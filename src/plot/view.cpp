/*! \file view.cpp
 * \brief View class implementation.
 *
 * \authors Dmitrii Leliuhin
 * \date July 2020
*/

//=======================================================================================

#include "view.h"

#include "vlog.h"

//=======================================================================================
View::View( const std::string& name, const Config& conf )
{
    cv::moveWindow( name, 0, 0 );

    if ( conf.main.rotate == 0 )
        _rotate_code = - 1;

    else if ( conf.main.rotate == 90 )
        _rotate_code = cv::ROTATE_90_CLOCKWISE;

    else if ( conf.main.rotate == - 90 )
        _rotate_code = cv::ROTATE_90_COUNTERCLOCKWISE;

    else if ( conf.main.rotate == 180 )
        _rotate_code = cv::ROTATE_180;
}
//=======================================================================================


//=======================================================================================
void View::plot( const Pack& data )
{
    if ( data.frame.jpeg.empty() )
        return;

    std::vector<char> buf;
    buf.assign( data.frame.jpeg.data(),
                data.frame.jpeg.data() + data.frame.jpeg.size() );

    auto img = cv::imdecode( buf, cv::IMREAD_COLOR );

    cv::resize( img, img, { 400, 400 } );

    if ( _rotate_code != - 1 )
        cv::rotate( img, img, _rotate_code );

    cv::moveWindow( _name, 0, 0 );
    cv::imshow( _name, img );

    _watershed( img );
    _k_means( img );

    auto key = cv::waitKey(10);

    if ( key == 27 )
        vapplication::stop();

    else if ( key == 'p' )
        cv::waitKey();
}
//=======================================================================================

using namespace cv;
//=======================================================================================
void View::_watershed( const cv::Mat& src )
{
    Mat kernel = (Mat_<float>(3,3) <<
                  1,  1, 1,
                  1, -8, 1,
                  1,  1, 1);

    Mat imgLaplacian;
    filter2D(src, imgLaplacian, CV_32F, kernel);

    Mat sharp;
    src.convertTo(sharp, CV_32F);

    Mat imgResult = sharp - imgLaplacian;
    imgResult.convertTo(imgResult, CV_8UC3);
    imgLaplacian.convertTo(imgLaplacian, CV_8UC3);

    //    cv::moveWindow( "Morph", 1200, 0 );
    //    cv::imshow( "Morph", bin );

    Mat bw;
    cvtColor(imgResult, bw, COLOR_BGR2GRAY);
    threshold(bw, bw, 110, 255, THRESH_BINARY | THRESH_OTSU);

    cv::moveWindow( "Threshold", 400, 0 );
    cv::imshow( "Threshold", bw );

    Mat dist;
    distanceTransform(bw, dist, DIST_L2, 3);
    normalize(dist, dist, 0.05, 1.0, NORM_MINMAX);

    cv::moveWindow( "Distance Transform", 800, 0 );
    cv::imshow( "Distance Transform", dist );

    threshold(dist, dist, 0.1, 1.0, THRESH_BINARY);

    Mat kernel1 = Mat::ones(3, 3, CV_8U);
    dilate(dist, dist, kernel1);

    cv::moveWindow( "Dilated", 1200, 0 );
    cv::imshow( "Dilated", dist );

    Mat dist_8u;
    dist.convertTo(dist_8u, CV_8U);

    std::vector<std::vector<Point> > contours;
    findContours(dist_8u, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    Mat markers = Mat::zeros(dist.size(), CV_32S);

    for (size_t i = 0; i < contours.size(); i++)
        drawContours(markers, contours, static_cast<int>(i), Scalar(static_cast<int>(i)+1), -1);

    circle(markers, Point(5,5), 3, Scalar(255), -1);

    watershed(imgResult, markers);

    Mat mark;
    markers.convertTo(mark, CV_8U);
    bitwise_not(mark, mark);

    std::vector<Vec3b> colors;
    for (size_t i = 0; i < contours.size(); i++)
    {
        int b = theRNG().uniform(0, 256);
        int g = theRNG().uniform(0, 256);
        int r = theRNG().uniform(0, 256);
        colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
    }

    Mat dst = Mat::zeros(markers.size(), CV_8UC3);
    // Fill labeled objects with random colors
    for (int i = 0; i < markers.rows; i++)
    {
        for (int j = 0; j < markers.cols; j++)
        {
            int index = markers.at<int>(i,j);
            if (index > 0 && index <= static_cast<int>(contours.size()))
            {
                dst.at<Vec3b>(i,j) = colors[index-1];
            }
        }
    }

    cv::moveWindow( "Watershed", 1600, 0 );
    cv::imshow( "Watershed", dst );
}
//=======================================================================================
void View::_k_means( const Mat& src )
{
    Mat p = Mat::zeros(src.cols*src.rows, 5, CV_32F);
    Mat bestLabels, centers, clustered;

    std::vector<Mat> bgr;
    cv::split(src, bgr);

    // i think there is a better way to split pixel bgr color
    for(int i=0; i<src.cols*src.rows; i++) {
        p.at<float>(i,0) = (i/src.cols) / src.rows;
        p.at<float>(i,1) = (i%src.cols) / src.cols;
        p.at<float>(i,2) = bgr[0].data[i] / 255.0;
        p.at<float>(i,3) = bgr[1].data[i] / 255.0;
        p.at<float>(i,4) = bgr[2].data[i] / 255.0;
    }

    int K = 5;
    cv::kmeans(p, K, bestLabels, TermCriteria( TermCriteria::EPS + TermCriteria::MAX_ITER, 15, 1.0), 3, KMEANS_PP_CENTERS, centers);

    int colors[K];
    for(int i=0; i<K; i++) {
        colors[i] = 255/(i+1);
    }
    // i think there is a better way to do this mayebe some Mat::reshape?
    clustered = Mat(src.rows, src.cols, CV_32F);
    for(int i=0; i<src.cols*src.rows; i++)
        clustered.at<float>(i/src.cols, i%src.cols) = (float)(colors[bestLabels.at<int>(0,i)]);

    clustered.convertTo(clustered, CV_8U);

    cv::moveWindow( "Clustered", 0, 500 );
    imshow("Clustered", clustered);
}
//=======================================================================================

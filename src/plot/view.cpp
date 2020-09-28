/*! \file view.cpp
 * \brief View class implementation.
 *
 * \authors Dmitrii Leliuhin
 * \date July 2020
*/

//=======================================================================================

#include "view.h"

#include "vlog.h"



constexpr int thresh_slider_max { 255 };
int thresh_slider { thresh_slider_max / 2};
cv::Mat bin;

const int int_slider_min { 0 };
const int int_slider_max { 100 };
int intensity_slider {0};

const int con_slider_min { 0 };
const int con_slider_max { 100 };
int contrast_slider {33};
cv::Mat enh;

//=======================================================================================
void on_trackbar( int, void* )
{
    cv::moveWindow( "binary", 600, 0 );
    cv::imshow( "binary", bin );

    cv::moveWindow( "enhance", 1200, 0 );
    cv::imshow( "enhance", enh );
}
//=======================================================================================


//=======================================================================================
View::View( const std::string& name, const Config& conf )
    : _name ( name )
{
    if ( conf.main.rotate == 0 )
        _rotate_code = - 1;

    else if ( conf.main.rotate == 90 )
        _rotate_code = cv::ROTATE_90_CLOCKWISE;

    else if ( conf.main.rotate == - 90 )
        _rotate_code = cv::ROTATE_90_COUNTERCLOCKWISE;

    else if ( conf.main.rotate == 180 )
        _rotate_code = cv::ROTATE_180;

    cv::namedWindow( "binary", 1 );
    cv::createTrackbar( "Binarization", "binary",
                        &thresh_slider, thresh_slider_max,
                        on_trackbar );
    cv::setTrackbarMin( "Binarization", "binary", 0 );
    cv::setTrackbarMax( "Binarization", "binary", thresh_slider_max );

    cv::namedWindow( "enhance", 1 );
    cv::createTrackbar( "Intensity", "enhance",
                        &intensity_slider, thresh_slider_max,
                        on_trackbar );
    cv::setTrackbarMin( "Intensity", "enhance", int_slider_min );
    cv::setTrackbarMax( "Intensity", "enhance", int_slider_max );

    cv::namedWindow( "enhance", 2 );
    cv::createTrackbar( "Contrast", "enhance",
                        &contrast_slider, thresh_slider_max,
                        on_trackbar );
    cv::setTrackbarMin( "Contrast", "enhance", con_slider_min );
    cv::setTrackbarMax( "Contrast", "enhance", con_slider_max );
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

    cv::resize( img, img, { 600, 600 } );

    if ( _rotate_code != - 1 )
        cv::rotate( img, img, _rotate_code );

    cv::moveWindow( _name, 0, 0 );
    cv::imshow( _name, img );

    enh = _enhance( img );

    cv::cvtColor( img, img, cv::COLOR_RGB2GRAY );
    cv::threshold( img, img, 0, thresh_slider, cv::THRESH_OTSU );

    bin = img;

    on_trackbar( thresh_slider, 0 );

    auto key = cv::waitKey(10);

    if ( key == 27 )
        vapplication::stop();

    else if ( key == 'p' )
        cv::waitKey();
}
//=======================================================================================
cv::Mat View::_enhance( const cv::Mat& src )
{
    double alpha = static_cast<double>( contrast_slider * 3.0 ) / 100.0;
    int beta = intensity_slider;

    cv::Mat res { cv::Mat::zeros( src.size(), src.type() ) };

    for( int y = 0; y < src.rows; y++ )
        for( int x = 0; x < src.cols; x++ )
            for( int c = 0; c < src.channels(); c++ )
                res.at<cv::Vec3b>(y,x)[c] =
                        static_cast<uchar>( alpha * src.at<cv::Vec3b>(y,x)[c] + beta );

    return res;
}
//=======================================================================================

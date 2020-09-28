/*! \file view.cpp
 * \brief View class implementation.
 *
 * \authors Dmitrii Leliuhin
 * \date July 2020
*/

//=======================================================================================

#include "view.h"

#include "vlog.h"

#include <iostream>

const int alpha_slider_max = 100;
int denoise_slider {0};
int noise_slider {0};

cv::Mat dst;

//=======================================================================================
void on_trackbar( int, void* )
{
    cv::moveWindow( "denoised", 700, 0 );
    cv::imshow( "denoised", dst );
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

    cv::namedWindow( "denoised", 1 );
    cv::createTrackbar( "denoise", "denoised",
                        &denoise_slider, alpha_slider_max,
                        on_trackbar );

    cv::namedWindow( "denoised", 2 );
    cv::createTrackbar( "noise", "denoised",
                        &noise_slider, alpha_slider_max,
                        on_trackbar );
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

    auto type = img.type();

    cv::Mat gauss = cv::Mat( img.size(), CV_16SC3 );
    cv::randn( gauss, cv::Scalar::all(0), cv::Scalar::all( noise_slider ) );

    img.convertTo( img, CV_16SC3 );
    cv::addWeighted( img, 1.0, gauss, 1.0, 0.0, img );

    img.convertTo( img, type );

    dst = img;

    cv::fastNlMeansDenoisingColored( dst, dst, denoise_slider );

    on_trackbar( denoise_slider, 0 );

    auto key = cv::waitKey(10);

    if ( key == 27 )
        vapplication::stop();

    else if ( key == 'p' )
        cv::waitKey();
}
//=======================================================================================

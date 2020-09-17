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

    _brightness_filter( img );

    _border( img );

    auto key = cv::waitKey(10);

    if ( key == 27 )
        vapplication::stop();

    else if ( key == 'p' )
        cv::waitKey();
}
//=======================================================================================


//=======================================================================================
void View::_brightness_filter( const cv::Mat& src )
{
    cv::Mat res;
    cv::Mat bri;
    cv::cvtColor( src, bri, cv::COLOR_RGB2HLS );

    double rnd_lightness_coeff =
            1.0 + static_cast <double> ( rand() ) /
            ( static_cast <double> ( RAND_MAX / ( 1.5 - 1.0 ) ) );

    res.convertTo( res, CV_32F );

    std::vector<cv::Mat> hls;
    cv::split( bri, hls );

    cv::multiply( hls[1], rnd_lightness_coeff, hls[1] );

    cv::merge( hls, res );

    res.convertTo( res, CV_8U );
    cv::cvtColor( res, res, cv::COLOR_HLS2RGB );

    cv::moveWindow( "Brightness filter", 400, 0 );
    cv::imshow( "Brightness filter", res );
}
//=======================================================================================
void View::_border( const cv::Mat& src )
{
    cv::Mat res( cv::Size( src.cols + 20, src.rows + 20 ), src.depth(), src.channels() );

    cv::copyMakeBorder( src, res,
                        50, 50, 50, 50,
                        cv::BORDER_REPLICATE, cv::Scalar( 250 ) );

    cv::moveWindow( "BORDER_REPLICATE", 800, 0 );
    cv::imshow( "BORDER_REPLICATE", res );
}
//=======================================================================================

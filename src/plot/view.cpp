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

    cv::Mat gray;
    cv::cvtColor( img, gray, cv::COLOR_RGB2GRAY );
    cv::threshold( gray, gray, 0, 255, cv::THRESH_OTSU );

    cv::moveWindow( "Binary", 400, 0 );
    cv::imshow( "Binary", gray );

    auto element = cv::getStructuringElement( cv::MORPH_RECT, cv::Size( 4, 4 ), cv::Point( 2, 2 ) );
    cv::morphologyEx( gray, gray, cv::MORPH_DILATE, element, { - 1, - 1 }, 2 );

    cv::moveWindow( "Dilate", 800, 0 );
    cv::imshow( "Dilate", gray );

    cv::morphologyEx( gray, gray, cv::MORPH_ERODE, element, { - 1, - 1 }, 2 );

    cv::moveWindow( "Erode", 1200, 0 );
    cv::imshow( "Erode", gray );

    cv::morphologyEx( gray, gray, cv::MORPH_OPEN, element, { - 1, - 1 }, 2 );

    cv::moveWindow( "Open", 0, 500 );
    cv::imshow( "Open", gray );

    cv::morphologyEx( gray, gray, cv::MORPH_CLOSE, element, { - 1, - 1 }, 2 );
    cv::bitwise_not( gray, gray );

    cv::moveWindow( "Close", 400, 500 );
    cv::imshow( "Close", gray );

    auto key = cv::waitKey(10);

    if ( key == 27 )
        vapplication::stop();

    else if ( key == 'p' )
        cv::waitKey();
}
//=======================================================================================

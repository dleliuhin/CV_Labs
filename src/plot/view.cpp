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
    if ( data.frame1.jpeg.empty() )
        return;

    std::vector<char> buf;
    buf.assign( data.frame1.jpeg.data(),
                data.frame1.jpeg.data() + data.frame1.jpeg.size() );

    auto img = cv::imdecode( buf, cv::IMREAD_COLOR );

    cv::resize( img, img, { 800, 800 } );

    if ( _rotate_code != - 1 )
        cv::rotate( img, img, _rotate_code );

    if ( !data.frame2.jpeg.empty() )
    {
        std::vector<char> raw;
        raw.assign( data.frame2.jpeg.data(),
                      data.frame2.jpeg.data() + data.frame2.jpeg.size() );

        auto other = cv::imdecode( raw, cv::IMREAD_COLOR );

        cv::resize( other, other, { 200, 200 } );

        if ( _rotate_code != - 1 )
            cv::rotate( other, other, _rotate_code );

        other.copyTo( img( cv::Rect( 300, 100, other.cols, other.rows ) ) );
    }

    cv::moveWindow( _name, 0, 0 );
    cv::imshow( _name, img );

    auto key = cv::waitKey(10);

    if ( key == 27 )
        vapplication::stop();

    else if ( key == 'p' )
        cv::waitKey();
}
//=======================================================================================

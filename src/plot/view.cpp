/*! \file view.cpp
 * \brief View class implementation.
 *
 * \authors Dmitrii Leliuhin
 * \date July 2020
*/

//=======================================================================================

#include "view.h"

#include "vlog.h"

const std::map<std::string, cv::ThresholdTypes> decision =
{
    { "THRESH_BINARY",     cv::THRESH_BINARY     },
    { "THRESH_BINARY_INV", cv::THRESH_BINARY_INV },
    { "THRESH_TRUNC",      cv::THRESH_TRUNC      },
    { "THRESH_TOZERO",     cv::THRESH_TOZERO     },
    { "THRESH_TOZERO_INV", cv::THRESH_TOZERO_INV },
    { "THRESH_MASK",       cv::THRESH_MASK       },
    { "THRESH_OTSU",       cv::THRESH_OTSU       },
    { "THRESH_TRIANGLE",   cv::THRESH_TRIANGLE   },
};

//=======================================================================================
View::View( const std::string& name, const Config& conf , QObject* parent )
    : QObject ( parent )
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

    cv::imshow( _name, img );

    _binarize( img );

    auto key = cv::waitKey(10);

    if ( key == 27 )
        vapplication::stop();

    else if ( key == 'p' )
        cv::waitKey();
}
//=======================================================================================
void View::thresold( const uint8_t& value )
{
    _threshold = value;
}
//=======================================================================================
void View::type( const std::string& data )
{
    _type = decision.find( data )->second;
}
//=======================================================================================
void View::_binarize( const cv::Mat& src )
{

}
//=======================================================================================

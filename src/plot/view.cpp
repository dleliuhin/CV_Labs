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
    : _name ( name )
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

    _init_counter();
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

    cv::rotate( img, img, cv::ROTATE_180 );

    cv::imshow( _name, img );

    _calc_colors( img );

    auto key = cv::waitKey(10);

    if ( key == 27 )
        vapplication::stop();

    else if ( key == 'p' )
        cv::waitKey();
}
//=======================================================================================


//=======================================================================================
void View::_calc_colors( const cv::Mat& src )
{
    _init_counter();

    cv::Mat hsv( src.size(), CV_8UC3 );
    cv::cvtColor( src, hsv, cv::COLOR_RGB2HSV );

    for ( auto i = 0; i < hsv.rows; ++i )
        for ( auto j = 0; j < hsv.cols; ++j )
        {
            auto p = hsv.at<cv::Vec3i>( i, j );

            _counter[ _pixel_type( static_cast<uint8_t>( p.val[0] ),
                    static_cast<uint8_t>( p.val[1] ),
                    static_cast<uint8_t>( p.val[2] ) ) ]++;
        }

    std::map<uint16_t, std::string> sorted;

    for ( auto it = _counter.begin(); it != _counter.end(); ++it )
    {
        std::string name;

        if ( it->first == Color::BLACK )
            name = "BLACK";
        else if ( it->first == Color::RED )
            name = "RED";
        else if ( it->first == Color::AQUA )
            name = "AQUA";
        else if ( it->first == Color::BLUE )
            name = "BLUE";
        else if ( it->first == Color::GREY )
            name = "GREY";
        else if ( it->first == Color::GREEN )
            name = "GREEN";
        else if ( it->first == Color::WHITE )
            name = "WHITE";
        else if ( it->first == Color::ORANGE )
            name = "ORANGE";
        else if ( it->first == Color::PURPLE )
            name = "PURPLE";
        else if ( it->first == Color::YELLOW )
            name = "YELLOW";

        sorted.emplace( it->second, name );
    }

    vdeb << "-----------------";

    for ( auto it = sorted.rbegin(); it != sorted.rend(); ++it )
        vdeb << it->second << " = " << it->first;

    vdeb << "-----------------";
}
//=======================================================================================
void View::_init_counter()
{
    _counter =
    {
        { Color::BLACK,  0 },
        { Color::WHITE,  0 },
        { Color::GREY,   0 },
        { Color::RED,    0 },
        { Color::ORANGE, 0 },
        { Color::YELLOW, 0 },
        { Color::AQUA,   0 },
        { Color::BLUE,   0 },
        { Color::PURPLE, 0 },
    };
}
//=======================================================================================
Color View::_pixel_type( const uint8_t& h, const uint8_t& s, const uint8_t& v )
{
    Color res { Color::BLACK };

    if ( v < 75 )
        res = Color::BLACK;
    else if ( v > 190 && s < 27 )
        res = Color::WHITE;
    else if ( s < 53 && v < 185 )
        res = Color::GREY;
    else if ( h < 7 )
        res = Color::RED;
    else if ( h < 25 )
        res = Color::ORANGE;
    else if ( h < 34 )
        res = Color::YELLOW;
    else if ( h < 73 )
        res = Color::GREEN;
    else if ( h < 102 )
        res = Color::AQUA;
    else if ( h < 140 )
        res = Color::BLUE;
    else if ( h < 170 )
        res = Color::PURPLE;
    else
        res = Color::RED;

    return res;
}
//=======================================================================================

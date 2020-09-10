/*! \file view.cpp
 * \brief View class implementation.
 *
 * \authors Dmitrii Leliuhin
 * \date July 2020
*/

//=======================================================================================

#include "view.h"

#include "vlog.h"

using namespace pcl::visualization;

//=======================================================================================

static constexpr uint16_t timeout = 100;

//=======================================================================================
View::View( const std::string& name, const Config& conf )
    : _conf ( conf )
{
    _image.setWindowTitle( name );
    _image.setSize( 1600, 900 );
}
//=======================================================================================
View::~View()
{
    _image.close();
    vapplication::stop();
}
//=======================================================================================


//=======================================================================================
void View::run()
{
    while( !_image.wasStopped() )
    {

    }
}
//=======================================================================================


//=======================================================================================
void View::plot( const Pack& data )
{
    vdeb << "Plot data";

    _image.showRGBImage( data.frame.jpeg.data(),
                         _conf.receive.frame_width,
                         _conf.receive.frame_height );

    _image.spinOnce( timeout );
}
//=======================================================================================

/*! \file view.h
 * \brief View class interface.
 *
 * Data PCL viewer.
 *
 * \authors Dmitrii Leliuhin
 * \date July 2020
 */

//=======================================================================================

#pragma once

#include "config.h"
#include "pack.h"

#include "vapplication.h"
#include "vsignal.h"

#include <pcl/visualization/image_viewer.h>

#include <string>

//=======================================================================================
/*! \class View
 * \brief Data PCL viewer.
 * \details Interconnects with the rest of the application classes to render results.
 */
class View
{
public:

    /*!
     * \param[in] name Plot widget title.
     * \details Initialize _plot PCL visualizer basic options.
     */
    View( const std::string& name = {}, const Config& conf = {} );

    /*! \brief default destructor.
     * \details Close plot widget and stops the application.
     */
    ~View();

    //-----------------------------------------------------------------------------------

    /*! \fn void run();
     * \brief Starts viewer loop.
     */
    void run();

    //-----------------------------------------------------------------------------------

    /*! \fn void plot();
     * \brief Draw some 2D/3D data.
     */
    void plot( const Pack& data );

    //-----------------------------------------------------------------------------------

private:

    Config _conf;

    //! \brief Image visualization widget.
    pcl::visualization::ImageViewer _image;
    pcl::PCLImagePtr _pimg;

};
//=======================================================================================

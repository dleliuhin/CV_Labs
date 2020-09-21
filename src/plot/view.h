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

#include <opencv2/opencv.hpp>

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
     * \param[in] conf Configuration parameters.
     * \details Initialize _plot PCL visualizer basic options.
     */
    View( const std::string& name = {}, const Config& conf = {} );

    //! Default destructor.
    ~View() = default;

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

    //! Widget name.
    std::string _name;

    //! OpenCV RotateFlags code.
    int8_t _rotate_code { - 1 };

    //-----------------------------------------------------------------------------------

    void _watershed( const cv::Mat& src );

    void _k_means( const cv::Mat& src );

};
//=======================================================================================

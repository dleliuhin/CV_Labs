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

#include "form.h"

#include <string>

#include <QObject>

//=======================================================================================
/*! \class View
 * \brief Data PCL viewer.
 * \details Interconnects with the rest of the application classes to render results.
 */
class View : public QObject
{
    Q_OBJECT

public:

    /*!
     * \param[in] name Plot widget title.
     * \param[in] conf Configuration parameters.
     * \details Initialize _plot PCL visualizer basic options.
     */
    explicit View( const std::string& name = {},
                   const Config& conf = {},
                   QObject* parent = nullptr );

    //! Default destructor.
    virtual ~View() override = default;

    //-----------------------------------------------------------------------------------

    /*! \fn void run();
     * \brief Starts viewer loop.
     */
    void run();

    //-----------------------------------------------------------------------------------

    void thresold( const uint8_t& value );
    void type( const std::string& data );

    //-----------------------------------------------------------------------------------

public slots:

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

    uint8_t _threshold {0};

    cv::ThresholdTypes _type { cv::THRESH_BINARY };

    //-----------------------------------------------------------------------------------

    void _binarize( const cv::Mat& src );

};
//=======================================================================================

/*! \file pack.h
 * \brief Data package from multiple sensors.
 *
 * \authors Dmitrii Leliuhin
 * \date July 2020
 */

//=======================================================================================

#pragma once

#include "defs.h"

#include "ZcmService.hpp"
#include "ZcmCameraBaslerFrame.hpp"
#include "ZcmCameraBaslerJpegFrame.hpp"

//=======================================================================================
/*! \class Data
 * \brief ZCM message wrapper class.
 */
class Data : ZcmService
{
public:

    /*! \fn Data & operator =( const ZcmService& data );
     * \brief Convert ZcmService data to custom format.
     * \param[in] data Message data.
     * \return Overloaded data;
     */
    Data & operator =( const ZcmService& data );

    //-----------------------------------------------------------------------------------

    /*! \fn void clear();
     * \brief Clear data members.
     */
    void clear();

    //-----------------------------------------------------------------------------------

    /*! \fn const int64_t & timestamp() const;
     * \brief timestamp getter.
     * \return Saved timestamp.
     */
    const int64_t & timestamp() const;

    //-----------------------------------------------------------------------------------

private:

    //! \brief Raw data timestamp.
    int64_t _timestamp {0};

};
//=======================================================================================

using JFrame = ZcmCameraBaslerJpegFrame;

//=======================================================================================
/*! \struct Pack
 * \param data ZCM data entry.
 */
class Pack
{
public:

    //! Raw data from camera.
    JFrame frame;

    //-----------------------------------------------------------------------------------

    /*! \fn void clear();
     * \brief Clear all data entries.
     */
    void clear();

};
//=======================================================================================

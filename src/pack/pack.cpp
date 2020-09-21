/*! \file pack.cpp
 * \brief Pack class implementation.
 *
 * \authors Dmitrii Leliuhin
 * \date July 2020
 */

//=======================================================================================

#include "pack.h"

//=======================================================================================
Data & Data::operator =( const ZcmService& data )
{
    this->_timestamp = data.u_timestamp;

    return *this;
}
//=======================================================================================
void Data::clear()
{
    _timestamp = 0;
}
//=======================================================================================


//=======================================================================================
const int64_t & Data::timestamp() const
{
    return _timestamp;
}
//=======================================================================================


//=======================================================================================
void Pack::clear( JFrame* frame )
{
    frame->info.serial_number.clear();
    frame->info.user_defined_name.clear();
    frame->synchronized = 0;
    frame->timestamp_ns = 0;
    frame->jpeg_size = 0;
    frame->jpeg.clear();
    frame->service.u_timestamp = 0;
    frame->service.processing_time = 0;
}
//=======================================================================================

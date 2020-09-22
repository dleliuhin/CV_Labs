/*! \file subscribe.cpp
 * \brief Subscribe class implementation.
 *
 * \authors Dmitrii Leliuhin
 * \date July 2020
 */

//=======================================================================================

#include "subscribe.h"

#include "vtime_point.h"

//=======================================================================================
Subscribe::Subscribe( zcm::ZCM* zcm, const Config& conf, QObject* parent )
    : QObject ( parent               )
    , _conf   ( conf                 )
{
    _pack.clear();

    _frame.received.link( conf.receive.channel.full,
                            [ this ]( const JFrame& msg )
    {
        _pack.frame = msg;

        emit received( _pack );
    } );
}
//=======================================================================================

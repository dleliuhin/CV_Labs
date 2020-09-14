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
Subscribe::Subscribe( const Config& conf )
    : _conf ( conf                )
    , _zcm  ( conf.receive.target )
{
    _pack.clear();

    _zcm.subscribe<JFrame>( conf.receive.channel.full,
                            [ this ]( const JFrame& msg )
    {
        _pack.frame = msg;

        received( _pack );
    } );
}
//=======================================================================================

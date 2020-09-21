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
    _pack.clear( &_pack.frame1 );
    _pack.clear( &_pack.frame2 );

    _zcm.subscribe<JFrame>( conf.receive.channel1.full,
                            [ this ]( const JFrame& msg )
    {
        _pack.frame1 = msg;

        received( _pack );
    } );

    _zcm.subscribe<JFrame>( conf.receive.channel2.full,
                            [ this ]( const JFrame& msg )
    {
        _pack.frame2 = msg;

        received( _pack );
    } );
}
//=======================================================================================

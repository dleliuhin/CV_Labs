/*! \file config.cpp
 * \brief Config class implementation.
 *
 * \authors Dmitrii Leliuhin
 * \date July 2020
 */

//=======================================================================================

#include "config.h"

#include "vcat.h"

//=======================================================================================
Config::Config()
{
    {
        _schema.subgroup( main.str );

        _schema.add( "debug",  &main.debug  );
        _schema.add( "rotate", &main.rotate );

        _schema.end_subgroup();
    }

    {
        _schema.subgroup( receive.str );

        _schema.add( "target", &receive.target );

        _schema.add( "prefix1", &receive.channel1.prefix );
        _schema.add( "name1",   &receive.channel1.name );

        _schema.add( "prefix2", &receive.channel2.prefix );
        _schema.add( "name2",   &receive.channel2.name );

        _schema.end_subgroup();
    }

    {
        _schema.subgroup( logs.str );

        _schema.add( "need_trace",   &logs.need_trace );
        _schema.add( "need_shared",  &logs.need_shared );
        _schema.add( "shared_name",  &logs.shared_name );
        _schema.add( "need_leveled", &logs.need_leveled );
        _schema.add( "leveled_path", &logs.leveled_path );
        _schema.add( "file_sizes",   &logs.file_sizes );
        _schema.add( "rotates",      &logs.rotates );

        _schema.end_subgroup();
    }

    _fill_ch();
}
//=======================================================================================


//=======================================================================================
void Config::capture( const vsettings& data )
{
    _schema.capture( data );

    _fill_ch();
}
//=======================================================================================
vsettings Config::by_default()
{
    return Config()._schema.build();
}
//=======================================================================================


//=======================================================================================
void Config::_fill_ch()
{
    receive.channel1.full = vcat( receive.channel1.prefix, receive.channel1.name );
    receive.channel2.full = vcat( receive.channel2.prefix, receive.channel2.name );
}
//=======================================================================================


//=======================================================================================
void Config::Logs::setup()
{
    if ( !need_trace )
        return;

    vlog::clear_executers();

    if ( need_shared )
        vlog::set_shared_log( shared_name, file_sizes, rotates );

    if ( need_leveled )
        vlog::set_leveled_log( leveled_path, file_sizes, rotates );
}
//=======================================================================================

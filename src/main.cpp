/*! \file main.cpp
 * \brief Entry app file.
 *
 * \authors Dmitrii Leliuhin
 * \date July 2020
 *
 * Details.
 *
 */

//=======================================================================================

#include "subscribe.h"
#include "config.h"
#ifdef GUI
#include "view.h"
#endif

#include "niias_arguments.h"

#include "vapplication.h"
#include "vthread.h"

#include "vzcm.h"

#include <iostream>

//=======================================================================================
/*! \fn int main( int argc, char **argv )
 * \brief Entry point.
 *
 * Execution of the program starts here.
 *
 * \param[in] argc Number of arguments.
 * \param[in] argv List of arguments.
 *
 * \return App exit status.
 */
int main( int argc, char **argv )
{
    // Parse config && create PID

    niias::arguments nargs( argc, argv,
                            "zcm_img_viewer - JSC NIIAS",
                            Config::by_default() );
    Config config;
    {
        config.capture( nargs.settings() );
        config.logs.setup();
    }

    //-----------------------------------------------------------------------------------

    // Link signals -> slots

    Subscribe subscriber( config );

    //-----------------------------------------------------------------------------------

    // GUI in separate thread

#ifdef GUI
    vthread thread;
    thread.invoke( [&]
    {
        View viewer( nargs.app_name(), config );
        subscriber.received.link( &viewer, &View::plot );
    } );
#endif

    //-----------------------------------------------------------------------------------

    vapplication::poll();

    return EXIT_SUCCESS;
}
//=======================================================================================

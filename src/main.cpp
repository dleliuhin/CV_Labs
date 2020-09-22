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

#include <QApplication>
#endif

#include "niias_arguments.h"

#include "vapplication.h"
#include "vthread.h"

#include <zcm/zcm-cpp.hpp>

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

    QApplication qapp( argc, argv );

    //-----------------------------------------------------------------------------------

    // Link signals -> slots

    zcm::ZCM zcm( config.receive.target );
    Subscribe subscriber( &zcm, config, &qapp );

    //-----------------------------------------------------------------------------------

    View viewer( nargs.app_name(), config, &qapp );

    QObject::connect( &subscriber, &Subscribe::received, &viewer, &View::plot );

#ifdef GUI

    Form form;

    //-------------------------------------------------------------------------------

    QObject::connect( &form, &Form::trackbar, &viewer, &View::thresold );
    QObject::connect( &form, &Form::type, &viewer, &View::type );

    //-------------------------------------------------------------------------------

    form.show();

#endif

    //-----------------------------------------------------------------------------------

    return qapp.exec();
}
//=======================================================================================

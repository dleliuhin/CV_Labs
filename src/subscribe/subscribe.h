/*! \file subscribe.h
 * \brief Subscribe class interface.
 *
 * ZCM message subscriber.
 *
 * \authors Dmitrii Leliuhin
 * \date July 2020
 */

//=======================================================================================

#pragma once

#include "config.h"
#include "pack.h"

#include "vsignal.h"

#include "vzcm_subscriber.h"

#include <QObject>

#include <zcm/zcm-cpp.hpp>

//=======================================================================================
/*! \class Subscribe
 * \brief ZCM message subscriber class.
 */
class Subscribe : public QObject
{
    Q_OBJECT

public:

    /*!
     * \param[in] conf Configuration settings.
     * \details Initialize _zcm node
     */
    explicit Subscribe( zcm::ZCM* zcm, const Config& conf = {}, QObject* parent = nullptr );

    //! \brief default destructor.
    virtual ~Subscribe() override = default;

    //-----------------------------------------------------------------------------------

signals:

    /*!
     * \brief emit signal if ZCM message received.
     * \param Pack Data from ZCM bus.
     */
    void received( const Pack& data );

    //-----------------------------------------------------------------------------------

private:

    //! \brief Configuration parameters.
    Config _conf;

    //! \brief Data aggregated from multiple sensors.
    Pack _pack;

    vzcm_subscriber<JFrame> _frame;

};
//=======================================================================================

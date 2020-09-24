/*! \file view.cpp
 * \brief View class implementation.
 *
 * \authors Dmitrii Leliuhin
 * \date July 2020
*/

//=======================================================================================

#include "view.h"

#include "vlog.h"

//=======================================================================================
View::View( const std::string& name, const Config& conf )
{
    if ( conf.main.rotate == 0 )
        _rotate_code = - 1;

    else if ( conf.main.rotate == 90 )
        _rotate_code = cv::ROTATE_90_CLOCKWISE;

    else if ( conf.main.rotate == - 90 )
        _rotate_code = cv::ROTATE_90_COUNTERCLOCKWISE;

    else if ( conf.main.rotate == 180 )
        _rotate_code = cv::ROTATE_180;
}
//=======================================================================================


//=======================================================================================
void View::plot( const Pack& data )
{
    if ( data.frame.jpeg.empty() )
        return;

    std::vector<char> buf;
    buf.assign( data.frame.jpeg.data(),
                data.frame.jpeg.data() + data.frame.jpeg.size() );

    auto img = cv::imdecode( buf, cv::IMREAD_COLOR );

    cv::resize( img, img, { 600, 600 } );

    if ( _rotate_code != - 1 )
        cv::rotate( img, img, _rotate_code );

    cv::moveWindow( _name, 0, 0 );
    cv::imshow( _name, img );

    cv::Mat plot = img;

    //-----------------------------------------------------------------------------------

    if ( !_was_init )
    {
        _bbox = cv::selectROI( _name, img, false );

        _bbox_mil = _bbox;
        _bbox_kcf = _bbox;
        _bbox_tld = _bbox;
        _bbox_csrt = _bbox;
        _bbox_mosse = _bbox;
        _bbox_boosting = _bbox;
        _bbox_medianflow = _bbox;

        _tracker_kcf = cv::TrackerKCF::create();
        _tracker_kcf->init( plot, _bbox_kcf );

        _tracker_mil = cv::TrackerMIL::create();
        _tracker_mil->init( plot, _bbox_mil );

        _tracker_tld = cv::TrackerTLD::create();
        _tracker_tld->init( plot, _bbox_tld );

        _tracker_csrt = cv::TrackerCSRT::create();
        _tracker_csrt->init( plot, _bbox_csrt );

        _tracker_mosse = cv::TrackerMOSSE::create();
        _tracker_mosse->init( plot, _bbox_mosse );

        _tracker_boosting = cv::TrackerBoosting::create();
        _tracker_boosting->init( plot, _bbox_boosting );

        _tracker_medianflow = cv::TrackerMedianFlow::create();
        _tracker_medianflow->init( plot, _bbox_medianflow );

        _was_init = true;
    }

    if ( _tracker_kcf->update( plot, _bbox_kcf ) )
    {
        cv::rectangle( plot, _bbox_kcf, cv::Scalar( 0, 0, 0 ), 2 );
        cv::putText( plot, "KCF", cv::Point( _bbox_kcf.x, _bbox_kcf.y ), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar( 0, 0, 0 ), 1 );
    }

    if ( _tracker_mil->update( plot, _bbox_mil ) )
    {
        cv::rectangle( plot, _bbox_mil, cv::Scalar( 255, 0, 0 ), 2 );
        cv::putText( plot, "MIL", cv::Point( _bbox_mil.x, _bbox_mil.y ), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar( 255, 0, 0 ), 1 );
    }

    if ( _tracker_tld->update( plot, _bbox_tld ) )
    {
        cv::rectangle( plot, _bbox_tld, cv::Scalar( 0, 0, 255 ), 2 );
        cv::putText( plot, "TLD", cv::Point( _bbox_tld.x, _bbox_tld.y ), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar( 0, 0, 255 ), 1 );
    }

    if ( _tracker_csrt->update( plot, _bbox_csrt ) )
    {
        cv::rectangle( plot, _bbox_csrt, cv::Scalar( 255, 255, 0 ), 2 );
        cv::putText( plot, "CSRT", cv::Point( _bbox_csrt.x, _bbox_csrt.y ), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar( 255, 255, 0 ), 1 );
    }

    if ( _tracker_mosse->update( plot, _bbox_mosse ) )
    {
        cv::rectangle( plot, _bbox_mosse, cv::Scalar( 0, 255, 255 ), 2 );
        cv::putText( plot, "MOSSE", cv::Point( _bbox_mosse.x, _bbox_mosse.y ), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar( 0, 255, 255 ), 1 );
    }

    if ( _tracker_boosting->update( plot, _bbox_boosting ) )
    {
        cv::rectangle( plot, _bbox_boosting, cv::Scalar( 0, 255, 255 ), 2 );
        cv::putText( plot, "Boosting", cv::Point( _bbox_boosting.x, _bbox_boosting.y ), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar( 0, 255, 0 ), 1 );
    }

    if ( _tracker_medianflow->update( plot, _bbox_medianflow ) )
    {
        cv::rectangle( plot, _bbox_medianflow, cv::Scalar( 102, 200, 153 ), 2 );
        cv::putText( plot, "MedianFlow", cv::Point( _bbox_medianflow.x, _bbox_medianflow.y ), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar( 102, 200, 153 ), 1 );
    }

    cv::moveWindow( "Trackers", 700, 0 );
    cv::imshow( "Trackers", plot );

    //-----------------------------------------------------------------------------------

    auto key = cv::waitKey(10);

    if ( key == 27 )
        vapplication::stop();

    else if ( key == 'p' )
        cv::waitKey();
}
//=======================================================================================

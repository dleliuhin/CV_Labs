/*! \file view.cpp
 * \brief View class implementation.
 *
 * \authors Dmitrii Leliuhin
 * \date July 2020
*/

//=======================================================================================

#include "view.h"

#include "vlog.h"

static constexpr uint16_t dim = 400;

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

    cv::resize( img, img, { dim, dim } );

    if ( _rotate_code != - 1 )
        cv::rotate( img, img, _rotate_code );

    cv::moveWindow( _name, 0, 0 );
    cv::imshow( _name, img );

    _intensity_hist( img, "intensity gray", true, 1, cv::Scalar( 0, 0, 0 ) );

    _intensity_channels( img );

    _equalize( img );

    auto key = cv::waitKey(10);

    if ( key == 27 )
        vapplication::stop();

    else if ( key == 'p' )
        cv::waitKey();
}
//=======================================================================================


//=======================================================================================
void View::_intensity_hist( const cv::Mat& src,
                            const std::string& name,
                            const bool& rgb,
                            const uint8_t& part,
                            const cv::Scalar& color )
{
    cv::Mat buf;

    if ( rgb )
        cv::cvtColor( src, buf, cv::COLOR_RGB2GRAY );
    else
        buf = src;

    std::vector<uint8_t> histogram( 256, 0 );

    for ( auto i = 0; i < buf.rows; ++i )
        for ( auto j = 0; j < buf.cols; ++j )
            histogram[ (int) buf.at<uchar>( i, j ) ]++;

    cv::Mat hist_img( dim, dim, CV_8UC1, cv::Scalar( 255, 255, 255 ) );

    auto max_intensity = *std::max_element( histogram.begin(), histogram.end(),
                                            [&]( const uint8_t& e1, const uint8_t& e2 )
    {
        return e1 < e2;
    } );

    for( auto i = 0; i < histogram.size(); ++i )
        histogram[i] = ( (double) histogram.at(i) / max_intensity ) * hist_img.rows;

    auto bin_w = cvRound( static_cast<double>( 512 / histogram.size() ) );

    for( auto i = 0; i < histogram.size(); ++i )
        cv::line( hist_img,
                  cv::Point( bin_w * i, dim ),
                  cv::Point( bin_w * i, dim - histogram.at(i) ),
                  color, 2, 8, 0 );

    cv::moveWindow( _name + name, part * dim, 0 );
    cv::imshow( _name + name, hist_img );
}
//=======================================================================================
void View::_intensity_channels( const cv::Mat& src )
{
    cv::cvtColor( src, src, cv::COLOR_RGB2BGR );
    std::vector<cv::Mat> channels;
    cv::split( src, channels );

    /// Establish the number of bins
    int histSize = 256;

    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };

    bool uniform = true; bool accumulate = false;

    cv::Mat b_hist, g_hist, r_hist;

    /// Compute the histograms:
    calcHist( &channels[0], 1, 0, {}, b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &channels[1], 1, 0, {}, g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &channels[2], 1, 0, {}, r_hist, 1, &histSize, &histRange, uniform, accumulate );

    // Draw the histograms for B, G and R
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );

    cv::Mat hist_img( hist_h, hist_w, CV_8UC3, cv::Scalar( 0,0,0) );

    /// Normalize the result to [ 0, histImage.rows ]
    cv::normalize(b_hist, b_hist, 0, hist_img.rows, cv::NORM_MINMAX, -1, {} );
    cv::normalize(g_hist, g_hist, 0, hist_img.rows, cv::NORM_MINMAX, -1, {} );
    cv::normalize(r_hist, r_hist, 0, hist_img.rows, cv::NORM_MINMAX, -1, {} );

    /// Draw for each channel
    for( int i = 1; i < histSize; i++ )
    {
        cv::line( hist_img, cv::Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                  cv::Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                  cv::Scalar( 255, 0, 0), 2, 8, 0  );
        cv::line( hist_img, cv::Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                  cv::Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                  cv::Scalar( 0, 255, 0), 2, 8, 0  );
        cv::line( hist_img, cv::Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                  cv::Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                  cv::Scalar( 0, 0, 255), 2, 8, 0  );
    }

    cv::moveWindow( "Channels intensity", 2 * dim, 0 );
    cv::imshow( "Channels intensity", hist_img );
}
//=======================================================================================
void View::_equalize( const cv::Mat& src )
{
    {
        cv::Mat equ = src;
        std::vector<cv::Mat> channels;
        cv::split( equ, channels );

        cv::equalizeHist( channels[0], channels[0] );
        cv::equalizeHist( channels[1], channels[1] );
        cv::equalizeHist( channels[2], channels[2] );

        cv::merge( channels, equ );
        cv::cvtColor( equ, equ, cv::COLOR_BGR2RGB );

        cv::moveWindow( "Equalized RGB", 0, 500 );
        cv::imshow( "Equalized RGB", equ );
    }

    {
        cv::Mat ycc;
        cv::cvtColor( src, ycc, cv::COLOR_RGB2YCrCb );

        std::vector<cv::Mat> channels;
        cv::split( ycc, channels );

        cv::equalizeHist( channels[0], channels[0] );

        cv::merge( channels, ycc );
        cv::cvtColor( ycc, ycc, cv::COLOR_YCrCb2RGB );

        cv::moveWindow( "Equalized YCrCb", dim, 500 );
        cv::imshow( "Equalized YCrCb", ycc );
    }

    {
        cv::Mat lab;
        cv::cvtColor( src, lab, cv::COLOR_RGB2Lab );

        std::vector<cv::Mat> channels;
        cv::split( lab, channels );

        auto clahe = cv::createCLAHE(4);

        clahe->apply( channels[0], channels[0] );
        cv::merge( channels, lab );
        cv::cvtColor( lab, lab, cv::COLOR_Lab2RGB );

        cv::moveWindow( "Equalized CLAHE", 2 * dim, 500 );
        cv::imshow( "Equalized CLAHE", lab );
    }
}
//=======================================================================================

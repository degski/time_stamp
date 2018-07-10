
// MIT License
//
// Copyright (c) 2018 degski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <ciso646>
#include <cstdio>
#include <ctime>

#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

#include <iostream>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

#include <boost/nowide/args.hpp>
#include <boost/nowide/iostream.hpp>


bool g_local = false, g_time = false, g_date = false, g_year = false, g_month = false, g_year_month = false;

std::vector<std::string> g_names;


inline void getOptions ( int argc, char ** argv ) {

    const std::string title = std::string ( "\n  " ) + std::string ( argv [ 0 ] ) + std::string ( " 4.0 - (c) degski 2018\n\n" ) + std::string ( "  options" );

    po::options_description desc ( title );

    desc.add_options ( )

        ( "help,h", "This help screen" )
        ( "file,f", po::value<std::vector<std::string>> ( &g_names ), "File(s) or directories, to be time-stamped" )
        ( "local,l", po::bool_switch ( &g_local ), "Local Time (default (no parameters) is UTC) (yyyymmddhhmmss)" )
        ( "utc,u", po::bool_switch ( &g_time ), "UTC (hh::mm:ss)" )
        ( "date,d", po::bool_switch ( &g_date ), "Date (dd.mm.yyyy)" )
        ( "month,m", po::bool_switch ( &g_month ), "Month (mm)" )
        ( "year,y", po::bool_switch ( &g_year ), "Year (yyyy)" )
        ( "release,r", po::bool_switch ( &g_year_month ), "Year and Month (yymm) for release stamping" );

    po::positional_options_description pos_desc;

    pos_desc.add ( "file", -1 );

    po::variables_map vm;

    po::store ( po::command_line_parser ( argc, argv ).options ( desc ).run ( ), vm );
    po::store ( po::command_line_parser ( argc, argv ).options ( desc ).positional ( pos_desc ).run ( ), vm );

    po::notify ( vm );

    if ( g_names.size ( ) ) {

        g_time = false, g_date = false, g_year = false, g_month = false, g_year_month = false;
    }

    if ( vm.count ( "help" ) ) {

        std::cout << desc << std::endl;
        std::exit ( 0 );
    }
}


std::string getTimestamp ( ) {

    time_t rawtime = time ( NULL );
    struct tm ptm;

    if ( g_local == false ) {

        gmtime_s ( &ptm, &rawtime );
    }

    else {

        localtime_s ( &ptm, &rawtime );
    }

    char buffer [ 64 ] { 0 };

    if ( g_date == false and g_time == false and g_year == false and g_month == false and g_year_month == false ) {

        snprintf ( buffer, 64, "%4i%02i%02i%02i%02i%02i", ptm.tm_year + 1900, ptm.tm_mon + 1, ptm.tm_mday, ptm.tm_hour, ptm.tm_min, ptm.tm_sec );
    }

    else if ( g_date == true and g_time == false and g_year == false and g_month == false and g_year_month == false ) {

        snprintf ( buffer, 64, "%02i.%02i.%04i", ptm.tm_mday, ptm.tm_mon + 1, ptm.tm_year + 1900 );
    }

    else if ( g_date == false and g_time == true and g_year == false and g_month == false and g_year_month == false ) {

        snprintf ( buffer, 64, "%02i:%02i:%02i", ptm.tm_hour, ptm.tm_min, ptm.tm_sec );
    }

    else if ( g_date == true and g_time == true and g_year == false and g_month == false and g_year_month == false ) {

        snprintf ( buffer, 64, "%02i.%02i.%04i/%02i:%02i:%02i", ptm.tm_mday, ptm.tm_mon + 1, ptm.tm_year + 1900, ptm.tm_hour, ptm.tm_min, ptm.tm_sec );
    }

    else if ( g_year == true ) {

        snprintf ( buffer, 64, "%4i", ptm.tm_year + 1900 );
    }

    else if ( g_month == true ) {

        snprintf ( buffer, 64, "%02i", ptm.tm_mon + 1 );
    }

    else if ( g_year_month == true ) {

        snprintf ( buffer, 64, "%02i%02i", ptm.tm_year - 100, ptm.tm_mon + 1 );
    }

    return std::string ( buffer );
}


int main ( int argc, char ** argv ) {

    boost::nowide::args utf8_arguments ( argc, argv );

    getOptions ( argc, argv );

    if ( g_names.size ( ) ) {

        const std::string time_stamp ( getTimestamp ( ) );

        int count = 0;

        for ( auto & name : g_names ) {

            fs::path old_file_name = name;

            if ( not ( old_file_name.is_absolute ( ) ) and not ( old_file_name.is_relative ( ) ) ) {

                old_file_name = fs::current_path ( ) / name;
            }

            if ( fs::exists ( old_file_name ) ) {

                const fs::path new_file_name = old_file_name.parent_path ( ) / std::string ( old_file_name.stem ( ).string ( ) + std::string ( "-" ) + time_stamp + old_file_name.extension ( ).string ( ) );

                fs::rename ( old_file_name, new_file_name );

                ++count;
            }
        }

        switch ( count ) {

            case 0: std::cout << "\n No files were time-stamped.\n"; break;
            case 1: std::cout << "\n One file was time-stamped.\n"; break;

            default: std::cout << "\n " << count << " files were time-stamped.\n";
        }
    }

    else {

        std::cout << getTimestamp ( );
    }

    return 0;
}

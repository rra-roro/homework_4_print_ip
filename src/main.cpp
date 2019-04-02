#include "print_ip.h"
#include <iostream>
#include <vector>
#include <list>
#include "lib_version.h"
#include "CLParser.h"

using namespace std;
using namespace roro_lib;

void help()
{
      cout << R"(
 This demo is my print_ip.

    print_ip  [-version | -? ]
       Options:  
       -version    -get version of program
       -?          -about program (this info)     
)" << endl;
}

void version_print_ip()
{
      cout << "Version print_ip: " << version() << endl;
}



#ifndef _TEST

int main(int argc, char* argv[])
{
      try
      {
            ParserCommandLine PCL;
            PCL.AddFormatOfArg("?", no_argument, '?');
            PCL.AddFormatOfArg("help", no_argument, '?');
            PCL.AddFormatOfArg("version", no_argument, 'v');

            PCL.SetShowError(false);
            PCL.Parser(argc, argv);

            if (PCL.Option['?'])
            {
                  help();
                  return 0;
            }
            if (PCL.Option['v'])
            {
                  version_print_ip();
                  return 0;
            }

            output_ip(static_cast<char>(-1));
            output_ip(static_cast<short>(0));
            output_ip(static_cast<int>(2130706433));
            output_ip(static_cast<long long>(8875824491850138409));
            output_ip(std::string("192.168.0.10"));
            output_ip(std::vector<char>{ -1, -1, -1, -1 });
            output_ip(std::list<unsigned short>{ 49320, 257 });
            output_ip(std::make_tuple<std::int8_t, std::int8_t, std::int8_t, std::int8_t>(1, 1, 1, 1));

      }
      catch (const std::exception& ex)
      {
            cerr << "Error: " << ex.what() << endl;
            return EXIT_FAILURE;
      }
      catch (...)
      {
            cerr << "Error: unknown exception" << endl;
            return EXIT_FAILURE;
      }

      return 0;
}

#endif

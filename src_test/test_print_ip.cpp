#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <deque>
#include <forward_list>
#include "gtest/gtest.h"
#include "gtest/gtest_prod.h"

#include "lib_version.h"
#include "print_ip.h"

#define _TEST 1



TEST(version, test1)
{
      ASSERT_TRUE(version() > 0);
}

TEST(output_ip, integral)
{
      std::stringstream sstr;
      roro_lib::output_ip(static_cast<char>(-1), sstr);      
      ASSERT_TRUE(sstr.str() == "255\n");

      std::stringstream().swap(sstr);
      roro_lib::output_ip(static_cast<short>(-2), sstr);
      ASSERT_TRUE(sstr.str() == "255.254\n");

      std::stringstream().swap(sstr);
      roro_lib::output_ip(static_cast<int>(-1), sstr);
      ASSERT_TRUE(sstr.str() == "255.255.255.255\n");

      std::stringstream().swap(sstr);
      roro_lib::output_ip(static_cast<long long>(8875824491850138409), sstr);
      ASSERT_TRUE(sstr.str() == "123.45.67.89.101.112.131.41\n");

      std::stringstream().swap(sstr);
      roro_lib::output_ip(static_cast<unsigned char>(-1), sstr);
      ASSERT_TRUE(sstr.str() == "255\n");

      std::stringstream().swap(sstr);
      roro_lib::output_ip(static_cast<unsigned short>(-2), sstr);
      ASSERT_TRUE(sstr.str() == "255.254\n");

      std::stringstream().swap(sstr);
      roro_lib::output_ip(static_cast<unsigned int>(-1), sstr);
      ASSERT_TRUE(sstr.str() == "255.255.255.255\n");

      std::stringstream().swap(sstr);
      roro_lib::output_ip(static_cast<unsigned long long>(8875824491850138409), sstr);
      ASSERT_TRUE(sstr.str() == "123.45.67.89.101.112.131.41\n");
}

TEST(output_ip, string)
{
      std::stringstream sstr;
      roro_lib::output_ip(std::string("192.168.4.4"), sstr);
      ASSERT_TRUE(sstr.str() == "192.168.4.4\n");

      std::stringstream().swap(sstr);
      roro_lib::output_ip("192.168.4.4", sstr);
      ASSERT_TRUE(sstr.str() == "192.168.4.4\n");
}

TEST(output_ip, container_vector)
{
      std::stringstream sstr;
      roro_lib::output_ip(std::vector<char>{ -1, -1, -1, -1 }, sstr);
      ASSERT_TRUE(sstr.str() == "255.255.255.255\n");

      std::stringstream().swap(sstr);
      roro_lib::output_ip(std::vector<short>{ 0x0201, 0x01FF }, sstr);
      ASSERT_TRUE(sstr.str() == "2.1.1.255\n");

      std::stringstream().swap(sstr);
      roro_lib::output_ip(std::vector<int>{ 0x0201FF01, 0x02FE02FE }, sstr);
      ASSERT_TRUE(sstr.str() == "2.1.255.1.2.254.2.254\n");

      std::stringstream().swap(sstr);
      roro_lib::output_ip(std::vector<long long>{ 0x02FE02FEFF01FF01}, sstr);
      ASSERT_TRUE(sstr.str() == "2.254.2.254.255.1.255.1\n");
}

TEST(output_ip, container_list)
{
      std::stringstream sstr;
      roro_lib::output_ip(std::list<char>{ -1, -1, -1, -1 }, sstr);
      ASSERT_TRUE(sstr.str() == "255.255.255.255\n");

      std::stringstream().swap(sstr);
      roro_lib::output_ip(std::list<short>{ 0x0201, 0x01FF }, sstr);
      ASSERT_TRUE(sstr.str() == "2.1.1.255\n");

      std::stringstream().swap(sstr);
      roro_lib::output_ip(std::list<int>{ 0x0201FF01, 0x02FE02FE }, sstr);
      ASSERT_TRUE(sstr.str() == "2.1.255.1.2.254.2.254\n");

      std::stringstream().swap(sstr);
      roro_lib::output_ip(std::list<long long>{ 0x02FE02FEFF01FF01 }, sstr);
      ASSERT_TRUE(sstr.str() == "2.254.2.254.255.1.255.1\n");
}

TEST(output_ip, container_deque)
{
      std::stringstream sstr;
      roro_lib::output_ip(std::deque<char>{ -1, -1, -1, -1 }, sstr);
      ASSERT_TRUE(sstr.str() == "255.255.255.255\n");

      std::stringstream().swap(sstr);
      roro_lib::output_ip(std::deque<short>{ 0x0201, 0x01FF }, sstr);
      ASSERT_TRUE(sstr.str() == "2.1.1.255\n");

      std::stringstream().swap(sstr);
      roro_lib::output_ip(std::deque<int>{ 0x0201FF01, 0x02FE02FE }, sstr);
      ASSERT_TRUE(sstr.str() == "2.1.255.1.2.254.2.254\n");

      std::stringstream().swap(sstr);
      roro_lib::output_ip(std::deque<long long>{ 0x02FE02FEFF01FF01 }, sstr);
      ASSERT_TRUE(sstr.str() == "2.254.2.254.255.1.255.1\n");
}

TEST(output_ip, tuple)
{
      std::stringstream sstr;
      roro_lib::output_ip(std::tuple<char,char,char,char>{ -1, -1, -1, -1 }, sstr);
      ASSERT_TRUE(sstr.str() == "255.255.255.255\n");

      std::stringstream().swap(sstr);
      roro_lib::output_ip(std::tuple<short, short>{ 0x0201, 0x01FF }, sstr);
      ASSERT_TRUE(sstr.str() == "2.1.1.255\n");

      std::stringstream().swap(sstr);
      roro_lib::output_ip(std::tuple<int, int>{ 0x0201FF01, 0x02FE02FE }, sstr);
      ASSERT_TRUE(sstr.str() == "2.1.255.1.2.254.2.254\n");

      std::stringstream().swap(sstr);
      roro_lib::output_ip(std::tuple<long long>{ 0x02FE02FEFF01FF01 }, sstr);
      ASSERT_TRUE(sstr.str() == "2.254.2.254.255.1.255.1\n");
}

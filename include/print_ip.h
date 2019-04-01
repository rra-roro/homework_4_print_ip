#pragma once
#include <iostream>
#include <array>
#include <string_view>
#include <vector>

// Compile-time ф-ия конвертации числа в массив байт.
// Cледующие компиляторы выполняют вычиcление этой ф-ии в сompile-time:
//     GCC 7.2 - yes
//     Clang 5.0 - yes
//     MSVC2017 19.16 - no

namespace roro_lib
{
      template <typename T>
      constexpr std::array<std::uint8_t, sizeof(T)> GetArrayByteFromValue(T value) // Предположим аргумент ф-ии это: int value = 0x55443322;
      {
            if constexpr (sizeof(T) == 1)
                  return { static_cast<std::uint8_t>(value) };
            else
            {
                  using uT = typename std::make_unsigned<T>::type; // using uT = unsigned int;

                  std::array<std::uint8_t, sizeof(T)> arr = { 0 }; // std::array<std::uint8_t, 4> arr = { 0 };

                  const uT mask = ~(static_cast<uT>(~0LL) >> 8); // получаем маску вида 0xFF000000

                  for (int i = 0; i < sizeof(value); i++)
                  {
                        const uT tmp = value & (mask >> (i * 8));                                 // шаг1: tmp = 0x55000000;  шаг2: tmp = 0x00440000; шаг3: tmp = 0x00003300; шаг4: tmp = 0x00000022;
                        arr[i] = static_cast<std::uint8_t>(tmp >> (sizeof(value) - (i + 1)) * 8); // шаг1: arr[0] = 0x55;  шаг2: arr[1] = 0x44; шаг3: arr[2] = 0x33; шаг4: arr[3] = 0x22;
                  }

                  return arr;
            }
      }


      template <typename T,
          typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
      constexpr void print_ip(const T& value)
      {
            auto byte_array = GetArrayByteFromValue(value);

            for (size_t i = 0; i < byte_array.size(); i++)
            {
                  std::cout << +byte_array[i];
                  if ((i + 1) != byte_array.size())
                        std::cout << ".";
                  else
                        std::cout << "\n";
            }
      }

      void print_ip(std::string_view value)
      {
            std::cout << value << "\n";
      }


      template <typename T, typename Al, template <typename, typename> typename C>
      constexpr void print_ip(const C<T, Al>& value)
      {
            std::vector<std::uint8_t> vec(sizeof(T) * value.size());

            auto It = vec.begin();

            for (const auto& item : value)
            {
                  auto byte_array = GetArrayByteFromValue(item);
                  It = copy(byte_array.begin(), byte_array.end(), It);
            }

            for (size_t i = 0; i < vec.size(); i++)
            {
                  std::cout << +vec[i];
                  if ((i + 1) != vec.size())
                        std::cout << ".";
                  else
                        std::cout << "\n";
            }
      }


      //template<typename T, typename... R>
      //constexpr void print_ip(const tuple<T, R...> &value)
      //{
      //	std::vector<std::uint8_t> vec(sizeof(value) * std::tuple_size<tuple<T, R...>>::value);
      //
      //	auto It = vec.begin();
      //	f or (size_t i = 0; i < std::tuple_size<tuple<T, R...>>::value; i++)
      //	{
      //		auto byte_array = GetArrayByteFromValue(std::get<i>(value));
      //		It = copy(byte_array.begin(), byte_array.end(), It);
      //	}
      //
      //	for (int i = 0; i < vec.size(); i++)
      //	{
      //		std::cout << +vec[i];
      //		if ((i + 1) != vec.size()) std::cout << ".";
      //		else std::cout << "\n";
      //	}
      //}

}

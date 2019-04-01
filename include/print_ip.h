#pragma once
#include <iostream>
#include <array>
#include <string_view>
#include <vector>
#include <tuple>

// Compile-time ф-ия конвертации числа в массив байт.
// Cледующие компиляторы выполняют вычиcление этой ф-ии в сompile-time:
//     GCC 7.2 - yes
//     Clang 5.0 - yes
//     MSVC2017 19.16 - no

namespace roro_lib
{
      namespace internal
      {
            template <typename T>
            constexpr std::array<std::uint8_t, sizeof(T)> GetArrayByteFromValue(T value)
            {
                  if constexpr (sizeof(T) == 1)
                        return { static_cast<std::uint8_t>(value) };
                  else
                  {
                        using uT = typename std::make_unsigned<T>::type;

                        std::array<std::uint8_t, sizeof(T)> arr = { 0 };

                        for (int i = sizeof(value) - 1; i >= 0; i--)
                        {
                              arr[sizeof(value) - i - 1] = static_cast<std::uint8_t>(value >> (i * 8));
                        }

                        return arr;
                  }
            }

            template <typename T>
            inline void output_ip_from(T& container)
            {
                  for (std::size_t i = 0; i < container.size(); i++)
                  {
                        std::cout << +container[i];
                        if ((i + 1) != container.size())
                              std::cout << ".";
                        else
                              std::cout << "\n";
                  }
            }
      }



      template <typename T,
          typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
      constexpr void print_ip(const T& value)
      {
            static_assert(std::is_integral<T>::value == true);

            auto byte_array = internal::GetArrayByteFromValue(value);

            internal::output_ip_from(byte_array);
      }

      void print_ip(std::string_view value)
      {
            std::cout << value << "\n";
      }


      template <typename T, typename Al, template <typename, typename> typename C>
      constexpr void print_ip(const C<T, Al>& value)
      {
            static_assert(std::is_integral<T>::value == true);

            std::vector<std::uint8_t> vec(sizeof(T) * value.size());

            auto It = vec.begin();

            for (const auto& item : value)
            {
                  auto byte_array = internal::GetArrayByteFromValue(item);
                  It = copy(byte_array.begin(), byte_array.end(), It);
            }

            internal::output_ip_from(vec);
      }

      template <std::size_t I = 0, typename It, typename... Tp>
      inline typename std::enable_if<I == sizeof...(Tp), void>::type for_each(const std::tuple<Tp...>&, It&)
      {
      }

      template <std::size_t I = 0, typename It, typename... Tp>
          inline typename std::enable_if < I<sizeof...(Tp), void>::type for_each(const std::tuple<Tp...>& t, It& it)
      {
            auto byte_array = internal::GetArrayByteFromValue(std::get<I>(t));
            it = copy(byte_array.begin(), byte_array.end(), it);

            for_each<I + 1, It, Tp...>(t, it);
      }

      template <typename T, typename... R>
      constexpr void print_ip(const std::tuple<T, R...>& value)
      {
            std::vector<std::uint8_t> vec(sizeof(T) * sizeof...(R) + 1);
            auto it = vec.begin();

            for_each<0, decltype(it)>(value, it);

            internal::output_ip_from(vec);
      }

}

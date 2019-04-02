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
                        std::array<std::uint8_t, sizeof(T)> arr = { 0 };

                        const std::size_t size_value = sizeof(value) - 1;

                        for (int i = size_value; i >= 0; i--)
                        {
                              arr[size_value - i] = static_cast<std::uint8_t>(value >> (i * 8));
                        }

                        return arr;
                  }
            }

            template <std::size_t I = 0, typename It, typename... Tp>
            constexpr void for_each(const std::tuple<Tp...>& t, It& it)
            {
                  if constexpr (I < sizeof...(Tp))
                  {
                        auto byte_array = GetArrayByteFromValue(std::get<I>(t));
                        it = copy(byte_array.begin(), byte_array.end(), it);

                        for_each<I + 1, It, Tp...>(t, it);
                  }
            }

            template <typename T>
            inline void output_ip_from(const T& container, std::ostream& os)
            {
                  const std::size_t  size_container = container.size() - 1;
                  for (std::size_t i = 0; i <= size_container; i++)
                  {
                        os << +container[i];
                        os << ((i != size_container) ? "." : "\n");
                  }
            }
      }


      template <typename T,
                typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
      constexpr void output_ip(const T& value, std::ostream& os = std::cout)
      {
            auto byte_array = internal::GetArrayByteFromValue(value);
            internal::output_ip_from(byte_array, os);
      }

      void output_ip(std::string_view str_ip, std::ostream& os = std::cout)
      {
            os << str_ip << "\n";
      }


      template <typename T, typename Al, template <typename, typename> typename C>
      constexpr void output_ip(const C<T, Al>& cont, std::ostream& os = std::cout)
      {
            static_assert(std::is_integral<T>::value == true);

            std::vector<std::uint8_t> vec(sizeof(T) * cont.size());

            auto It = vec.begin();

            for (const auto& item : cont)
            {
                  auto byte_array = internal::GetArrayByteFromValue(item);
                  It = copy(byte_array.begin(), byte_array.end(), It);
            }

            internal::output_ip_from(vec, os);
      }

      template <typename T, typename... R>
      constexpr void output_ip(const std::tuple<T, R...>& tp, std::ostream& os = std::cout)
      {
            static_assert(std::is_integral<T>::value == true);

            std::array<std::uint8_t, sizeof(T) * sizeof...(R) + 1> arr = { 0 };
            auto it = arr.begin();

            internal::for_each<0, decltype(it)>(tp, it);

            internal::output_ip_from(arr, os);
      }

}

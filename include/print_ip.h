#pragma once
#include <iostream>
#include <array>
#include <string_view>
#include <vector>
#include <tuple>
#include <algorithm>

namespace roro_lib
{
      namespace internal
      {
            /*!   \brief  Constexpr ф-ия разбивает целочисленное  входное значение  на массив  байт            
                          В начале массива размещаются старшие байты целочисленного значения.

                  \param[in] value -целочисленное значение, которое мы хотим разбить на байты
                  \return массив байт, представляющих исходное значение
            */
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

            /*!   \brief  Constexpr ф-ия извлекает все значения из tuple и записывает их 
                          в позицию полученного итератора

                  \param[in] t  -это tuple из которого мы извлекаем значения
                  \param[inout] it  -итератор в позицию которого мы сохраняем значения
            */
            template <std::size_t I = 0, typename It, typename... Tp>
            constexpr void get_all_tuple_items(const std::tuple<Tp...>& t, It& it)
            {
                  if constexpr (I < sizeof...(Tp))
                  {
                        auto byte_array = GetArrayByteFromValue(std::get<I>(t));
                        it = std::copy(byte_array.begin(), byte_array.end(), it);

                        get_all_tuple_items<I + 1, It, Tp...>(t, it);
                  }
            }

            /*!   \brief  Ф-ия которая форматирует вывод полученного массива байт,
                          разделяя выводимые в std::ostream байты точкой.

                  \param[in] container  -откуда мы берем байты
                  \param[in] os  -поток ostream куда мы выводим байты.
            */
            template <typename T>
            inline void output_ip_from(const T& container, std::ostream& os)
            {
                  const std::size_t  end_item = container.size() - 1;
                  for (std::size_t i = 0; i <= end_item; i++)
                  {
                        os << +container[i];
                        os << ((i != end_item) ? "." : "\n");
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

      inline void output_ip(std::string_view str_ip, std::ostream& os = std::cout)
      {
            os << str_ip << "\n";
      }


      template <typename T, typename Al, template <typename, typename> typename C>
      void output_ip(const C<T, Al>& cont, std::ostream& os = std::cout)
      {
            static_assert(std::is_integral_v<T>, "template parameter type of container should be integral type");

            std::vector<std::uint8_t> vec(sizeof(T) * cont.size());

            auto It = vec.begin();

            for (const auto& item : cont)
            {
                  auto byte_array = internal::GetArrayByteFromValue(item);
                  It = std::copy(byte_array.begin(), byte_array.end(), It);
            }

            internal::output_ip_from(vec, os);
      }

      template <typename T, typename... R>
      constexpr void output_ip(const std::tuple<T, R...>& tp, std::ostream& os = std::cout)
      {
            static_assert(std::is_integral_v<T>, "all template parameter types of tuple should be the same integral types");
            static_assert((std::is_same_v<T, R> && ...), "all template parameter types of tuple should be the same integral types");

            std::array<std::uint8_t, sizeof(T) * sizeof...(R) + 1> arr = { 0 };
            auto it = arr.begin();

            internal::get_all_tuple_items<0, decltype(it)>(tp, it);

            internal::output_ip_from(arr, os);
      }

}

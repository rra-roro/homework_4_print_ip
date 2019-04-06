#pragma once
#include <iostream>
#include <array>
#include <string_view>
#include <vector>
#include <tuple>
#include <algorithm>
#include <type_traits>
#include <cstdint>

namespace roro_lib
{
      namespace internal
      {
            /*!   \brief  Constexpr ф-ия разбивает целочисленное  входное значение  на массив  байт.            
                          В начале массива размещаются старшие байты целочисленного значения.

                  \param[in] value -целочисленное значение, которое мы хотим разбить на байты
                  \return массив байт, представляющих исходное значение
            */
            template <typename T>
            constexpr std::array<std::uint8_t, sizeof(T)> get_array_byte_from_value(T value)
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
                          в позицию полученного итератора, предварительно разбивая каждое
                          знаение на отдельные байты

                  \param[in] t  -это tuple из которого мы извлекаем значения
                  \param[inout] it  -итератор в позицию которого мы сохраняем значения
            */
            template <std::size_t I = 0, typename It, typename... Tp>
            constexpr void get_all_tuple_items([[maybe_unused]] const std::tuple<Tp...>& t,
                                               [[maybe_unused]] It& it)
            {                  
                  if constexpr (I < sizeof...(Tp))
                  {
                        auto byte_array = get_array_byte_from_value(std::get<I>(t));
                        it = std::copy(byte_array.begin(), byte_array.end(), it);

                        get_all_tuple_items<I + 1, It, Tp...>(t, it);
                  }
            }

            /*!   \brief  Ф-ия, которая форматирует вывод полученного массива байт,
                          разделяя выводимые в std::ostream байты точкой.

                  \param[in] container  -откуда мы берем байты
                  \param[in] os  -поток ostream, куда мы выводим байты.
            */
            template <typename T>
            inline void output_ip_from(const T& container, std::ostream& os)
            {
                  const std::size_t end_item = container.size() - 1;
                  for (std::size_t i = 0; i <= end_item; i++)
                  {
                        os << +container[i];
                        os << ((i != end_item) ? "." : "\n");
                  }
            }
      }

      /*!   \brief  Ф-ия реализует функцию печати условного ip-адреса,
                    который представлен в виде произвольного целочисленного типа.

            \param[in] value  -целочисленное значение, содержащее ip-адрес
            \param[in] os  -поток ostream, куда мы выводим ip-адрес
      */
      template <typename T,
                typename std::enable_if_t<std::is_integral_v<T>>* = nullptr>
      constexpr void output_ip(const T& value, std::ostream& os = std::cout)
      {
            auto byte_array = internal::get_array_byte_from_value(value);
            internal::output_ip_from(byte_array, os);
      }

      /*!   \brief  Ф-ия реализует функцию печати условного ip-адреса,
                    который представлен в виде строки.

            \param[in] str_ip  -строковое значение, содержащее ip-адрес
            \param[in] os  -поток ostream, куда мы выводим ip-адрес
      */
      inline void output_ip(std::string_view str_ip, std::ostream& os = std::cout)
      {
            os << str_ip << "\n";
      }

      /*!   \brief  Ф-ия реализует функцию печати условного ip-адреса,
                    который содержится в стандартном контейнере:                    
                            vector, deque или list
                    или с любым контейнером, удовлетворяющим условию:
                    1) налиие forward_iterator
                    2) наличие ф-ии члена size()

            \param[in] cont  -контейнер, содержащий ip-адрес
            \param[in] os  -поток ostream, куда мы выводим ip-адрес
      */
      template <typename T, typename Al, template <typename, typename> typename C,
                typename std::enable_if_t<
                                         // следующие два критерия исключают обработку std:: string этой функцией.
                                          std::is_same_v<T, typename C<T, Al>::value_type> &&                     
                                          std::is_same_v<Al, typename C<T, Al>::allocator_type> &&         
                                          std::is_base_of_v<std::forward_iterator_tag,
                                                            typename C<T, Al>::iterator::iterator_category> &&
                                          std::is_member_function_pointer_v<decltype(&C<T, Al>::size)>,
                                          int> = 0>
      void output_ip(const C<T, Al>& cont, std::ostream& os = std::cout)
      {
            static_assert(std::is_integral_v<T>,
                          "template parameter type of container should be integral type");

            std::vector<std::uint8_t> vec(sizeof(T) * cont.size());

            auto It = vec.begin();

            for (const auto& item : cont)
            {
                  auto byte_array = internal::get_array_byte_from_value(item);
                  It = std::copy(byte_array.begin(), byte_array.end(), It);
            }

            internal::output_ip_from(vec, os);
      }

      /*!   \brief  Ф-ия реализует функцию печати условного ip-адреса,
                    который содержится в кортеже

            \param[in] tp  -кортеж, содержащий ip-адрес
            \param[in] os  -поток ostream, куда мы выводим ip-адрес
      */
      template <typename T, typename... R>
      constexpr void output_ip(const std::tuple<T, R...>& tp, std::ostream& os = std::cout)
      {
            static_assert(std::is_integral_v<T> && (std::is_same_v<T, R> && ...),
                          "all template parameter types of tuple should be the same integral types");

            std::array<std::uint8_t, sizeof(T) * std::tuple_size_v<std::tuple<T, R...>>> arr = { 0 };
            auto it = arr.begin();

            internal::get_all_tuple_items<0, decltype(it)>(tp, it);

            internal::output_ip_from(arr, os);
      }

}

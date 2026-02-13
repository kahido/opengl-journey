/*
 *   Author:
 *   Date;
 *   Description:
 */

#ifndef _PROJECT_HEADER_
#define _PROJECT_HEADER_

namespace project
{

template<typename Type, typename... Types>
constexpr auto sum_any_number_of_elements(Type&& first, Types&&... tails)
{
    return (first + ... + tails);
}

}// namespace project

#endif// _PROJECT_HEADER_

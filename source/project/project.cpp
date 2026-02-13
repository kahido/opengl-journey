/*
 *   Author:
 *   Date;
 *   Description:
 */

#include <project/header.hpp>

constexpr int const SUM_RESULT = 6;

int main()
{
    static_assert(project::sum_any_number_of_elements(1, 2, 3) == SUM_RESULT);

    return 0;
}

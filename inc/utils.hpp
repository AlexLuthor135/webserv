/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doduwole <doduwole@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 10:43:46 by doduwole          #+#    #+#             */
/*   Updated: 2024/06/01 10:43:48 by doduwole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <sstream>
#include <algorithm>

class Utils {
public:
    static std::string to_lower(const std::string& s);
    static std::string to_upper(const std::string& s);
    static std::string trim_left(const std::string& str, char c);
    static std::string trim_right(const std::string& str, char c);
    static std::string unique_char(const std::string& str);
    static int stoi(const std::string& str, std::size_t *pos = NULL, int base = 10);

    template<typename T>
    static std::string to_string(T number) {
        std::stringstream ss;
        ss << number;
        return ss.str();
    }

    struct s_compare {
        bool operator() (const std::string& lhs, const std::string& rhs) const {
            return Utils::to_lower(lhs) < Utils::to_lower(rhs);
        };
    };
};

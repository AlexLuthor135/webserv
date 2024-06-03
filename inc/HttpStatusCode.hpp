/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatusCode.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doduwole <doduwole@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 10:42:38 by doduwole          #+#    #+#             */
/*   Updated: 2024/06/01 10:42:40 by doduwole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSTATUSCODES_HPP
#define HTTPSTATUSCODES_HPP

#include "./AllHeaders.hpp"

class HttpStatusCodes {
private:
    std::map<int, std::string> codeMap;

public:
    HttpStatusCodes();
    ~HttpStatusCodes();

    HttpStatusCodes(const HttpStatusCodes &rhs);
    HttpStatusCodes &operator=(const HttpStatusCodes &rhs);
    
    std::string getStatusCode(int code);
};

#endif
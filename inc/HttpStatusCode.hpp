/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpStatusCode.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alappas <alappas@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 20:56:37 by alappas           #+#    #+#             */
/*   Updated: 2024/06/04 20:56:39 by alappas          ###   ########.fr       */
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
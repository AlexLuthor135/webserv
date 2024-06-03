/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doduwole <doduwole@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 10:43:07 by doduwole          #+#    #+#             */
/*   Updated: 2024/06/01 10:43:09 by doduwole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MIMETYPES_HPP
#define MIMETYPES_HPP

#include "./AllHeaders.hpp"

class MimeTypes {
private:
    std::map<std::string, std::string> mimeMap;

public:
    MimeTypes();
    ~MimeTypes();

    MimeTypes(const MimeTypes &rhs);
    MimeTypes &operator=(const MimeTypes &rhs);
    std::string getExt(const std::string& type);

    std::string getType(const std::string& extension);
    std::map<std::string, std::string> getMap();
};

#endif

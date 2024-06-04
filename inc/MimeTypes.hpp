/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alappas <alappas@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 20:56:45 by alappas           #+#    #+#             */
/*   Updated: 2024/06/04 20:56:48 by alappas          ###   ########.fr       */
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

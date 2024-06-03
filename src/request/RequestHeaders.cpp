/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHeaders.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alappas <alappas@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 14:50:06 by alappas           #+#    #+#             */
/*   Updated: 2024/06/03 14:50:09 by alappas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/HttpRequest.hpp"

int HttpRequest::parseHeaders() {

    size_t headerEnd;
    size_t colonPos;
    std::string headerKey;
    std::string headerVal;

    while ((headerEnd = req_buffer_.find("\r\n")) != std::string::npos) {
        if (req_buffer_.find("\r\n") == 0) {
            req_buffer_.erase(0, headerEnd + 2);
            buffer_section_ = SPECIAL_HEADERS;
            break;
        }

        if ((colonPos = req_buffer_.find(':', 0)) != std::string::npos) {
            if (colonPos == 0 || req_buffer_[colonPos - 1] == ' ')
                return 400;
            headerKey = trimmer(req_buffer_.substr(0, colonPos));
            headerVal = trimmer(req_buffer_.substr(colonPos + 1, headerEnd - colonPos - 1));

            std::transform(headerKey.begin(), headerKey.end(), headerKey.begin(), ::tolower);
            
            if (!isValidHeaderFormat(headerKey))
                return 400;

            headers_[headerKey] = headerVal;
        } else {
            return 400;
        }
        req_buffer_.erase(0, headerEnd + 2);
    }

    return 200;
}

bool HttpRequest::isValidHeaderFormat(const std::string& header) {
    if (header.empty())
        return false;

    for (std::string::const_iterator it = header.begin(); it != header.end(); ++it) {
        if (!isValidHeaderChar(*it)) {
            return false;
        }
    }
    return true;
}


bool HttpRequest::isValidHeaderChar(unsigned char c) {
    return (c >= 0x21 && c <= 0x7E) || ((c & 0x80) != 0 && c != 0x7F);
}

int HttpRequest::checkSpecialHeaders() {
    if (headers_.count("host")) {
        std::string value = headers_["host"];
        if (value.empty() || value.find('@') != std::string::npos) {
            std::cerr << "Invalid 'Host' header value." << std::endl;
            return 400;
        }
    }

    if (headers_.count("transfer-encoding")) {
        std::string value = headers_["transfer-encoding"];
        if (value == "chunked") {
            isChunked_ = true;
            chunk_status_ = CHUNK_SIZE;
            buffer_section_ = CHUNK;
        } else {
            return 400;
        }
    } else if (headers_.count("content-length")) {
        std::string value = headers_["content-length"];
        if (value.find_first_not_of("0123456789") != std::string::npos) {
            std::cerr << "Invalid 'Content-Length' header value." << std::endl;
            return 400;
        }
        try {
            std::istringstream iss(value); 
            iss >> length_;
        } catch (const std::exception& e) {
            std::cerr << "Error parsing 'Content-Length' header: " << e.what() << std::endl;
            return 400;
        }
        buffer_section_ = BODY;
    } else {
        return 100;
    }



    if (headers_.count("method")) {
        std::string value = headers_["method"];
        if (value != "POST" && value != "PUT") {
            std::cerr << "Unsupported HTTP method: " << value << std::endl;
            return 100;
        }
    }

    return 200;
}

std::string HttpRequest::trimmer(const std::string& str) {
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    return (start == std::string::npos || end == std::string::npos)
        ? ""
        : str.substr(start, end - start + 1);
}

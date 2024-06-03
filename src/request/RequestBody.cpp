/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestBody.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alappas <alappas@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 14:49:59 by alappas           #+#    #+#             */
/*   Updated: 2024/06/03 14:50:02 by alappas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/HttpRequest.hpp"

int HttpRequest::parseBody()
{

    if (req_buffer_.length() >= length_)
    {
        body_.append(req_buffer_, 0, length_);
        req_buffer_.erase(0, length_);
        body_offset_ += req_buffer_.length();

        return (body_.length() == length_) ? 100 : 400;
    }
    return 200;
}

int HttpRequest::parseChunkedBody()
{
    size_t end;

    while ((end = req_buffer_.find("\r\n")) != std::string::npos)
    {
        if (chunk_status_ == CHUNK_SIZE)
        {
            std::string hex = req_buffer_.substr(0, end);
            int parseResult = parseChunkSize(hex);
            if (parseResult != 0)
                return parseResult;
            req_buffer_.erase(0, end + 2);
            chunk_status_ = CHUNK_BODY;
        }
        else if (chunk_status_ == CHUNK_BODY)
        {
            if (chunk_size_ == 0)
            {
                if (!req_buffer_.empty())
                    return parseChunkTrailer();
                return 100;
            }

            if (req_buffer_.size() >= chunk_size_)
            {
                body_.append(req_buffer_, 0, chunk_size_);
                req_buffer_.erase(0, chunk_size_ + 2);
                chunk_size_ = 0;
                chunk_status_ = CHUNK_SIZE;
            }
            else
            {
                body_.append(req_buffer_);
                chunk_size_ -= req_buffer_.size();
                req_buffer_.clear();
            }
        }
    }
    return 200;
}

int HttpRequest::parseChunkSize(const std::string &hex)
{
    std::istringstream hexStream(hex);
    unsigned long tempChunkSize = 0;
    hexStream >> std::hex >> tempChunkSize;
    
    if (hexStream.fail() || hexStream.bad())
        return 400;

    if (tempChunkSize > static_cast<unsigned long>(std::numeric_limits<size_t>::max()))
        return 400;
    chunk_size_ = static_cast<size_t>(tempChunkSize);
    return 0;
}

int HttpRequest::parseChunkTrailer()
{
    size_t end;
    std::string line;

    while ((end = req_buffer_.find("\r\n")) != std::string::npos)
    {
        line = req_buffer_.substr(0, end);
        req_buffer_.erase(0, end + 2);

        if (line.empty())
        {
            break;
        }
        size_t separatorPos = line.find(':');
        if (separatorPos != std::string::npos)
        {
            std::string key = line.substr(0, separatorPos);
            std::string value = line.substr(separatorPos + 1);

            key = trim(key);
            value = trim(value);
            headers_[key] = value;
        }
        else
        {
            return 400;
        }
    }

    return 100;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResposeMethods.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alappas <alappas@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 14:50:57 by alappas           #+#    #+#             */
/*   Updated: 2024/06/03 14:50:58 by alappas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/HttpResponse.hpp"

pthread_mutex_t g_write;

int HttpResponse::GET()
{ 
    pthread_mutex_lock(&g_write);
    if (!file_)
    {
        std::cerr << "File not found" << std::endl;
        pthread_mutex_unlock(&g_write);
        return 500;
    }

    if (config_.getAutoIndex() && file_->is_directory())
    {
        headers_["Content-Type"] = "text/html; charset=UTF-8";
        body_ = file_->listDir(config_.getRequestTarget());
    }
    else
    {
        std::string mimeType = file_->getMimeType(file_->getMimeExt());
        if (mimeType.empty())
            mimeType = "application/octet-stream";
        headers_["Content-Type"] = mimeType;

        if (!charset_.empty())
            headers_["Content-Type"] += "; charset=" + charset_;

        body_ = file_->getContent();
    }
    headers_["Content-Length"] = ftos(body_.length());
    headers_["Cache-Control"] = "no-cache";
    pthread_mutex_unlock(&g_write);

    return 200;
}

int HttpResponse::POST() {
    int status_code = 500;
    body_ = config_.getBody();

    pthread_mutex_lock(&g_write);
    if (!file_->exists()) {
        file_->createFile(body_);
        status_code = 201;
    } else {
        MimeTypes mimeTypes;
        std::string contentType = config_.getHeader("content-type");
        bool isMultipart = containsBoundary(contentType);
        std::string boundary = isMultipart ? extractBoundary(contentType) : contentType;

        if (isMultipart && !boundary.empty()) {
            std::string filename = extractFilename(body_);
            std::string fileContent = extractContent(body_, boundary);
            body_.clear();
            body_.append(fileContent);
            if (!filename.empty() && !fileContent.empty()) {
                file_->appendFile(fileContent, filename);
                status_code = 201;
            } else {
                status_code = 400;
            }
        } else {
            std::string ext = mimeTypes.getType(boundary);
            if (ext.empty()) {
                std::cerr << "Invalid content type" << std::endl;
                status_code = 415;
            } else {
                std::string default_name = "default" + ext;
                std::string x_filename = config_.getHeader("X-Filename");
                x_filename = x_filename.empty() ? default_name : x_filename;
                file_->appendFile(body_, x_filename);
                status_code = 201;
            }
        }
    }

    pthread_mutex_unlock(&g_write);
    headers_["Content-Length"] = ftos(body_.length());

    if (!file_->getFilePath().empty())
        headers_["Location"] = file_->getFilePath();

    return status_code;
}

// int HttpResponse::PUT() {
//     pthread_mutex_lock(&g_write);

//     int status_code = 500;

//     if (!file_) {
//         std::cerr << "File not found" << std::endl;
//         pthread_mutex_unlock(&g_write);
//         return 500;
//     }

//     if (file_->exists()) {
//         // Update existing file
//         if (file_->updateFile(config_.getBody())) {
//             status_code = 200; // or 204 if no content in response is desired
//         } else {
//             std::cerr << "Failed to update file" << std::endl;
//             status_code = 500;
//         }
//     } else {
//         // Create new file
//         if (file_->createFile(config_.getBody())) {
//             status_code = 201;
//         } else {
//             std::cerr << "Failed to create file" << std::endl;
//             status_code = 500;
//         }
//     }

//     pthread_mutex_unlock(&g_write);
//     return status_code;
// }

// int HttpResponse::PUT() {
//     pthread_mutex_lock(&g_write);

//     int status_code = 500;

//     if (!file_) {
//         std::cerr << "File not found" << std::endl;
//         pthread_mutex_unlock(&g_write);
//         return 500;
//     }
    
//     if (file_->exists()) {
//         MimeTypes mimeTypes;
//         std::string contentType = config_.getHeader("content-type");
//         bool isMultipart = containsBoundary(contentType);
//         std::string boundary = isMultipart ? extractBoundary(contentType) : contentType;

//         if (isMultipart && !boundary.empty()) {
//             std::string filename = extractFilename(config_.getBody());
//             std::string fileContent = extractContent(config_.getBody(), boundary);

//             body_.clear();
//             body_.append(fileContent);
//             if (!filename.empty() && !fileContent.empty()) {
//                 if (file_->updateFile(body_)) {
//                     status_code = 200;
//                 } else {
//                     std::cerr << "Failed to update file" << std::endl;
//                     status_code = 500;
//                 }
//             } else {
//                 status_code = 400;
//             }
//         } else {
            
//             if (file_->createFile(body_)) {
//                 status_code = 201;
//             } else {
//                 std::cerr << "Failed to create file" << std::endl;
//                 status_code = 500;
//             }
//         }
//     } else {
//         std::cerr << "File does not exist" << std::endl;
//         status_code = 404;
//     }

//     pthread_mutex_unlock(&g_write);
//     return status_code;
// }

int HttpResponse::PUT() {
    pthread_mutex_lock(&g_write);

    int status_code = 500;

    if (!file_) {
        std::cerr << "File not found" << std::endl;
        pthread_mutex_unlock(&g_write);
        return 500;
    }

    MimeTypes mimeTypes;
    std::string contentType = config_.getHeader("content-type");
    bool isMultipart = containsBoundary(contentType);
    std::string boundary = isMultipart ? extractBoundary(contentType) : contentType;

    if (isMultipart && !boundary.empty()) {
        std::string filename = extractFilename(config_.getBody());
        std::string fileContent = extractContent(config_.getBody(), boundary);

        body_.clear();
        body_.append(fileContent);
        if (!filename.empty() && !fileContent.empty()) {
            if (file_->exists()) {
                if (file_->updateFile(body_)) {
                    status_code = 200;
                } else {
                    std::cerr << "Failed to update file" << std::endl;
                    status_code = 500;
                }
            } else {
                if (file_->createFile(body_)) {
                    status_code = 201;
                } else {
                    std::cerr << "Failed to create file" << std::endl;
                    status_code = 500;
                }
            }
        } else {
            status_code = 400;
        }
    } else {
        if (file_->exists()) {
            if (file_->updateFile(config_.getBody())) {
                status_code = 200;
            } else {
                std::cerr << "Failed to update file" << std::endl;
                status_code = 500;
            }
        } else {
            if (file_->createFile(config_.getBody())) {
                status_code = 201;
            } else {
                std::cerr << "Failed to create file" << std::endl;
                status_code = 500;
            }
        }
    }

    pthread_mutex_unlock(&g_write);
    return status_code;
}


int HttpResponse::DELETE()
{
    pthread_mutex_lock(&g_write);

    int status_code = 200;

    if (!file_)
    {
        std::cerr << "File not found" << std::endl;
        status_code = 500;
    }
    else
    {
        if (!file_->exists())
        {
            std::cerr << "File does not exist" << std::endl;
            status_code = 404;
        }
        else
        {
            if (file_->deleteFile())
            {
                headers_["Content-Length"] = "0";
                status_code = 200;
            }
            else
            {
                status_code = 500;
            }
        }
    }

    pthread_mutex_unlock(&g_write);
    std::string header = "<!DOCTYPE html>\n\
                 <html>\n\
                 <body>\n";

    std::string footer = "</body>\n\
                 </html>";
    body_.append(header);
    if (status_code == 200)
        body_.append("<h1>File deleted</h1>\n");
    else
        body_ = (status_code == 404)
                    ? body_.append("<h1>File not found</h1>\n")
                    : body_.append("<h1>Internal Server Error</h1>\n");
    body_.append(footer);
    headers_["Content-Type"] = "text/html";
    headers_["Content-Length"] = ftos(body_.length());

    return status_code;
}

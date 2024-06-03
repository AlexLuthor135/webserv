/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doduwole <doduwole@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 10:42:16 by doduwole          #+#    #+#             */
/*   Updated: 2024/06/01 14:06:16 by doduwole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST_PARSER_HPP
#define HTTP_REQUEST_PARSER_HPP

#include "./AllHeaders.hpp"

class HttpRequest {
private:
    std::string method_;
    std::string uri_;
    std::string scheme_;
    std::string authority_;
    std::string path_;
    std::string query_;
    std::string frag_;
    std::string target_;
    std::string protocol_;
    std::map<std::string, std::string> headers_;
    std::string body_;
    std::string req_buffer_;
    size_t length_;
    size_t chunk_size_;
    bool isChunked_;
    int body_offset_;
    struct timeval start_tv_;
    struct timeval last_tv_;

    enum Section {
        REQUEST_LINE,
        HEADERS,
        SPECIAL_HEADERS,
        BODY,
        CHUNK,
        COMPLETE,
        ERROR
    };
    enum ChunkStatus {
        CHUNK_BODY,
        CHUNK_SIZE
    };

    Section buffer_section_;
    ChunkStatus chunk_status_;

    int extractURIComponents();
    bool isValidScheme(const std::string& scheme);
    int isValidAuthority(const std::string& authority);
    bool isValidPath(const std::string& path);
    bool isValidQuery(const std::string& query);
    bool isValidFragment(const std::string& fragment);
    void print_uri_extracts();
    bool isValidIPv6(const std::string& ipv6);
    int isValidProtocol(const std::string& protocol);
    void parseContentLength();
    int parseMethod();
    int validateURI();
    int extractRequestLineData(std::string requestLine);
    std::string trimmer(const std::string& str);
    bool isValidHeaderChar(unsigned char c);
    bool isValidHeaderFormat(const std::string& header);
    int parseHeaders();
    int parseRequestLine();
    int parseBody();
    int parseChunkTrailer();
    int parseChunkedBody();
    int parseChunkSize(const std::string& hex);
    int checkSpecialHeaders();
    void extractPathQueryFragment(size_t pathStart);
    void parseSchemeAndAuthority(size_t schemeEnd, size_t& pathStart);
    std::string decodeURIComponent(const std::string& str);
    std::string uri_suffix_;


public:
    HttpRequest();
    ~HttpRequest();
    HttpRequest(const HttpRequest &rhs);
    HttpRequest &operator=(const HttpRequest &rhs);

    int parseRequest(std::string &buffer);

    std::string &getMethod();
    std::string &getURI();
    std::string &getPath();
    std::string &getQuery();
    std::string &getFragment();
    std::string &getProtocol();
    const std::string &getBody() const;
    std::string &getBody();
    std::string &getHeader(std::string key);
    std::map<std::string, std::string> getHeaders() const;
    std::string getTarget() const;
    size_t getContentLength();
    std::string getUriSuffix();
    void setUriSuffix(std::string& uri);

    void setTarget(std::string target);
    void printRequest(HttpRequest parser);
    int getStatus();
};

#endif

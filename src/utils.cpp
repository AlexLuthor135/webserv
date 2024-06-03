/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alappas <alappas@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 14:51:22 by alappas           #+#    #+#             */
/*   Updated: 2024/06/03 14:51:23 by alappas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/AllHeaders.hpp"

bool isMethodCharValid(char ch)
{
    return (ch == '!' || ch == '#' || ch == '$' || ch == '%' || ch == '&' || ch == '\'' ||
            ch == '*' || ch == '+' || ch == '-' || ch == '.' || ch == '^' || ch == '_' ||
            ch == '`' || ch == '|' || ch == '~' || std::isdigit(ch) || std::isalpha(ch));
}

void trimWordFromEnd(int &start, int &end, std::string line)
{
    (void)start;
    end = line.size() - 1;
    while (isspace(line[end]) || line[end] == '{')
        end--;
}

int checkCurly(std::string line)
{
    int i = 0;
    int openCurly = 0;
    int closedCurly = 0;

    while (line[i])
    {
        if (line[i] == '{')
            ++openCurly;
        else if (line[i] == '}')
            ++closedCurly;
        ++i;
    }
    return (openCurly == closedCurly) ? 0 : 1;
}

std::string cutTillSemicolon(std::string str)
{
    size_t i = str.find_first_of(';');
    if (i != std::string::npos)
        str = str.substr(0, i);
    return str;
}

void ft_errors(std::string arg, int i)
{
    if (i == 1)
        std::cerr << "Usage: " << arg << " <config_file>" << std::endl;
    if (i == 2)
        std::cerr << "Error opening file: " << arg << std::endl;
    if (i == 3)
        std::cerr << "Quotes are not closed." << std::endl;
    exit(1);
}

std::vector<std::string> split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);

    while (std::getline(tokenStream, token, delimiter))
        tokens.push_back(token);

    return tokens;
}

bool isAlpha(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool isDigit(char c)
{
    return (c >= '0' && c <= '9');
}

bool isAlphaNum(char c)
{
    return isAlpha(c) || isDigit(c);
}

bool isUnreserved(char c)
{
    return isAlphaNum(c) || c == '-' || c == '.' || c == '_' || c == '~';
}

bool isSubDelim(char c)
{
    return c == '!' || c == '$' || c == '&' || c == '\'' || c == '(' || c == ')' || c == '*' || c == '+' || c == ',' || c == ';' || c == '=';
}

bool isHexDigit(char c)
{
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

std::string trim(const std::string &str)
{
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");

    return (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

unsigned int hexToDecimal(const std::string &hex)
{
    std::stringstream ss;
    ss << std::hex << hex;
    unsigned int decimal;
    if (!(ss >> decimal))
        throw std::invalid_argument("Invalid hex value");

    return decimal;
}

std::map<char, int> initializeB64Index()
{
    std::map<char, int> index;
    const char B64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    for (int i = 0; i < 64; ++i)
        index[B64chars[i]] = i;

    return index;
}

const std::map<char, int> B64index = initializeB64Index();

const std::string b64decode(const void *data, const size_t &len)
{
    if (len == 0)
        return "";

    const unsigned char *p = static_cast<const unsigned char *>(data);
    size_t j = 0,
           pad1 = len % 4 || p[len - 1] == '=',
           pad2 = pad1 && (len % 4 > 2 || p[len - 2] != '=');
    const size_t last = (len - pad1) / 4 << 2;
    std::string result((last / 4 * 3) + pad1 + pad2, '\0');
    unsigned char *str = reinterpret_cast<unsigned char *>(&result[0]);

    for (size_t i = 0; i < last; i += 4)
    {
        int n = B64index.at(p[i]) << 18 | B64index.at(p[i + 1]) << 12 | B64index.at(p[i + 2]) << 6 | B64index.at(p[i + 3]);
        str[j++] = n >> 16;
        str[j++] = (n >> 8) & 0xFF;
        str[j++] = n & 0xFF;
    }
    if (pad1)
    {
        int n = B64index.at(p[last]) << 18 | B64index.at(p[last + 1]) << 12;
        str[j++] = n >> 16;
        if (pad2)
        {
            n |= B64index.at(p[last + 2]) << 6;
            str[j++] = (n >> 8) & 0xFF;
        }
    }
    return result;
}

std::string b64decode(const std::string &str64)
{
    return b64decode(str64.c_str(), str64.size());
}

std::string ftos(size_t num)
{
    std::ostringstream oss;
    oss << num;
    return oss.str();
}

std::string removeDupSlashes(std::string str)
{
    if (str.empty())
        return str;

    std::string result;
    result.reserve(str.length());

    bool prevWasSlash = false;
    for (size_t i = 0; i < str.length(); ++i)
    {
        if (str[i] == '/')
        {
            if (!prevWasSlash)
                result.push_back(str[i]);

            prevWasSlash = true;
        }
        else
        {
            result.push_back(str[i]);
            prevWasSlash = false;
        }
    }

    if (!result.empty() && result[result.length() - 1] == '/')
        result.erase(result.length() - 1);

    return result;
}

std::string formatHttpDate(time_t timeValue)
{
    char buf[32];
    struct tm *timeinfo = gmtime(&timeValue);

    timeinfo->tm_hour += 4;
    mktime(timeinfo);

    strftime(buf, sizeof(buf), "%a, %d %b %Y %T GMT", timeinfo);
    return std::string(buf);
}

std::string get_http_date()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    time_t currentTime = tv.tv_sec;
    return formatHttpDate(currentTime);
}

std::string extractFilename(const std::string& body) {
    size_t filenamePos = body.find("filename=\"");
    if (filenamePos != std::string::npos) {
        filenamePos += 10;

        size_t closingQuotePos = body.find("\"", filenamePos);
        if (closingQuotePos != std::string::npos) {
            return body.substr(filenamePos, closingQuotePos - filenamePos);
        }
    }
    return "";
}

std::string extractBoundary(const std::string& contentType) {
    size_t boundaryPos = contentType.find("boundary=");
    return (boundaryPos == std::string::npos) ? "" : contentType.substr(boundaryPos + 9);
}

bool containsBoundary(const std::string& input) {
    return input.find("boundary") != std::string::npos;
}

std::string extractContent(const std::string& body, const std::string& boundary) {
    std::string boundaryLine = "--" + boundary;
    std::string endBoundaryLine = boundaryLine + "--";

    size_t boundaryStart = body.find(boundaryLine);
    if (boundaryStart == std::string::npos)
        return "";

    boundaryStart += boundaryLine.length() + 2;

    size_t headerEnd = body.find("\r\n\r\n", boundaryStart);
    if (headerEnd == std::string::npos)
        return "";

    headerEnd += 4;

    size_t contentEnd = body.find(boundaryLine, headerEnd);
    if (contentEnd == std::string::npos)
        contentEnd = body.find(endBoundaryLine, headerEnd);

    if (contentEnd == std::string::npos)
        return "";

    contentEnd -= 2;

    return body.substr(headerEnd, contentEnd - headerEnd);
}

template <typename T>
void printVec(const std::vector<T> &value, const std::string &callingFunction)
{
    typename std::vector<T>::const_iterator it;
    std::cout << "Printing vector from " << callingFunction << std::endl;

    for (it = value.begin(); it != value.end(); ++it)
        std::cout << "Value: " << *it << std::endl;

    std::cout << std::endl;
}
template void printVec<std::string>(const std::vector<std::string> &value, const std::string &callingFunction);
template void printVec<int>(const std::vector<int> &value, const std::string &callingFunction);

template <typename KeyType, typename ValueType>
void printMap(const std::map<KeyType, ValueType> &m)
{
    typename std::map<KeyType, ValueType>::const_iterator it;
    for (it = m.begin(); it != m.end(); ++it)
        std::cout << it->first << ": " << it->second << std::endl;
    std::cout << std::endl;
}
template void printMap(const std::map<std::string, std::string> &m);
template void printMap(const std::map<int, std::string> &m);
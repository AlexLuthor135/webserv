/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseNegotiation.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alappas <alappas@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 14:50:52 by alappas           #+#    #+#             */
/*   Updated: 2024/06/03 14:50:54 by alappas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/HttpResponse.hpp"

int ftstoi(const std::string &str, std::size_t *pos = NULL, int base = 10) {
    static const std::string baseChars = "0123456789abcdef";

    if (str.empty())
        throw std::invalid_argument("null or empty string argument");

    bool negate = (str[0] != '-');
    std::size_t i = (str[0] == '+' || str[0] == '-');

    int result = 0;
    for (; i < str.length(); ++i) {
        char c = str[i];
        if (baseChars.find(c) == std::string::npos)
            throw std::invalid_argument("invalid input string");

        int digit = baseChars.find(c);
        if (result > (std::numeric_limits<int>::max() - digit) / base)
            throw std::overflow_error("overflow");
        
        result = result * base + digit;
    }

    if (negate)
        result = -result;

    if (pos != NULL)
        *pos = i;

    return result;
}

bool HttpResponse::localization(std::vector<std::string> &matches) {
    std::string all = config_.getHeader("Accept-Language");
    int maxQ = 0;
    std::vector<std::string> selectMatches;
    std::string defaultLanguage = "en";

    headers_["Content-Language"] = defaultLanguage;

    while (!all.empty()) {
        std::size_t pos = all.find_first_of(" ,;\0");
        std::string str = all.substr(0, pos);
        all.erase(0, pos);

        int q = 10;
        if (all.find(".") != std::string::npos) {
            std::size_t qPos;
            q = ftstoi(all.substr(all.find_first_of(".") + 1, 1).c_str(), &qPos);
            all.erase(0, qPos);
        }

        std::vector<std::string> newMatches;
        if (str.find("*") == std::string::npos) {
            for (std::vector<std::string>::iterator it = matches.begin(); it != matches.end(); ++it) {
                if (it->find("." + str) != std::string::npos)
                    newMatches.push_back(*it);
            }
        } else {
            newMatches = matches;
        }

        if (!newMatches.empty() && q > maxQ) {
            selectMatches = newMatches;
            if (str[0] != '*')
                headers_["Content-Language"] = str;
            maxQ = q;
        }

        if (all.find(",") == std::string::npos) {
            if (!selectMatches.empty()) {
                matches = selectMatches;
                return true;
            }
            return false;
        }

        all.erase(0, all.find_first_of("abcdefghijklmnoprstuvwxyz*"));
    }

    return false;
}

CharsetAndQ HttpResponse::extractCharsetAndQ(const std::string &charsetAndQ)
{
  CharsetAndQ entry;
  entry.qValue = 1.0;

  size_t qPos = charsetAndQ.find(";q=");
  if (qPos != std::string::npos)
  {
    entry.charset = charsetAndQ.substr(0, qPos);
    std::string qStr = charsetAndQ.substr(qPos + 3);
    try
    {
      entry.qValue = atof(qStr.c_str());
    }
    catch (...)
    {
      entry.qValue = 0.0;
    }
  }
  else
  {
    entry.charset = charsetAndQ;
  }

  return entry;
}

std::string HttpResponse::findBestCharset(const std::vector<CharsetAndQ> &charsetAndQValues, const std::vector<std::string> &matches)
{
  std::string bestCharset;
  double bestQValue = 0.0;

  for (size_t i = 0; i < charsetAndQValues.size(); ++i)
  {
    CharsetAndQ entry = charsetAndQValues[i];
    if (entry.charset == "*")
    {
      bestCharset = matches.front();
      bestQValue = entry.qValue;
      break;
    }
    else
    {
      for (size_t j = 0; j < matches.size(); ++j)
      {
        if (matches[j].find("." + entry.charset) != std::string::npos && entry.qValue > bestQValue)
        {
          bestCharset = matches[j];
          bestQValue = entry.qValue;
          break;
        }
      }
    }
  }

  return bestCharset;
}

std::string HttpResponse::handleDefaultCharset(const std::string &bestCharset)
{
  return (bestCharset.empty()) ? "utf-8" : bestCharset;
}


std::string HttpResponse::accept_charset(std::vector<std::string> &matches)
{
  std::string all = config_.getHeader("Accept-Charset");
  std::vector<CharsetAndQ> charsetAndQValues;

  size_t pos = 0;
  while (pos < all.length())
  {
    size_t nextComma = all.find(',', pos);
    size_t end = (nextComma == std::string::npos) ? all.length() : nextComma;
    std::string charsetAndQ = all.substr(pos, end - pos);

    CharsetAndQ entry = extractCharsetAndQ(charsetAndQ);
    charsetAndQValues.push_back(entry);

    if (nextComma == std::string::npos)
      break;
    else
      pos = nextComma + 1;
  }

  std::string bestCharset = findBestCharset(charsetAndQValues, matches);

  return handleDefaultCharset(bestCharset);
}



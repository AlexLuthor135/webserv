/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestConfig.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doduwole <doduwole@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 10:43:16 by doduwole          #+#    #+#             */
/*   Updated: 2024/06/01 14:07:08 by doduwole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTCONFIG_HPP
#define REQUESTCONFIG_HPP

#include "./AllHeaders.hpp"
#include "./DB.hpp"
#include "./Listen.hpp"

class HttpRequest;
class Client;
struct Listen;
struct DB;

enum LocationModifier
{
  NONE,
  EXACT,
  CASE_SENSITIVE,
  CASE_INSENSITIVE,
  LONGEST,
};

typedef std::map<std::string, std::string> MapStr;
typedef std::vector<std::string> VecStr;
typedef std::map<std::string, VecStr> KeyValues;
typedef std::pair<MapStr, VecStr> KeyMapValue;
typedef std::map<int, std::vector<KeyMapValue> > GroupedDBMap;

class RequestConfig
{
public:
  RequestConfig(HttpRequest &request, Listen &host_port, DB &db, Client &client);
  ~RequestConfig();
  RequestConfig(const RequestConfig &rhs);
  RequestConfig(const RequestConfig &rhs, HttpRequest &request, Client &client);
  RequestConfig &operator=(const RequestConfig &rhs);


  const VecStr &filterDataByDirectives(const std::vector<KeyMapValue> &values, std::string directive, std::string location);
  const VecStr &cascadeFilter(std::string directive, std::string location);
  const VecStr &checkRootDB(std::string directive);
  LocationModifier setModifier(const std::string &modifiers);
  bool isMethodAccepted(std::string &method);
  void redirectLocation(std::string target);
  std::string locationExtractor(const std::string &locationStr);
  std::string findLongestMatch();
  std::string findLongestMatch(std::string target);

  void setUp(size_t targetServerIdx);
  void setTarget(const std::string &target);
  void setRoot(const VecStr root);
  void setUri(const std::string uri);
  void setClientMaxBodySize(const VecStr size);
  void setAutoIndex(const VecStr autoindex);
  void setAutoIndex(bool autoindex);
  void setIndexes(const VecStr &indexes);
  void setErrorPages(const VecStr &errors);
  void setMethods(const VecStr &methods);
  void setAuth(const VecStr &auths);
  void setUpload(const VecStr &upload);
  void setCgi(const VecStr &cgi);
  void setLocationsMap(const std::vector<KeyMapValue> &values);
  void setRedirectMap(const VecStr &redirects);
  void setRedirCode(int code);

  std::string getUriSuffix();

  int &getRedirCode();
  std::string &getTarget();
  std::string &getRequestTarget();
  std::string &getQuery();
  std::string &getFragment();
  std::string &getHost();
  uint32_t &getPort();
  Client &getClient();
  std::string &getRoot();
  std::string &getUri();
  size_t &getClientMaxBodySize();
  bool getAutoIndex();
  VecStr &getIndexes();
  std::map<int, std::string> &getErrorPages();
  std::map<int, std::string> &getRedirectionMap();
  std::vector<std::string> &getMethods();
  std::string &getMethod();
  const std::string &getBody() const;
  std::map<std::string, std::string> getHeaders();
  std::string &getHeader(std::string key);
  std::string &getProtocol();
  size_t getContentLength();
  std::string &getAuth();
  std::string &getUpload();
  std::vector<std::string> &getCgi();
  std::map<std::string, int> &getLocationsMap();
  RequestConfig *getRequestLocation(std::string request_target);
  bool directiveExists(std::string directive, std::string location);
  void returnRedirection();
  void setBestMatch(std::string &newTarget);
  void setLociMatched(int val);
  int getLociMatched();
  void setTargetSensitivity();
  bool isCgi(std::string path);
  void setCgi(bool& val);
  bool get_Cgi();
  std::string &getBody();

  void setMap(const VecStr &vec, std::map<int, std::string> &resultMap, std::string &codes);
  void assignCodes(const std::string &codes, const std::string &page, std::map<int, std::string> &resultMap);

  void printConfigSetUp();
  void setClient(Client &client);
  std::string location_cache_;

private:
  HttpRequest &request_;
  std::vector<KeyMapValue> targetServer_;
  Client &client_;
  Listen host_port_;
  DB db_;
  LocationModifier modifierType_;
  std::string target_;
  std::string root_;
  std::string uri_;
  size_t client_max_body_size_;
  bool autoindex_;
  std::vector<std::string> indexes_;
  std::map<int, std::string> error_codes_;
  std::map<int, std::string> redirectMap_;
  std::vector<std::string> allowed_methods_;
  size_t serverId;
  std::string auth_;
  std::string upload_;
  std::vector<std::string> cgi_;
  std::map<std::string, int> locationsMap_;
  int isLociMatched_;
  std::string uri_suffix_;
  int redir_code_;
};

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Servers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doduwole <doduwole@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 10:43:36 by doduwole          #+#    #+#             */
/*   Updated: 2024/06/01 10:43:38 by doduwole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERS_HPP
# define SERVERS_HPP
#include "AllHeaders.hpp"

class ConfigDB;
class InputArgs;
struct DB;
struct Listen;
class HttpRequest;
class CgiClient;

class Servers {
	private:
		int _epoll_fds;
		std::vector<int> _server_fds;
		std::map<int, std::vector<std::string> > _domain_to_server;
		std::map<int, std::string> _ip_to_server;
		std::map<std::string, std::vector<std::string> > _keyValues;
		std::map<int, std::vector<std::string> > server_index;
		std::map<int, int> server_fd_to_index;
		std::map<int, int> client_to_server;
		int	_client_amount;
		std::map<int, HttpRequest> _client_data;
		std::map<int, CgiClient*> _cgi_clients;
		std::map<int, int> _cgi_clients_childfd;
		std::map<int, time_t> _client_time;

		int stop_fd;
			
	public:
		
		Servers(ConfigDB &configDB);
		~Servers();
		Servers(const Servers &rhs);
		Servers &operator=(const Servers &rhs);

		ConfigDB &configDB_;

		int		checkSocket(std::string port);
		int		checkSocketPort(std::string port);
		int		createSocket();
		int		bindSocket(std::string port);
		int		listenSocket();
		int		combineFds(int socket_fd);
		void	createEpoll();
		void	createServers();
		void	assignDomain(std::string port, int server_fd);
		void	assignLocalDomain(int server_fd);
		void	initEvents();
		void	deleteClient(int client_fd);
		std::vector<std::string> getPorts();
		std::map<std::string, std::vector<std::string> > getKeyValue() const;
		bool getRequest(int client_fd, std::string &request);
		int setNonBlocking(int fd);
		void printData();
		void removeFromEpoll(int fd);

		void handleIncomingConnection(int server_fd);
		void handleIncomingData(int client_fd);
		void printServerAddress(int server_fd);
		size_t handleResponse(int reqStatus, int server_fd, int new_socket, HttpRequest &parser);
		void checkClientTimeout();
		void setTimeout(int client_fd);
		int handleIncomingCgi(int client_fd);
};

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doduwole <doduwole@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 10:42:53 by doduwole          #+#    #+#             */
/*   Updated: 2024/06/01 10:42:56 by doduwole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTEN_HPP
# define LISTEN_HPP

#include <string>

struct Listen
{
  std::string ip_;
  uint32_t port_;

  Listen() : ip_("127.0.0.1"), port_(80){};
  Listen(std::string ip, uint32_t port) : ip_(ip), port_(port){};
};

#endif
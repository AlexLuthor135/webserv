/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alappas <alappas@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 20:56:41 by alappas           #+#    #+#             */
/*   Updated: 2024/06/04 20:56:43 by alappas          ###   ########.fr       */
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
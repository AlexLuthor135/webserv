/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DB.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doduwole <doduwole@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 10:41:58 by doduwole          #+#    #+#             */
/*   Updated: 2024/06/01 10:42:01 by doduwole         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DB_HPP
#define DB_HPP

#include <map>
#include <vector>
#include <string>

typedef std::map<std::string, std::string> MapStr;
typedef std::vector<std::string> VecStr;
typedef std::map<std::string, VecStr> KeyValues;
typedef std::pair<MapStr, VecStr> KeyMapValue;
typedef std::map<int, std::vector<KeyMapValue> > GroupedDBMap;

struct DB
{
  const GroupedDBMap serversDB;
  const GroupedDBMap rootDB;
};

#endif
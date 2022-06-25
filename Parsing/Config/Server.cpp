/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mokhames <mokhames@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/18 14:33:30 by mokhames          #+#    #+#             */
/*   Updated: 2022/06/25 00:23:31 by mokhames         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Location.hpp"

  // ! Constuctors and destructor
 Server::Server() : serverline(), serverName(), _listen(), _locations(), root(), _allowed_methods(), _index(), redirect_uri(), errorPages(), cgiPath(), _autoindex() {
     setClientMaxBodySize(10000000000);
 }
 Server::~Server() {}
 Server::Server(Server const& c)
 {
     *this = c;
 }
 Server &Server::operator=(Server const& c)
 {
    //std::cout << "Server::operator=(Server)" << std::endl;
    //std::cout << c.getIp() << std::endl;
    serverline.assign(c.serverline.begin(), c.serverline.end());
    serverName.assign(c.serverName.begin(), c.serverName.end());
    _listen = c._listen;
    _locations.assign(c._locations.begin(), c._locations.end());
    root = c.root;
    _allowed_methods = c._allowed_methods;
    _index = c._index;
    redirect_uri = c.redirect_uri;
    errorPages = c.errorPages;
    cgiPath = c.cgiPath;
    uploadPath = c.uploadPath;
    _clinet_max_body_size = c._clinet_max_body_size;
    _autoindex = c._autoindex;
   // std::cout << _listen.getIp() << std::endl;
    return *this;

 }

  // ! /////////////////////// Getters & Setters //////////////////////
 void Server::setServerline(std::vector<std::string> c)
 {
     serverline.assign(c.begin(), c.end());
 }
 // ! /////////////////////// Parse Lines //////////////////////

void Server::parseLines()
{
    std::vector<std::string>::iterator it = serverline.begin();
    
    it +=2;
    for (;it != serverline.end();it++)
    {
    //   std::cout << *it << std::endl;
        if ((*it)[0] == '#')
            continue;
        if ((*it).find("server_name ") != std::string::npos)
            fetch_server_name(*it);
        else if ((*it).find("listen ") != std::string::npos)
            fetch_host(*it);
        else if ((*it).find("root") != std::string::npos)
            fetch_root(*it);
        else if ((*it).find("allow_methods ") != std::string::npos)
             fetch_allowed_methods(*it);
        else if ((*it).find("client_body_buffer_size ") != std::string::npos)
            fetch_cbbs(*it);
        else if ((*it).find("redirect ") != std::string::npos)
            fetch_redirect(*it);
        else if ((*it).find("cgi_path ") != std::string::npos)
            fetch_cgi(*it);
        else if ((*it).find("autoindex ") != std::string::npos)
            fetch_autoindex(*it);
        else if ((*it).find("upload ") != std::string::npos)
            fetch_upload(*it);
        else if ((*it).find("index ") != std::string::npos)
            fetch_index(*it);
        else if ((*it).find("error_page ") != std::string::npos)
            fetchErrorPage(*it);
        else if ((*it).find("location ") != std::string::npos)
            it += fetch_location(it);
        else if ((*it).find("}") != std::string::npos)
                continue;
        else
            throw NotacceptableError();
    }
}

 void Server::fetch_server_name(std::string& c)
 {
     int i = spaceCount(c, 12);
    std::string tmp = c.substr(i, c.size() - i);
    size_t pos = 0;
    while ((pos = tmp.find(" ")) != std::string::npos || (pos = tmp.find("\t")) != std::string::npos)
    {
        serverName.push_back(tmp.substr(0,pos));
        tmp.erase(0, pos + 1);
    }
    serverName.push_back(tmp.substr(0,pos));
    tmp.erase(0, pos + 1);
 }

 void Server::fetch_host(std::string& c)
 {
     int i = spaceCount(c, 7);
    std::string tmp = c.substr(i, c.size() - i);
    size_t pos = 0;
     while ((pos = tmp.find(":")) != std::string::npos)
    {
        setIp(tmp.substr(0,pos));
        tmp.erase(0, pos + 1);
    }
    i = 0;
    std::istringstream(tmp.substr(0,pos)) >> i;
    sscanf(tmp.substr(0,pos).c_str(), "%d", &i);
    setPort(i);
    tmp.erase(0, pos + 1);
}

 void Server::fetch_redirect(std::string& c)
 {
     int i = spaceCount(c, 9);
    std::string tmp = c.substr(i, c.size() - i);
    i = 0;
    for (;tmp[i] == ' '; i++);
    tmp = tmp.substr(i, tmp.size() - i);
    std::vector<std::string> a;
     size_t pos = 0;
    while ((pos = tmp.find(" ")) != std::string::npos || (pos = tmp.find("\t")) != std::string::npos)
    {
        a.push_back(tmp.substr(0, pos));
        tmp.erase(0, pos + 1);
    }
    a.push_back(tmp.substr(0, pos));
    tmp.erase(0, pos + 1);
    if (a.size() != 2)
        throw NotacceptableError();
    if (a[0].size() != 3)
        throw NotacceptableError();
    int q = 0;
    std::istringstream(a[0]) >> q;
    sscanf(a[0].c_str(), "%d", &q);
    redirect_uri.insert(std::make_pair(q, a[1]));

 }
void Server::fetch_upload(std::string& c)
 {
     //std::cout << "root insde fetch " <<  c << std::endl;
    int i = spaceCount(c, 7);
    this->setUploadPath(c.substr(i, c.size() - i));
    if (!parsePath(this->uploadPath))
        std::cout << "fuck u" << std::endl;
 }

 void Server::fetch_cgi(std::string& c)
 {
     //std::cout << "root insde fetch " <<  c << std::endl;
     int i = spaceCount(c, 9);
    this->setCgiPath(c.substr(i, c.size() - i));

 }

 void Server::fetch_root(std::string& c)
 {
     //std::cout << "root insde fetch " <<  c << std::endl;
     int i = spaceCount(c, 5);
    this->setRoot(c.substr(i, c.size() - i));

 }
//  void Server::fetch_index(std::string& c)
//  {
//     setIndex(c.substr(6, c.size() - 6));
//  }
int Server::fetch_location(std::vector<std::string>::iterator it)
{
    int i = 1;
    Location tmp(*this);
    size_t pos = 0;
    // _locations.resize(_locations.size() + 1);
    // i = spaceCount(*it, 9);
    std::string a = (*it).substr(9, it->size() - 9);
    pos = a.find(" ");
    tmp.setLocation_match(a.substr(0, pos));
    it++;
      while (*it != "}")
    {
        if ((*it)[0] == '#')
        {
            i++;
            it++;
            continue;
        }
    
        if ((*it).find("root") != std::string::npos)
            tmp.fetch_root(*it);
        else if ((*it).find("allow_methods ") != std::string::npos)
            tmp.fetch_allowed_methods(*it);
        else if ((*it).find("client_body_buffer_size ") != std::string::npos)
            tmp.fetch_cbbs(*it);
        else if ((*it).find("autoindex ") != std::string::npos)
            tmp.fetch_autoindex(*it);
        else if ((*it).find("index ") != std::string::npos)
            tmp.fetch_index(*it);
        else if ((*it).find("error_page ") != std::string::npos)
            tmp.fetchErrorPage(*it);
        else if ((*it).find("redirect ") != std::string::npos)
            tmp.fetch_redirect(*it);
        else if ((*it).find("cgi_path ") != std::string::npos)
            tmp.fetch_cgi(*it);
        else if ((*it).find("upload ") != std::string::npos)
            tmp.fetch_upload(*it);
        else
             throw NotacceptableError();
        i++;
        it++;
    }
    _locations.push_back(tmp);
    return i;
}

void Server::fetch_index(std::string& c)
 {
     int i = spaceCount(c, 6);
    std::string tmp = c.substr(i, c.size() - i);
    size_t pos = 0;
    while ((pos = tmp.find(" ")) != std::string::npos || (pos = tmp.find("\t")) != std::string::npos)
    {
        std::string str = tmp.substr(0,pos);
        _index.push_back(str);
        tmp.erase(0, pos + 1);
    }
    _index.push_back(tmp.substr(0,pos));
    tmp.erase(0, pos + 1);
 }
 void Server::fetch_allowed_methods(std::string& c)
 {
     int i = spaceCount(c, 14);
    std::string tmp = c.substr(i, c.size() - i);
    size_t pos = 0;
    int flag = 0;
    while ((pos = tmp.find(" ")) != std::string::npos || (pos = tmp.find("\t")) != std::string::npos)
    {
        std::string str = tmp.substr(0,pos);
        if (str != "GET" && str != "POST" && str != "DELETE")
            throw WrongMethod();
        _allowed_methods.push_back(str);
        flag = 1;
        tmp.erase(0, pos + 1);
    }
    if (flag == 0)
    {
           if (tmp.substr(0,pos) != "GET" && tmp.substr(0,pos) != "POST" && tmp.substr(0,pos) != "DELETE")
            throw WrongMethod();
    }
    _allowed_methods.push_back(tmp.substr(0,pos));
    tmp.erase(0, pos + 1);
 }
 void Server::fetch_cbbs(std::string& c)
 {
     int e = spaceCount(c, 24);
    std::string tmp = c.substr(e, c.size() - e);
    unsigned long i = 0;
    std::istringstream(tmp) >> i;
    sscanf(tmp.c_str(), "%ld", &i);
    setClientMaxBodySize(i);
 }

 void Server::fetch_autoindex(std::string& c)
 {
     int i = spaceCount(c, 10);
    std::string tmp = c.substr(i, c.size() - i);
    if (tmp == "on")
        setAutoIndex(true);
    else
        setAutoIndex(false);
}

void Server::fetchErrorPage(std::string& c)
{
    int i = spaceCount(c, 12);
    std::string tmp = c.substr(i, c.size() - i);
    i = 0;
    for (;tmp[i] == ' '; i++);
    tmp = tmp.substr(i, tmp.size() - i);
    std::vector<std::string> a;
     size_t pos = 0;
    while ((pos = tmp.find(" ")) != std::string::npos || (pos = tmp.find("\t")) != std::string::npos)
    {
        a.push_back(tmp.substr(0, pos));
        tmp.erase(0, pos + 1);
    }
    a.push_back(tmp.substr(0, pos));
    tmp.erase(0, pos + 1);
    if (a.size() != 2)
        throw NotacceptableError();
    if (a[0].size() != 3)
        throw NotacceptableError();
    int q = 0;
    std::istringstream(a[0]) >> q;
    sscanf(a[0].c_str(), "%d", &q);
    errorPages.insert(std::make_pair(q, a[1]));
    // std::cout << q << std::endl;
}
int Server::spaceCount(std::string &c, int a) const {

    int i = a;
    for (; c[i] == ' '; i++);
    return i;
}
// ! ///////////////////////////// Error pages ////////////////////

// void Server::checkStatusCode(std::string& c)
// {
    
// }

    int Server::parsePath(std::string& c)
    {
        std::stack<std::string> a;
        std::string tmp = c;
        std::string sub;
        size_t pos = 0;
        while ((pos = tmp.find("/")) != std::string::npos)
        {
             sub = tmp.substr(0, pos);
             if (sub == ".." && !a.empty())
                 a.pop();
             else if (sub == ".");
             else
                 a.push(tmp.substr(0, pos));
             if (a.empty())
                 return 0;
             tmp.erase(0, pos + 1);
        }
        return 1;
    }

  // ! ///////////////////////////// checkers  /////////////////////
    bool Server::checkServerName(std::string& c)
    {
        for (size_t i = 0; i < serverName.size(); i++)
        {
            if (serverName[i] == c)
                return true;
        }
        return false;
    }


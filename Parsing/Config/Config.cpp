#include "Config.hpp"
#include "Server.hpp"
#include "Location.hpp"

// ! //////////////////// Constuctors and destructor /////////////////
<<<<<<< HEAD
Config::Config(std::string file) : file_name(file) , full_file(), lines(), servers()
{

=======

Config::Config(std::string file) : file_name(file) , full_file(), lines(), servers()
{
>>>>>>> akhalidy
}

Config::~Config(){}

Config::Config(Config const& c) 
{
    *this = c;
}

Config &Config::operator=(Config const& c) 
{
    servers = c.servers;
<<<<<<< HEAD
=======
    lines = c.lines;
    _binders = c._binders;
>>>>>>> akhalidy
    return *this;
}

// ! /////////////////////// Getters & Setters //////////////////////
std::vector<Server> Config::getServers() const
{
    return servers;
}

std::string Config::getFullFile() const
{
    return full_file;
}

std::vector<std::string> Config::getLines() const
{
    return lines;
}
std::string Config::getLine(std::string &) const
{
    return "sever_name";
}

Server Config::getServer(int port, std::string &c) const
{
    std::vector<Server> a;
    int save = 0;
    int j = 0;
    for (int i = 0; i < servers.size(); i++)
    {
        if (servers[i].getPort() == port)
            a.push_back(servers[i]);
    }
<<<<<<< HEAD
=======
    // std::cout << "Server " << std::endl;
>>>>>>> akhalidy
    if (!c.size() && a.size())
        return a[0];
    else if (!c.size())
            throw NoServerFoundException();
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i].checkServerName(c))
        {
            save = i;
            j++;
        }

    }
    if (j > 1)
        throw (MultpiteServerName());
<<<<<<< HEAD
    
    return servers[save];
}

=======
    return servers[save];
}
 std::map<int, std::string> Config::getBinders() const
 {
        return _binders;
 }
>>>>>>> akhalidy
void Config::setfile(std::string& file)
{
    full_file = file;
}
// ! /////////////////////// file_Readers //////////////////////


void Config::main_read()
{
    read_file();
    split_line();
    split_servers();
    main_error_check();
    parse_server();
    parse_bind_map();
}

void Config::read_file()
{
    std::fstream a(file_name);

    if (!a.is_open())
    {
        std::cout << "Error : " << file_name;
        throw fileErrorException();
    }
    std::string output;
    while (std::getline(a, output, '\t'))
        full_file += output;
}

void    Config::split_line()
{
    std::string line;
    std::string full_file1(full_file);
    size_t pos = 0;
    while ((pos = full_file1.find("\n")) != std::string::npos)
    {
        line = full_file1.substr(0, pos);
        if (line[0] != '\0')
        lines.push_back(line);
        full_file1.erase(0, pos + 1);
    }
    lines.push_back(line);
    full_file1.erase(0, pos + 1);
}

void Config::split_servers()
{
    int open = 1;
    Server  s1;
    std::vector<std::string>::iterator it = lines.begin();
    std::vector<std::string>::iterator save = it;
    while (it != lines.end())
    {
        if ((*it).find("server ") != std::string::npos && open == 1)
            open = 0;
        else if ((*it).find("server ") != std::string::npos && open == 0)
        {
            std::vector<std::string> a (save, it);
            s1.setServerline(a);
            servers.push_back(s1);
            save = it;
        }
        it++;
    }
    std::vector<std::string> a (save, it);
    s1.setServerline(a);
    servers.push_back(s1);
}

void Config::parse_server()
{
<<<<<<< HEAD
    //std::vector<std::string> a;
    std::vector<Location> a;
   // int a;

    for (int i = 0; i < servers.size(); i++)
        servers[i].parseLines();
    std::string s = "youpiq";
    Server sr = getServer(8000, s);
    std::cout << sr.getIndex() << std::endl;
    // std::map<int, std::string> c = servers[0].getErrorPages();

    // for (std::map<int, std::string>::iterator it= c.begin(); it != c.end(); it++)
    //     std::cout << it->first << " = " << it->second << std::endl;
     //for (int i = 0; i < servers.size(); i++)
     //{
    //    // ? TO print server Names;
    // //    a =  servers[i].getServerName();
    // //    std::cout << "Server " << i << std::endl;
    // //    for (int j = 0; j < a.size(); j++)
    // //         std::cout << a[j] << std::endl;

    //    // ? to print ip and port
    //    std::cout << servers[i].getIp() << "--------------------------------" << servers[i].getPort() << std::endl;
    //     // ? to print root
    //     std::cout << servers[i].getRoot() << std::endl;
    //     // ? to print index
    //     std::cout << servers[i].getIndex() << std::endl;
    //    a = servers[0].getLocation();
        //std::cout << a[0].getRoot() << std::endl;
        //  for (int i = 0; i < a.size(); i++)
            //  std::cout << "location  " << a[i].getLocation_match() << " = " << a[i].getRoot() << std::endl;
    // }
=======
    for (int i = 0; i < servers.size(); i++)
        servers[i].parseLines();
>>>>>>> akhalidy
}

void Config::parse_bind_map()
{
    for (int i = 0; i < servers.size(); i++)
<<<<<<< HEAD
        _binders.insert(std::pair<int, const char *>(servers[i].getPort(),servers[i].getIp().c_str()));
=======
        _binders.insert(std::pair<int, std::string>(servers[i].getPort(),servers[i].getIp()));
	// std::map<int, const char*>::iterator it = _binders.begin();
	// std::map<int, const char*>::iterator end = _binders.end();
	//std::cout << " hello " << _binders[8001] << std::endl;
	// for(; it != end; ++it)
	// 	std::cout << "Binding to " << it->second << " on port " << it->first << std::endl;
>>>>>>> akhalidy
}

// ! /////////////////////// Errors Managers //////////////////////

void Config::main_error_check()
{
    check_brekets();
}

void Config::check_brekets()
{
    int open;
    open = 0;
    for (int i = 0; full_file[i]; i++)
    {
        if (full_file[i] == '{')
            open++;
        if (full_file[i] == '}')
            open--;
    }
    if (open != 0)
        throw NotclosedBrackets();
}

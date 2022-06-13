/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trevor <trevor@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/11 12:08:56 by laafilal          #+#    #+#             */
/*   Updated: 2022/06/13 10:20:02 by trevor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__
# include "../FileHandler/fileHandler.hpp"
# include "../Parsing/Request/Request.hpp"
# include "../Parsing/Config/Config.hpp"
# include "../Parsing/Config/Server.hpp"
# include "../Parsing/Config/Location.hpp"
# include "../utility/utility.hpp"
# include <map>
# include <iomanip> // only for linux

namespace ws {
	
	class Response
	{
		public:
			bool response_is_tmp;
			
		public:
			Response();
			~Response();
			std::string getHeaders(Request &request, Config &config, std::string statusCode);
			std::pair<std::string, bool> getbody();
			
		private:
			int buildResponseTry;
			std::string bodyPath;
			std::string statusCode;
			std::map<std::string, std::string> headers_list;

		private:
			std::string headerBuilder();

			void setDateHeader();
			void setContentLength(std::string filePath);
			void setHeader(std::string key, std::string value);

			long long getFileSize(std::string filePath);
			std::string getMessage(std::string statusCode);
		
	};

}

#endif
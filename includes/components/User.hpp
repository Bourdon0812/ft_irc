/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilbonnev <ilbonnev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:08:46 by ilbonnev          #+#    #+#             */
/*   Updated: 2025/09/10 14:16:33 by ilbonnev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <set>

class User
{
	private:
		bool					passOK;
		bool					nickOk;
		bool					userOk;
		bool					welcome;
		bool					quit;
		
		std::string				nick;
		std::string				username;
		std::string				realname;
		std::string				hostname;

		std::string				inBuf;
		std::string				outBuf;

		std::set<std::string>	channels;
		
	public:
		User();

		bool							getPassOK() const;
		bool							getNickOk() const;
		bool							getUserOk() const;
		bool							getWelcome() const;
		bool							getQuit() const;

		const std::string&				getNick() const;
		const std::string&				getUsername() const;
		const std::string&				getRealname() const;
		const std::string&				getHostname() const;

		const std::string&				getInBuf() const;
		const std::string&				getOutBuf() const;

		const std::set<std::string>&	getChannels() const;

		void							setPassOK(bool value);
		void							setNickOk(bool value);
		void							setUserOk(bool value);
		void							setWelcome(bool value);
		void							setQuit(bool value);							
		void							setNick(const std::string& value);
		void							setUsername(const std::string& value);
		void							setRealname(const std::string& value);
		void							setHostname(const std::string& value);							
		void							setInBuf(const std::string& value);
		void							setOutBuf(const std::string& value);							

		void							addChannel(const std::string& channel);
		void							removeChannel(const std::string& channel);
};

#endif

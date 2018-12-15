//============================================================================
// Name        : rsa.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include<iostream>
#include<math.h>
#include<string>
#include<map>
#include <chrono>
#include <ctime>
#include <utility>

class Server {
	std::map<long int, std::pair<std::string, std::string>> messages;

public:
	Server() {
		messages.clear();
	}

	void listAllMessages() {
		for (std::map<long int, std::pair<std::string, std::string>>::iterator it =
				messages.begin(); it != messages.end(); ++it) {
			std::cout << std::ctime(&it->first) << " - " << it->second.first
					<< " : " << it->second.second << std::endl;
		}
	}

	void addMessage(std::string sender, std::string message) {
		std::pair<std::string, std::string> tmpPair(sender, message);
		messages.insert(
				{ std::chrono::system_clock::to_time_t(
						std::chrono::system_clock::now()), tmpPair });
	}

};

class Rsa {
public:
	long int publicKey[50], temp[50];
	Rsa(std::string _nameOfClient, int _primeNumber, int _anotherPrimeNumber) :
			nameOfClient(_nameOfClient), primeNumber(_primeNumber), anotherPrimeNumber(
					_anotherPrimeNumber) {
		t = (primeNumber - 1) * (anotherPrimeNumber - 1);
		n = primeNumber * anotherPrimeNumber;
		encryption_key();
	}

	std::string sendMessage(long int _publicKey[], long int _temp[],
			std::string _messageToEncrypt, Server* _srv) {
		messageToEncrypt = _messageToEncrypt;
		_srv->addMessage(nameOfClient, _messageToEncrypt);
		convertMessage();
		encrypt(_publicKey, _temp);
		std::string result = "";
		for (int i = 0; enMsg[i] != -1; i++)
			result += enMsg[i];
		return result;
	}

	void receiveMessage(std::string _encryptedMessage) {
		messageToDecrypt = _encryptedMessage;
		convertEnMessage();
		std::string tempStr = "";
		std::cout << "\n\nenMsg in string IS\n";
		for (int i = 0; enMsg[i] != -1; i++)
			tempStr += enMsg[i];
		std::cout << tempStr;
		decrypt();
	}

	void encrypt(long int _publicKey[], long int _temp[]) {
		convertMessage();
		long int pt, ct, key = _publicKey[0], k, len;
		int i = 0;
		len = messageToEncrypt.length();

		while (i != len) {
			pt = msg[i];
			pt = pt - 96;
			k = 1;
			for (int j = 0; j < key; j++) {
				k = k * pt;
				k = k % n;
			}
			_temp[i] = k;
			ct = k + 96;
			enMsg[i] = ct;
			i++;
		}
		enMsg[i] = -1;
		std::cout << "\n\nTHE ENCRYPTED MESSAGE IS\n";
		for (i = 0; enMsg[i] != -1; i++)
			std::cout << enMsg[i];
	}

	void decrypt() {
		convertEnMessage();
		std::cout << "\n\nenMsg values in decrypt\n";
		for (int i = 0; enMsg[i] != -1; i++)
			std::cout << enMsg[i];
		long int pt, ct, key = privateKey[0], k;
		int i = 0;
		while (enMsg[i] != -1) {
			ct = temp[i];
			k = 1;
			for (int j = 0; j < key; j++) {
				k = k * ct;
				k = k % n;
			}
			pt = k + 96;
			msg[i] = pt;
			i++;
		}
		msg[i] = -1;
		std::cout << "\n\nTHE DECRYPTED MESSAGE IS\n";
		for (i = 0; msg[i] != -1; i++)
			std::cout << msg[i];

		std::cout << std::endl;
	}

	void setMessageToDecrypt(std::string message) {
		messageToDecrypt = message;
	}

	void setMessageToEncrypt(std::string message) {
		messageToEncrypt = message;
	}

private:
	std::string nameOfClient, messageToEncrypt, messageToDecrypt;
	int primeNumber, anotherPrimeNumber, t, n;
	long int privateKey[50];
	char msg[255], enMsg[255];

	void convertMessage() {
		messageToEncrypt.copy(msg, messageToEncrypt.size() + 1);
		msg[messageToEncrypt.length()] = -1;
		std::cout << "\n\nTHE SIMPLE CONVERT IS\n";
		for (int i = 0; msg[i] != -1; i++)
			std::cout << msg[i];
	}

	void convertEnMessage() {
		messageToDecrypt.copy(enMsg, messageToDecrypt.size() + 1);
		enMsg[messageToDecrypt.length()] = -1;
		std::cout << "\n\nTHE ENCRYPTED CONVERT IS\n";
		for (int i = 0; enMsg[i] != -1; i++)
			std::cout << enMsg[i];
	}

	int prime(long int pr) {
		int j = sqrt(pr);
		for (int i = 2; i <= j; i++) {
			if (pr % i == 0)
				return 0;
		}
		return 1;
	}

	void encryption_key() {
		int k;
		k = 0;
		for (int i = 2; i < t; i++) {
			int flag;
			if (t % i == 0)
				continue;
			flag = prime(i);
			if (flag == 1 && i != primeNumber && i != anotherPrimeNumber) {
				publicKey[k] = i;
				flag = cd(publicKey[k]);
				if (flag > 0) {
					privateKey[k] = flag;
					k++;
				}
				if (k == 99)
					break;
			}
		}
	}

	long int cd(long int a) {
		long int k = 1;
		while (1) {
			k = k + t;
			if (k % a == 0)
				return (k / a);
		}
	}

};

int main() {
	Rsa alice("Alice", 13, 17);
	Rsa bob("Bob", 5, 7);

	Server* srv = new Server();

	std::string en = bob.sendMessage(alice.publicKey, alice.temp, "hello", srv);
	alice.receiveMessage(en);

	std::cout << "\n";
	std::cout << "Messages stored by the server:" <<std::endl;
	srv->listAllMessages();
}


//============================================================================
// Name        : rsa.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//C++ program for encryption and decryption
#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<string.h>

using namespace std;

//int x, y, n, t, i, flag;
//long int e[50] public, d[50] private, temp[50], j;
//char en[50], m[50];
//char msg[100];

class Rsa {
	std::string nameOfClient, messageToEncrypt, messageToDecrypt;
	int primeNumber, anotherPrimeNumber, t, n;
	long int temp[50], publicKey[50], privateKey[50];
	char msg[255], enMsg[255];

	void convertMessage() {
		strncpy(msg, messageToEncrypt.c_str(), messageToEncrypt.length());
		msg[messageToEncrypt.length()] = -1;
		cout << "\n\nTHE SIMPLE CONVERT IS\n";
		for (int i = 0; msg[i] != -1; i++)
					cout << msg[i];
	}

	void convertEnMessage() {
		strncpy(enMsg, messageToDecrypt.c_str(), messageToDecrypt.length());
		enMsg[messageToDecrypt.length()] = -1;
		cout << "\n\nTHE ENCRYPTED CONVERT IS\n";
		for (int i = 0; enMsg[i] != -1; i++)
			cout << enMsg[i];
	}

	int prime(long int pr) {
		int j = sqrt(pr);
		for (int i = 2; i <= j; i++) {
			if (pr % i == 0)
				return 0;
		}
		return 1;
	}

	//function to generate encryption key
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

public:
	Rsa(std::string _nameOfClient, int _primeNumber,
			int _anotherPrimeNumber) :
			nameOfClient(_nameOfClient), primeNumber(
					_primeNumber), anotherPrimeNumber(_anotherPrimeNumber) {
		t = (primeNumber - 1) * (anotherPrimeNumber - 1);
		n = primeNumber * anotherPrimeNumber;
		encryption_key();
	}

	std::string sendMessage(long int _publicKey[],
			std::string _messageToEncrypt) {
		messageToEncrypt = _messageToEncrypt;
		convertMessage();
		encrypt(_publicKey);
		std::string temp;
		temp.clear();
		for (int i = 0; enMsg[i] != -1; i++)
			temp += enMsg[i];
		return temp;
	}

	void receiveMessage(std::string _encryptedMessage) {
		messageToDecrypt = _encryptedMessage;
		convertEnMessage();
		decrypt();
	}

	//function to encrypt the message
	void encrypt(long int _publicKey[]) {
		long int pt, ct, key = _publicKey[0], k, len;
		int i = 0;
		len = strlen(msg);

		while (i != len) {
			pt = msg[i];
			pt = pt - 96;
			k = 1;
			for (int j = 0; j < key; j++) {
				k = k * pt;
				k = k % n;
			}
			temp[i] = k;
			ct = k + 96;
			enMsg[i] = ct;
			i++;
		}
		enMsg[i] = -1;
		cout << "\n\nTHE ENCRYPTED MESSAGE IS\n";
		for (i = 0; enMsg[i] != -1; i++)
			cout << enMsg[i];
	}

	//function to decrypt the message
	void decrypt() {
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
		cout << "\n\nTHE DECRYPTED MESSAGE IS\n";
		for (i = 0; msg[i] != -1; i++)
			cout << msg[i];

		cout << endl;
	}

	long int* getPublicKey() {
		return publicKey;
	}
};

int main() {
	Rsa* alice = new Rsa("Alice", 13, 17);
	Rsa* bob = new Rsa("Bob", 5, 7);
	alice->receiveMessage(bob->sendMessage(alice->getPublicKey(), "ligma"));
//	Rsa rsa;
//   cout << "\nENTER FIRST PRIME NUMBER\n";
//   cin >> x;
//
//   //checking whether input is prime or not
//   flag = rsa.prime(x);
//   if(flag == 0)
//   {
//      cout << "\nINVALID INPUT\n";
//      exit(0);
//   }
//
//   cout << "\nENTER SECOND PRIME NUMBER\n";
//   cin >> y;
//
//   flag = rsa.prime(y);
//   if(flag == 0 || x == y)
//   {
//      cout << "\nINVALID INPUT\n";
//      exit(0);
//   }
//
//   cout << "\nENTER MESSAGE OR STRING TO ENCRYPT\n";
//   cin >> msg;
//
//   for(i = 0; msg[i] != 0; i++)
//      m[i] = msg[i];
//   n = x * y;
//   t = (x - 1) * (y - 1);
//
//   rsa.encryption_key();
//   cout << "\nPOSSIBLE VALUES OF e AND d ARE\n";
//
//   for(i = 0; i < j - 1; i++)
//      cout << "\n" << e[i] << "\t" << d[i];
//
//   rsa.encrypt();
//   rsa.decrypt();
//   return 0;
} //end of the main program


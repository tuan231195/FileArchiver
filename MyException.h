/* 
 * File:   MyException.h
 * Author: bx830
 *
 * Created on 19 August 2015, 4:45 PM
 */

#ifndef MYEXCEPTION_H
#define	MYEXCEPTION_H

#include <string>
#include <exception>
using namespace std;

class MyException : public exception {
public:
    MyException(string& whatsup) : message(whatsup) {  }
    ~MyException() throw() {}
    const char* what(){return message.c_str();}
private:
    string message;
};

#endif	/* MYEXCEPTION_H */


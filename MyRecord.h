/* 
 * File:   MyRecord.h
 * Author: bx830
 *
 * Created on 19 August 2015, 4:45 PM
 */

#ifndef MYRECORD_H
#define	MYRECORD_H

#include "MyException.h"
#include <map>
#include <vector>
#include <algorithm>
#include <boost/regex.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
using namespace std;

class MyRecord {
public:
    MyRecord();
    explicit MyRecord(string& id); // I don't want implicit conversion from std::string to MyRecord!
    // Mutator functions - setting of other fields
    // In this version, hardly any of the implementations will actually throw exceptions
    // later validation functions will be added
    void setName(string& aname) throw (MyException);
    void setEmail(string& amail) throw (MyException);
    void setInfo(string& info) throw (MyException);
    void setImage(string& imagestr) throw (MyException);
    void addRole(string& anotherrole) ;
    void addKeyValue(string& collectionname, string& key, string& value) throw(MyException);
    // Accessor functions

    string getID() const { return this->id; }
    string getName() const { return this->name; }
    string getEmail() const { return this->email; }
    string getInfo() const { return this->info; }
    string getImage() const { return this->image; }
    string getAttribute(string& collectionname, string& key) const throw(MyException);
    const vector<string>& getRoles() const { return this->roles; }
    const map<string,string>& getPhones() const { return this->phones; }
    //string getPhones() const {return this->phones;}
    const map<string,string>& getAddresses() const { return this->addresses; }
    const map<string,string>& getOtherKV() const { return this->other; }
    
    bool hasRole(string& queryrole) const;
    template<class Archive>
    void serialize(Archive& archive, const unsigned int)
    {
        archive & BOOST_SERIALIZATION_NVP(id);
        archive & BOOST_SERIALIZATION_NVP(name);
        archive & BOOST_SERIALIZATION_NVP(email);
        archive & BOOST_SERIALIZATION_NVP(info);
        archive & BOOST_SERIALIZATION_NVP(roles);
        archive & BOOST_SERIALIZATION_NVP(phones);
        archive & BOOST_SERIALIZATION_NVP(addresses);
        archive & BOOST_SERIALIZATION_NVP(other);
        archive & BOOST_SERIALIZATION_NVP(image);

    }
    /*
    friend ostream& operator<<(ostream& out,const MyRecord&);
    friend istream& operator>>(istream& ins,MyRecord&);
    void printOn(ostream&)const;
    void readFrom(istream&);
    */
private:
    string id; // Also known as "nickname", or even "primary key"
    string name; // full name
    string email; 
    string image;
    string info;
    vector<string> roles;
    map<string,string> phones;
    //string phones;
    map<string,string> addresses;
    map<string,string> other;
     // Finally, note that copy constructor and assignment operator are
    // private (and no implementations will be defined).  I choose to 
    // disallow such operations.
    MyRecord(const MyRecord& orig);
    MyRecord& operator=(const MyRecord&);
    // Further there is no virtual destructor, I do not intend MyRecord
    // to be the base class in some hierarchy.
    
};

#endif	/* MYRECORD_H */


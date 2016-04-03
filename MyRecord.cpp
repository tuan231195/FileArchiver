#include "MyRecord.h"

using namespace boost;

MyRecord::MyRecord()
{

}
MyRecord::MyRecord(string& id) {
    this->id = id;
}

void MyRecord::setName(string& aname) throw (MyException) {
    this->name = aname;
}

void MyRecord::setEmail(string& amail) throw (MyException) {
    //this->email = amail;
    boost::regex validationExpression("^[a-z0-9_\\+-]+(\\.[a-z0-9_\\+-]+)*@[a-z0-9-]+(\\.[a-z0-9-]+)*\\.([a-z]{2,4})$");
    if(boost::regex_match(amail,validationExpression)){
        this->email = amail.c_str();
    }else{
        std::string msg="The email address cannot be matched";
        throw MyException(msg); 
    }
}

void MyRecord::setInfo(string& info) throw (MyException) {
    this->info = info;
}

void MyRecord::setImage(string& imagestr) throw (MyException) {
    this->image = imagestr;
}

void MyRecord::addRole(string& anotherrole)  {
    // Should it be an error to specify the same role more than once? No, it
    // will not really matter - so don't throw exception, just don't duplicate.
    vector<string>::iterator it;
    it = find(this->roles.begin(), this->roles.end(), anotherrole);
    if (it != this->roles.end()) return; // Found it, so don't duplicate
    roles.push_back(anotherrole);
}

void MyRecord::addKeyValue(string& collectionname, string& key, string& value) throw (MyException) {
    if (key.empty() || value.empty()) {
        string msg = "Empty key or value";
        throw MyException(msg);
    }
    // If same collection/key combination specified more than once, later value
    // overwrites earlier value for that key.
   
    //Change to allow for STL's insert() not allowing for replacement
    if (collectionname == "Phones") {
        this->phones[key]=value;
    } else
        if (collectionname == "Addresses") {
        this->addresses[key]=value;
    } else
        if (collectionname == "Other") {
         this->other[key]=value;       
    } else {
        string errmsg = "There is no collection called " + collectionname;
        throw MyException(errmsg);
    }
}

string MyRecord::getAttribute(string& collectionname, string& key) const throw (MyException) {


    if (collectionname == "Phones") {
        return phones.find(key)->second;
    } else
        if (collectionname == "Addresses") {
        return addresses.find(key)->second;
    } else
        if (collectionname == "Other") {
        return other.find(key)->second;
    }

    string errmsg = "There is no collection called " + collectionname;
    throw MyException(errmsg);

}


bool MyRecord::hasRole(string&  anotherrole) const  {
    // Should it be an error to specify the same role more than once? No, it
    // will not really matter - so don't throw exception, just don't duplicate.
    vector<string>::const_iterator it;
    it = find(this->roles.begin(), this->roles.end(), anotherrole);
   return(it != this->roles.end()) ;
}


/*
void MyRecord::readFrom(istream& ins)
{
  
}
istream& operator >>(istream& ins,MyRecord& rec)
{
    rec.readFrom(ins);
    return ins;
}
ostream& operator <<(ostream& out,const MyRecord& rec)
{
    rec.printOn(out);
    return out;
}
void MyRecord::printOn(ostream& out) const
{
    out << "\t<myrecord>" << endl;
    out << "\t\t<id>" << id <<  "</id>" << endl;  
    out << "\t\t<name>" <<  name << "</name>" << endl;
    out << "\t\t<email>" << email << "</email>" << endl;
    out << "\t\t<image>" << image << "</image>" << endl;
    out << "\t\t<info>" << info << "</info>" << endl;
    out << "\t\t<roles>" << endl;
//    out << roles.size() << endl;
    vector<string>::const_iterator it1;
    for(it1=roles.begin();it1!=roles.end();it1++)
    {
        out << "\t\t\t<role>" << (*it1) << "</role>" << endl;
    }
    out << "\t\t</roles>" << endl;

//    out << phones.size() << endl;
    
    out << "\t\t<phones>" << endl;
    map<string,string>::const_iterator it2;
    for(it2=phones.begin();it2!=phones.end();it2++)
    {
        out << "\t\t\t<phonerec>" << endl;
        out << "\t\t\t\t<type>" << (*it2).first << "</type>" << endl;
        out << "\t\t\t\t<number>" << (*it2).second << "</number>" << endl;
        out << "\t\t\t</phonerec>" << endl;
    }
    out << "\t\t</phones>" << endl;
    
 //   out << addresses.size()<< endl;
    
    out << "\t\t<addresses>" << endl;
    for(it2=addresses.begin();it2!=addresses.end();it2++)
    {    
      out << "<\t\t\taddress>" << (*it2).first << " " <<(*it2).second << "</address>" << endl;
    }
     out << "\t\t</addresses>" << endl;
 //   out << other.size()<< endl;
    out << "\t\t<others>" << endl;
    for(it2=other.begin();it2!=other.end();it2++)
    {    
      out << "\t\t\t<otherrec>" << endl;
      out << "\t\t\t\t<key>" << (*it2).first << "</key>" << endl;
      out << "\t\t\t\t<value>" <<(*it2).second << "</value>" << endl;           
      out << "\t\t\t</otherrec>" << endl;
    }
    out << "\t\t</others>" << endl;
     out << "\t</myrecord>" << endl;
}
*/


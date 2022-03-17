#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream> 
#include <iomanip> 
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

//doresit binary search, spis lower_bound

class Company{
    string m_name;
    string m_addr;
    string m_taxID;
    int m_sum = 0;

public:
    Company (string name, string addr, string taxID)
    : m_name(name), m_addr(addr), m_taxID(taxID)
    {}

    bool isSameCompanyName (string name, string addr) const{
        return (strcasecmp(m_name.c_str(), name.c_str()) == 0) && (strcasecmp(m_addr.c_str(), addr.c_str()) == 0);
    }

    bool isSameCompanyID (string taxID) const{
        return m_taxID == taxID;
    }

    static int cmp (const Company & a, const Company & b){
        if( strcmp(a.m_name.c_str(),b.m_name.c_str()) == 0){
            return (strcmp(a.m_addr.c_str(), b.m_addr.c_str())< 0);
        } else return strcmp(a.m_name.c_str(),b.m_name.c_str()) < 0;
    }

    void addAmount (int amount) {
        m_sum += amount;
    }

    inline int getSum(void) const {return m_sum;}
    inline string getName(void) const {return m_name;}
    inline string getAddr(void) const {return m_addr;}

};

class CVATRegister {
    vector<Company> db;
    vector<int> amounts;
    mutable int cnt = 1;

public:
    CVATRegister(void) {}

    ~CVATRegister(void) {}

    bool newCompany(const string &name, const string &addr, const string &taxID) {
        for (const auto &x: db) {
            if (x.isSameCompanyName(name, addr) || x.isSameCompanyID(taxID)) {
                return false;
            }
        }
        db.push_back(Company(name, addr, taxID));
        sort(db.begin(), db.end(), Company::cmp);

        for (size_t i = 0; i < db.size(); i++){
            cout << db[i].getName() << endl;
            cout << db[i].getAddr() << endl;
        }
        cout << endl;
        return true;
    }

    bool cancelCompany(const string &name, const string &addr) {
        for (size_t i = 0; i < db.size(); i++) {
            if (db[i].isSameCompanyName(name, addr)) {
                db.erase(db.begin() + i);
                return true;
            }
        }
        return false;
    }

    bool cancelCompany(const string &taxID) {
        for (size_t i = 0; i < db.size(); i++) {
            if (db[i].isSameCompanyID(taxID)) {
                db.erase(db.begin() + i);
                return true;
            }
        }
        return false;
    }

    bool invoice(const string &taxID, unsigned int amount) {
       // binary_search(db.begin(), db.end(), taxID);
        for (size_t i = 0; i < db.size(); i++) {
            if (db[i].isSameCompanyID(taxID)) {
                db[i].addAmount(amount);
                amounts.push_back(amount);
                sort(amounts.begin(), amounts.end());
                return true;
            }
        }
        return false;
    }

    bool invoice(const string &name, const string &addr, unsigned int amount) {
        for (size_t i = 0; i < db.size(); i++) {
            if (db[i].isSameCompanyName(name, addr)) {
                db[i].addAmount(amount);
                amounts.push_back(amount);
                sort(amounts.begin(), amounts.end());
                return true;
            }
        }
        return false;
    }

    bool audit(const string &name, const string &addr, unsigned int &sumIncome) const {
        for (size_t i = 0; i < db.size(); i++) {
            if (db[i].isSameCompanyName(name, addr)) {
                sumIncome = db[i].getSum();
                return true;
            }
        }
        return false;
    }

    bool audit(const string &taxID, unsigned int &sumIncome) const {
        for (size_t i = 0; i < db.size(); i++) {
            if (db[i].isSameCompanyID(taxID)) {
                sumIncome = db[i].getSum();
                return true;
            }
        }
        return false;
    }

    bool firstCompany(string &name, string &addr) const {
        if (db.size() == 0) return false;
        name = db[0].getName();
        addr = db[0].getAddr();
        cout << name << endl;
        cout << addr << endl;
        return true;
    }

    bool nextCompany(string &name, string &addr) const {
        cout << db.size() << endl;
        if (cnt >= int(db.size())) return false;
        name = db[cnt].getName();
        addr = db[cnt].getAddr();
        cout << name << endl;
        cout << addr << endl;
        cnt++;
        return true;
    }

    unsigned int medianInvoice(void) const {
        int size = amounts.size();
        if (size == 0) return 0;
        if (size == 1) return amounts[0];
        int middle = size / 2;
        if (size % 2){
            //liche
            return amounts[middle];
        }
        else{
            //sude
            return amounts[middle];
        }
    }
};

#ifndef __PROGTEST__
int               main           ( void )
{
  string name, addr;
  unsigned int sumIncome;

  CVATRegister b1;
  assert ( b1 . newCompany ( "ACME", "Thakurova", "666/666" ) );
  assert ( b1 . newCompany ( "ACME", "Kolejni", "666/666/666" ) );
  assert ( b1 . newCompany ( "Dummy", "Thakurova", "123456" ) );
  assert ( b1 . invoice ( "666/666", 2000 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "666/666/666", 3000 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 4000 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "aCmE", "Kolejni", 5000 ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . audit ( "ACME", "Kolejni", sumIncome ) && sumIncome == 8000 );
  assert ( b1 . audit ( "123456", sumIncome ) && sumIncome == 4000 );
  assert ( b1 . firstCompany ( name, addr ) && name == "ACME" && addr == "Kolejni" );
  assert ( b1 . nextCompany ( name, addr ) && name == "ACME" && addr == "Thakurova" );
  assert ( b1 . nextCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
  assert ( ! b1 . nextCompany ( name, addr ) );
  assert ( b1 . cancelCompany ( "ACME", "KoLeJnI" ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . cancelCompany ( "666/666" ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . invoice ( "123456", 100 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 300 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 200 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 230 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 830 ) );
  assert ( b1 . medianInvoice () == 830 );
  assert ( b1 . invoice ( "123456", 1830 ) );
  assert ( b1 . medianInvoice () == 1830 );
  assert ( b1 . invoice ( "123456", 2830 ) );
  assert ( b1 . medianInvoice () == 1830 );
  assert ( b1 . invoice ( "123456", 2830 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 3200 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . firstCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
  assert ( ! b1 . nextCompany ( name, addr ) );
  assert ( b1 . cancelCompany ( "123456" ) );
  assert ( ! b1 . firstCompany ( name, addr ) );

  CVATRegister b2;
  assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . newCompany ( "Dummy", "Kolejni", "123456" ) );
  assert ( ! b2 . newCompany ( "AcMe", "kOlEjNi", "1234" ) );
  assert ( b2 . newCompany ( "Dummy", "Thakurova", "ABCDEF" ) );
  assert ( b2 . medianInvoice () == 0 );
  assert ( b2 . invoice ( "ABCDEF", 1000 ) );
  assert ( b2 . medianInvoice () == 1000 );
  assert ( b2 . invoice ( "abcdef", 2000 ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( b2 . invoice ( "aCMe", "kOlEjNi", 3000 ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( ! b2 . invoice ( "1234567", 100 ) );
  assert ( ! b2 . invoice ( "ACE", "Kolejni", 100 ) );
  assert ( ! b2 . invoice ( "ACME", "Thakurova", 100 ) );
  assert ( ! b2 . audit ( "1234567", sumIncome ) );
  assert ( ! b2 . audit ( "ACE", "Kolejni", sumIncome ) );
  assert ( ! b2 . audit ( "ACME", "Thakurova", sumIncome ) );
  assert ( ! b2 . cancelCompany ( "1234567" ) );
  assert ( ! b2 . cancelCompany ( "ACE", "Kolejni" ) );
  assert ( ! b2 . cancelCompany ( "ACME", "Thakurova" ) );
  assert ( b2 . cancelCompany ( "abcdef" ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( ! b2 . cancelCompany ( "abcdef" ) );
  assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . cancelCompany ( "ACME", "Kolejni" ) );
  assert ( ! b2 . cancelCompany ( "ACME", "Kolejni" ) );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */

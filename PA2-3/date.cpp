#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

//=================================================================================================
// a dummy exception class, keep this implementation
class InvalidDateException : public invalid_argument
{
public:
    InvalidDateException ( )
            : invalid_argument ( "invalid date or format" )
    {
    }
};
//=================================================================================================
// date_format manipulator - a dummy implementation. Keep this code unless you implement your
// own working manipulator.
ios_base & ( * date_format ( const char * fmt ) ) ( ios_base & x )
{
    return [] ( ios_base & ios ) -> ios_base & { return ios; };
}
//=================================================================================================
class CDate
{
    int m_year = 0;
    int m_month = 0;
    int m_day = 0;


public:
    CDate(int year, int month, int day)
    : m_year(year), m_month(month), m_day(day)
    {
        if(!checkdate(year,month,day)){
            throw InvalidDateException();
        }
    }

    static bool checkdate(int y, int m, int d)
    {
        if (! (1<= m && m<=12) )
            return false;
        if (! (1<= d && d<=31) )
            return false;
        if ( (d==31) && (m==2 || m==4 || m==6 || m==9 || m==11) )
            return false;
        if ( (d==30) && (m==2) )
            return false;
        if ( (m==2) && (d==29) && (y%4!=0) )
            return false;
        if ( (m==2) && (d==29) && (y%400==0) )
            return true;
        if ( (m==2) && (d==29) && (y%100==0) )
            return false;
        if ( (m==2) && (d==29) && (y%4==0)  )
            return true;

        return true;
    }

    CDate operator + (int a) const{
        struct tm date = {0,0,0};
        date.tm_year = m_year - 1900;
        date.tm_mon = m_month - 1;
        date.tm_mday = m_day;

        const time_t ONE_DAY = 24 * 60 * 60 ;
        time_t date_seconds = mktime( &date ) + (a * ONE_DAY) ;
        date = *localtime( &date_seconds ) ;
        return CDate(date.tm_year + 1900, date.tm_mon + 1, date.tm_mday);
    }

    CDate operator - (int a) const{
        struct tm date = {0,0,0};
        date.tm_year = m_year - 1900;
        date.tm_mon = m_month - 1;
        date.tm_mday = m_day;

        const time_t day = 24 * 60 * 60 ;
        time_t date_seconds = mktime( &date ) - (a * day) ;
        date = *localtime( &date_seconds ) ;
        return CDate(date.tm_year + 1900, date.tm_mon + 1, date.tm_mday);
    }

    int operator - (CDate a) const{
        struct tm date1 = {0,0,12};
        date1.tm_year = m_year - 1900;
        date1.tm_mon = m_month - 1;
        date1.tm_mday = m_day;

        struct tm date2 = {0, 0, 12};
        date2.tm_year = a.m_year - 1900;
        date2.tm_mon = a.m_month - 1;
        date2.tm_mday = a.m_day;

        double seconds = difftime(mktime(&date1), mktime(&date2));
        return seconds / 86400;
    }

    CDate operator ++ () {
        struct tm date = {0,0,0};
        date.tm_year = m_year - 1900;
        date.tm_mon = m_month - 1;
        date.tm_mday = m_day;

        const time_t day = 24 * 60 * 60 ;
        time_t date_seconds = mktime( &date ) + (day) ;
        date = *localtime( &date_seconds ) ;
        m_year = date.tm_year + 1900;
        m_month = date.tm_mon + 1;
        m_day = date.tm_mday;
        return *this;
    }

    CDate operator -- () {
        struct tm date = {0,0,12};
        date.tm_year = m_year - 1900;
        date.tm_mon = m_month - 1;
        date.tm_mday = m_day;

        const time_t day = 24 * 60 * 60 ;
        time_t date_seconds = mktime( &date ) - (day) ;
        date = *localtime( &date_seconds ) ;
        m_year = date.tm_year + 1900;
        m_month = date.tm_mon + 1;
        m_day = date.tm_mday;
        return *this;
    }

    const CDate operator ++ (int){
        CDate dup(*this);
        ++(*this);
        return dup;
    }

    const CDate operator -- (int){
        CDate dup(*this);
        --(*this);
        return dup;
    }

    bool operator == (CDate a) const{
        return m_year == a.m_year && m_month == a.m_month && m_day == a.m_day;
    }

    bool operator != (CDate a) const{
        return m_year != a.m_year || m_month != a.m_month || m_day != a.m_day;
    }

    bool operator > (CDate a) const{
        if (m_year == a.m_year){
            if(m_month == a.m_month){
                if (m_day == a.m_day){
                    return false;
                } else return m_day > a.m_day;
            } else return m_month > a.m_month;
        } else return m_year > a.m_year;
    }

    bool operator >= (CDate a) const{
        if (m_year == a.m_year){
            if(m_month == a.m_month){
                if (m_day == a.m_day){
                    return true;
                } else return m_day >= a.m_day;
            } else return m_month >= a.m_month;
        } else return m_year >= a.m_year;
    }

    bool operator < (CDate a) const{
        if (m_year == a.m_year){
            if(m_month == a.m_month){
                if (m_day == a.m_day){
                    return false;
                } else return m_day < a.m_day;
            } else return m_month < a.m_month;
        } else return m_year < a.m_year;
    }

    bool operator <= (CDate a) const{
        if (m_year == a.m_year){
            if(m_month == a.m_month){
                if (m_day == a.m_day){
                    return true;
                } else return m_day <= a.m_day;
            } else return m_month <= a.m_month;
        } else return m_year <= a.m_year;
    }

    friend ostream & operator << (ostream & os, const CDate a)
    {
        return os << a.m_year<< '-' << setfill('0')<< setw(2) << a.m_month << '-' << setfill ('0') << setw(2) << a.m_day ;
    }


    friend istream & operator >> (istream & is, CDate & a) {
        char dash = 0;
        int y = a.m_year;
        int m = a.m_month;
        int d = a.m_day;
        is >> a.m_year >> dash >> a.m_month >> dash >> a.m_day;
        if (dash != '-' || !checkdate(a.m_year, a.m_month, a.m_day)) {
            is.setstate(ios::failbit);
            a.m_year = y;
            a.m_month = m;
            a.m_day = d;
        }
        return is;
    }
};



#ifndef __PROGTEST__
int main ( void )
{
    ostringstream oss;
    istringstream iss;

    CDate a ( 2000, 1, 2 );
    CDate b ( 2010, 2, 3 );
    CDate c ( 2004, 2, 10 );
    oss . str ("");
    oss << a;
    assert ( oss . str () == "2000-01-02" );
    oss . str ("");
    oss << b;
    assert ( oss . str () == "2010-02-03" );
    oss . str ("");
    oss << c;
    assert ( oss . str () == "2004-02-10" );
    a = a + 1500;
    oss . str ("");
    oss << a;
    assert ( oss . str () == "2004-02-10" );
    b = b - 2000;
    oss . str ("");
    oss << b;
    assert ( oss . str () == "2004-08-13" );
    assert ( b - a == 185 );
    assert ( ( b == a ) == false );
    assert ( ( b != a ) == true );
    assert ( ( b <= a ) == false );
    assert ( ( b < a ) == false );
    assert ( ( b >= a ) == true );
    assert ( ( b > a ) == true );
    assert ( ( c == a ) == true );
    assert ( ( c != a ) == false );
    assert ( ( c <= a ) == true );
    assert ( ( c < a ) == false );
    assert ( ( c >= a ) == true );
    assert ( ( c > a ) == false );
    a = ++c;
    oss . str ( "" );
    oss << a << " " << c;
    assert ( oss . str () == "2004-02-11 2004-02-11" );
    a = --c;
    oss . str ( "" );
    oss << a << " " << c;
    assert ( oss . str () == "2004-02-10 2004-02-10" );
    a = c++;
    oss . str ( "" );
    oss << a << " " << c;
    assert ( oss . str () == "2004-02-10 2004-02-11" );
    a = c--;
    oss . str ( "" );
    oss << a << " " << c;
    assert ( oss . str () == "2004-02-11 2004-02-10" );
    iss . clear ();
    iss . str ( "2015-09-03" );
    assert ( ( iss >> a ) );
    oss . str ("");
    oss << a;
    assert ( oss . str () == "2015-09-03" );
    a = a + 70;
    oss . str ("");
    oss << a;
    assert ( oss . str () == "2015-11-12" );

    CDate d ( 2000, 1, 1 );
    try
    {
        CDate e ( 2000, 32, 1 );
        assert ( "No exception thrown!" == nullptr );
    }
    catch ( ... )
    {
    }
    iss . clear ();
    iss . str ( "2000-12-33" );
    assert ( ! ( iss >> d ) );
    oss . str ("");
    oss << d;
    assert ( oss . str () == "2000-01-01" );
    iss . clear ();
    iss . str ( "2000-11-31" );
    assert ( ! ( iss >> d ) );
    oss . str ("");
    oss << d;
    assert ( oss . str () == "2000-01-01" );
    iss . clear ();
    iss . str ( "2000-02-29" );
    assert ( ( iss >> d ) );
    oss . str ("");
    oss << d;
    assert ( oss . str () == "2000-02-29" );
    iss . clear ();
    iss . str ( "2001-02-29" );
    assert ( ! ( iss >> d ) );
    oss . str ("");
    oss << d;
    assert ( oss . str () == "2000-02-29" );



    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
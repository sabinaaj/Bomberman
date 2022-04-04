#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CDate
{
private:
    int m_y;
    int m_m;
    int m_d;
public:
    CDate (int y, int m, int d)
    : m_y(y), m_m(m), m_d(d)
    {}

    bool operator > (CDate a) const{
        if (m_y == a.m_y){
            if(m_m == a.m_m){
                if (m_d == a.m_d){
                    return false;
                } else return m_d > a.m_d;
            } else return m_m > a.m_m;
        } else return m_y > a.m_y;
    }

    bool operator < (CDate a) const{
        if (m_y == a.m_y){
            if(m_m == a.m_m){
                if (m_d == a.m_d){
                    return false;
                } else return m_d < a.m_d;
            } else return m_m < a.m_m;
        } else return m_y < a.m_y;
    }

    bool operator >= (CDate a) const{
        if (m_y == a.m_y){
            if(m_m == a.m_m){
                if (m_d == a.m_d){
                    return true;
                } else return m_d >= a.m_d;
            } else return m_m >= a.m_m;
        } else return m_y >= a.m_y;
    }


    bool operator != (CDate a) const{
        return m_y != a.m_y || m_m != a.m_m || m_d != a.m_d;
    }

    friend ostream & operator << (ostream & os, const CDate a)
    {
        return os << a.m_y<< '-' << setfill('0')<< setw(2) << a.m_m<< '-' << setfill ('0') << setw(2) << a.m_d ;
    }

};

class CSupermarket
{
private:
    struct Goods{
        string m_name;
        CDate m_date;
        mutable int m_cnt;

        Goods(string name, CDate date, int cnt)
        : m_name(std::move(name)), m_date(date), m_cnt(cnt)
        {}

        static bool cmpName (const Goods & a, const Goods & b){
            if (a.m_name == b.m_name){
                return (a.m_date < b.m_date);
            }
            else return a.m_name < b.m_name;
        }

    };
    vector<Goods> goods;

public:
    CSupermarket()= default;

    void store (const string & name, CDate expiryDate, int count){
        Goods x = Goods(name, expiryDate, 0);
        auto pos = lower_bound(goods.begin(),goods.end(),x, Goods::cmpName);

        if (pos == goods.end() || pos->m_name != name|| pos->m_date != expiryDate){
            //nenajde
            goods.insert(pos, Goods(name, expiryDate,count));
          /* for (auto & good : goods){
                cout << good.m_name<< endl;
                cout << good.m_date << endl;
            }
            cout << endl;*/
        }
        else {
            //TO ZVLADNES <3
            //najde
            pos->m_cnt += count;
        }

    }

    list<pair<string,int>> sell (list<pair<string,int>> & shoppingList){
        list<pair<string,int>> sell;
        for (auto & it : shoppingList){
            auto pos = lower_bound(goods.begin(),goods.end(), it.first, [](const Goods & a, string b)
                                                                                                     {return a.m_name < b;});
            if(pos == goods.end() || pos->m_name != it.first){
                //nenajde
                sell.emplace_back(it.first, it.second);
            }
            else{
                //najde
                if (it.second <= pos->m_cnt){
                    //muze se prodat vse
                    pos->m_cnt -= it.second;
                }
                else{
                    //neni dostatek zbozi
                    int remain = it.second - pos->m_cnt;
                    auto pos2 = next(pos,1);
                    pos->m_cnt = 0;
                    if (pos->m_name == pos2->m_name){
                        cout<< "test " << pos->m_name << " " << pos2->m_name << endl;
                        if (pos2->m_cnt >= remain){
                            //u dalsiho zbozi se naslo zbozi, ktere se muze prodat
                            pos2->m_cnt -= remain;
                        }
                        else{
                            int remain = it.second - pos->m_cnt;
                            pos2->m_cnt = 0;
                            sell.emplace_back(it.first, remain);
                        }
                    } else{
                        //neni nikdo stejneho jmena
                    sell.emplace_back(it.first, remain);
                    }
                }
            }
        }
        for (auto & good : sell){
            cout << good.first<< endl;
            cout << good.second << endl;
        }
        cout << endl;
        shoppingList = sell;
        return shoppingList;
    }

    list<pair<string,int>> expired (CDate date) const{
        list<pair<string,int>> expired;
        int temp = 0;
        for(int i = 0; i < goods.size(); i++){
           // cout << goods[i].m_name << endl;
            if (date > goods[i].m_date && goods[i].m_cnt != 0){
                //cout << goods[i].m_name << endl;
                if (i + 1 == goods.size()){
                    expired.emplace_back(goods[i].m_name, goods[i].m_cnt);
                }
                else if (goods[i].m_name == goods[i + 1].m_name) {
                    if (goods[i+1].m_date >= date){
                        expired.emplace_back(goods[i].m_name, goods[i].m_cnt);
                    } else {
                        temp = goods[i + 1].m_cnt + goods[i].m_cnt;
                    }

                }
                else{
                    if (temp != 0){
                        expired.emplace_back(goods[i].m_name, temp);
                        temp = 0;
                    } else expired.emplace_back(goods[i].m_name, goods[i].m_cnt);
                }
            }
        }
        expired.sort([](const std::pair<string, int> &a, const std::pair<string, int> &b)
        {return a.second > b.second;});
        return expired;
    }
};
#ifndef __PROGTEST__
int main ( void )
{
    CSupermarket s;
    s . store ( "bread", CDate ( 2016, 4, 30 ), 100 );
    s . store ( "butter", CDate ( 2016, 5, 10 ), 10 );
    s . store ( "beer", CDate ( 2016, 8, 10 ), 50 );
    s . store ( "bread", CDate ( 2016, 4, 25 ), 100 );
    s . store ( "okey", CDate ( 2016, 7, 18 ), 5 );

    list<pair<string,int> > l0 = s . expired ( CDate ( 2018, 4, 30 ) );
    assert ( l0 . size () == 4 );
    assert ( ( l0 == list<pair<string,int> > { { "bread", 200 }, { "beer", 50 }, { "butter", 10 }, { "okey", 5 } } ) );

    list<pair<string,int> > l1 { { "bread", 2 }, { "Coke", 5 }, { "butter", 20 } };
    s . sell ( l1 );
    assert ( l1 . size () == 2 );
    assert ( ( l1 == list<pair<string,int> > { { "Coke", 5 }, { "butter", 10 } } ) );

    list<pair<string,int> > l2 = s . expired ( CDate ( 2016, 4, 30 ) );
    assert ( l2 . size () == 1 );
    assert ( ( l2 == list<pair<string,int> > { { "bread", 98 } } ) );

    list<pair<string,int> > l3 = s . expired ( CDate ( 2016, 5, 20 ) );
    assert ( l3 . size () == 1 );
    assert ( ( l3 == list<pair<string,int> > { { "bread", 198 } } ) );

    list<pair<string,int> > l4 { { "bread", 105 } };
    s . sell ( l4 );
    assert ( l4 . size () == 0 );
    assert ( ( l4 == list<pair<string,int> > {  } ) );

    list<pair<string,int> > l5 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l5 . size () == 3 );
    assert ( ( l5 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 } } ) );

    s . store ( "Coke", CDate ( 2016, 12, 31 ), 10 );

    /*list<pair<string,int> > l6 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 }, { "Cokes", 1 } };
    s . sell ( l6 );
    assert ( l6 . size () == 3 );
    assert ( ( l6 == list<pair<string,int> > { { "cake", 1 }, { "cuke", 1 }, { "Cokes", 1 } } ) );

    list<pair<string,int> > l7 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l7 . size () == 4 );
    assert ( ( l7 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 7 }, { "okey", 5 } } ) );

    s . store ( "cake", CDate ( 2016, 11, 1 ), 5 );

    list<pair<string,int> > l8 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 } };
    s . sell ( l8 );
    assert ( l8 . size () == 2 );
    assert ( ( l8 == list<pair<string,int> > { { "Cake", 1 }, { "coke", 1 } } ) );

    list<pair<string,int> > l9 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l9 . size () == 5 );
    assert ( ( l9 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 }, { "cake", 3 } } ) );

    list<pair<string,int> > l10 { { "cake", 15 }, { "Cake", 2 } };
    s . sell ( l10 );
    assert ( l10 . size () == 2 );
    assert ( ( l10 == list<pair<string,int> > { { "cake", 12 }, { "Cake", 2 } } ) );

    list<pair<string,int> > l11 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l11 . size () == 4 );
    assert ( ( l11 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 } } ) );

    list<pair<string,int> > l12 { { "Cake", 4 } };
    s . sell ( l12 );
    assert ( l12 . size () == 0 );
    assert ( ( l12 == list<pair<string,int> > {  } ) );

    list<pair<string,int> > l13 = s . expired ( CDate ( 2017, 1, 1 ) );
    assert ( l13 . size () == 4 );
    assert ( ( l13 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 }, { "Coke", 2 } } ) );

    list<pair<string,int> > l14 { { "Beer", 20 }, { "Coke", 1 }, { "bear", 25 }, { "beer", 10 } };
    s . sell ( l14 );
    assert ( l14 . size () == 1 );
    assert ( ( l14 == list<pair<string,int> > { { "beer", 5 } } ) );

    s . store ( "ccccb", CDate ( 2019, 3, 11 ), 100 )
            . store ( "ccccd", CDate ( 2019, 6, 9 ), 100 )
            . store ( "dcccc", CDate ( 2019, 2, 14 ), 100 );

    list<pair<string,int> > l15 { { "ccccc", 10 } };
    s . sell ( l15 );
    assert ( l15 . size () == 1 );
    assert ( ( l15 == list<pair<string,int> > { { "ccccc", 10 } } ) );*/

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */


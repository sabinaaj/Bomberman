#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <memory>
#include <functional>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

class Tree{
public:
    int Value = 0;
    Tree * Right = nullptr;
    Tree * Left = nullptr;
    Tree (int value, Tree * right, Tree * left){
        Value = value;
        Right = right;
        Left = left;
    }
};

class Huff {
    const char * m_inFile;
    const char * m_outFile;
    int m_bitPointer = 0;
    int m_catche[8];
    int m_point = 0;
    int m_bitmask = 0;
    int m_num = 0;
    char m_char;
    bool m_chunk = true;
    bool m_fail = false;
    bool m_fileOpenRead = false;
    bool m_fileOpenWrite = false;
    ifstream m_in;
    ofstream m_out;
    Tree * m_root;

public:
    inline bool 	is_fail	(void) {return m_fail;}
    inline bool     is_chunk (void) {return m_chunk;}

    Huff ( const char * inFile, const char * outFile ){
        m_inFile = inFile;
        m_outFile = outFile;
    }


/* ------------------------------------------------------------------------------*/
    bool openFileRead (void){
        m_in.open( m_inFile, ios::binary);
        if (!m_in  || m_in.fail() ) return false;
        if(!m_in.is_open()) return false;
        m_fileOpenRead=true;
        return true;
    }

    bool openFileWrite (void){
        m_out.open ( (const char *) m_outFile);
        if (!m_out) return false;
        if(!m_out.is_open()) return false;
        if (m_out.fail()) return false;
        m_fileOpenWrite=true;
        return true;
    }
/* ------------------------------------------------------------------------------*/
    bool getBit (bool & bit){
        if(!m_fileOpenRead)
        {
            if(!openFileRead()) return false;
        }
        if(m_bitPointer==8) m_bitPointer=0;
        if(m_bitPointer==0)
        {
            if(!readBajt()) return false;
        }
        bit = m_catche[m_bitPointer++];
        return true;
    }

    bool readBajt (void){
        m_in.get(m_char);
        if(!m_in.good()) return false;
        for (int i=0;i<8;i++) {
            m_catche[m_point++] = (bool)((m_char >> (7 - m_bitmask++)) & 0x1);
        }
        m_point=0;
        m_bitmask=0;
        return true;
    }
/* ------------------------------------------------------------------------------*/
    bool startMakeTree (void){
        bool bit;
        if (!getBit(bit)) return false;
        if (bit) return false;
        else m_root = new Tree (0, makeTree(), makeTree());
        return true;
    }

    Tree * makeTree (void){
        bool bit;
        Tree * root;
        int value = 0;
        if (!getBit(bit)) {
            m_fail = true;
            return nullptr;
        }
        if (bit){
            if (!scanUTF8(value)) {
                m_fail = true;
                return nullptr;
            }
            root = new Tree (value, nullptr, nullptr);
        }
        else {
            root = new Tree (0, makeTree(), makeTree());
        }
        return root;
    }

    bool startControlTree (void){
        m_root = controlTree (m_root);
        if (m_root) return true;
        return false;
    }

    Tree * controlTree (Tree * root)
    {
        if (root->Left) root->Left = controlTree(root->Left);
        if (root->Right) root->Right = controlTree(root->Right);
        if ((!root-> Left) != (!root->Right)) return nullptr;
        return root;
    }
/* ------------------------------------------------------------------------------*/

bool readUTF8 ( int & value, int type){
    bool bit;
    value = 0;
        switch (type) {
            case 1:
                for(int i=0; i<7; i++){
                    if(!getBit(bit)) return false;
                    value |= (1 << (6-i)) * bit;
                }
                break;
            case 2:
                value |= (1 << (15)) * 1;
                value |= (1 << (14)) * 1;
                value |= (1 << (13)) * 0;
                for(int i=3; i<type*8; i++){
                    if(!getBit(bit)) return false;
                    if(!i%8) if(bit!=1) return false;
                    if(!((i%8)-1)) if(bit!=0) return false;
                    value |= (1 << (type*8-1-i)) * bit;
                }
                break;
            case 3:
                value |= (1 << (23)) * 1;
                value |= (1 << (22)) * 1;
                value |= (1 << (21)) * 1;
                value |= (1 << (20)) * 0;
                for(int i=4; i<type*8; i++){
                    if(!getBit(bit)) return false;
                    if(!i%8) if(bit!=1) return false;
                    if(!((i%8)-1)) if(bit!=0) return false;
                    value |= (1 << (type*8-1-i)) * bit;
                }
                break;
            case 4:
                value |= (1 << (31)) * 1;
                value |= (1 << (30)) * 1;
                value |= (1 << (29)) * 1;
                value |= (1 << (28)) * 1;
                value |= (1 << (27)) * 0;
                for(int i=5; i<type*8; i++){
                    if(!getBit(bit)) return false;
                    if(!i%8) if(bit!=1) return false;
                    if(!((i%8)-1)) if(bit!=0) return false;
                    value |= (1 << (type*8-1-i)) * bit;
                }
                break;
        }
        return true;
}

bool scanUTF8 (int & value){
    unsigned int typeUTF8=1;
    unsigned int i=0;
    bool bit;
    while(true){
        if(typeUTF8>4) return false;
        if(!getBit(bit)) return false;
        if((!bit)&&(i!=1)) break;
        if((!bit)&&(i==1)) return false;
        if((bit)&&(i!=1)) typeUTF8++;
        i++;
    }
    if(!readUTF8(value,typeUTF8)) return false;
    return true;
}

bool  writeUTF8 (int value){
    char x;
    for (int i = 3; i > -1; i--){
        x = (value >> 8*i);
        //cout << x << endl;
        if(x) if(!writeToFile(x)) return false;
    }
    return true;
}
/* ------------------------------------------------------------------------------*/
bool decode (void){
    bool bit;
    while(m_num != 0){
        Tree * root = m_root;
        if (root == nullptr) return false;
        while(!root->Value){
            if (!getBit(bit)) return false;
            if (bit) root = root->Left;
            else root = root->Right;
            if (root == nullptr) return false;
        }
        //cout << root->Value << endl;
        if(!writeUTF8(root->Value)) {return false; }
        m_num --;
    }
    return true;
}

bool chunk (void){
    bool bit;
    if (!getBit(bit)) return false;
    if (bit) {
        m_chunk = true;
        m_num = 4096;
        return true;
    }

    for (int i = 11; i > -1; i--){
        if (!getBit(bit)) return false;

        if (bit && i == 0) {m_num += 1; break;}
        if (bit) {
            int result = 1;
            for (int j = i; j > 0; j--){
               result *= 2;
            }
            m_num += result;
        }

    }
    m_chunk = false;
    return true;
}

bool writeToFile (char value){
    if (! m_fileOpenWrite){
        if(!openFileWrite()) return false;
    }

    if (m_out.good()){
        m_out << value;
        return true;
    }
    return false;
}

void close (void){
    if(m_fileOpenWrite) m_out.close();
    if(m_fileOpenRead) m_in.close();
    //delete(m_root);
}

};

bool decompressFile ( const char * inFileName, const char * outFileName ){
    Huff x = Huff(inFileName,outFileName);
    if (!x.startMakeTree()) return false;
    if (!x.startControlTree()) return false;
    if (x.is_fail()) return false;
    while(x.is_chunk()){
        if (!x.chunk()) return false;
        if (!x.decode()) return false;
    }
    x.close();
    return true;
}

bool compressFile ( const char * inFileName, const char * outFileName )
{
  // keep this dummy implementation (no bonus) or implement the compression (bonus)
  return false;
}
#ifndef __PROGTEST__
bool identicalFiles ( const char * fileName1, const char * fileName2 )
{
  string command = "diff " + (string)fileName1 + " " + (string)fileName2;
  if(!system(command.c_str())) return true;
  return false;
}

int main ( void )
{
  /*assert ( decompressFile ( "tests/test0.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test0.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test1.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test1.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test2.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test2.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test3.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test3.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test4.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test4.orig", "tempfile" ) );

  assert ( ! decompressFile ( "tests/test5.huf", "tempfile" ) );*/

  decompressFile("input.bin", "tempfile");

 /* assert ( decompressFile ( "tests/extra0.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra0.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra1.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra1.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra2.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra2.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra3.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra3.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra4.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra4.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra5.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra5.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra6.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra6.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra7.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra7.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra8.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra8.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra9.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra9.orig", "tempfile" ) );*/

  return 0;
}
#endif /* __PROGTEST__ */

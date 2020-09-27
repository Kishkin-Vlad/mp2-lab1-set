// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include <cmath>

#include "tbitfield.h"


TBitField::TBitField(int len)
{
    if (len > -1)
    {
        BitLen = len;
        int x = log(sizeof(TELEM) * 8) / log(2); // 2^x=32 -> log(32)/log(2)
        MemLen = (len + sizeof(TELEM) * 8 - 1) >> x;

        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++)
            pMem[i] = 0;
    } else {
        throw std::logic_error("len arr <= -1");
    }

}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    MemLen = bf.MemLen;
    BitLen = bf.BitLen;

    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}


TBitField::~TBitField()
{
    delete pMem;
    pMem = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    int size = sizeof(TELEM) * 8;
    int a = n / size;
    int b = n % size;

    if (a != 0) 
        return b + 1;
    else 
        return b;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0) throw std::logic_error("GetMemMask(n) with n < 0");
    if (n >= BitLen) throw std::logic_error("GetMemMask(n) with n >= BitLen");

    return pow(2, n);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0) throw std::logic_error("SetBit(n) with n < 0");
    if (n >= BitLen) throw std::logic_error("SetBit(n) with n >= BitLen");

    int size = sizeof(TELEM) * 8;
    int ind = n / size;
    int shift = n % size;

    pMem[ind] = pMem[ind] | (1 << shift);    
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0) throw std::logic_error("ClrBit(n) with n < 0");
    if (n >= BitLen) throw std::logic_error("ClrBit(n) with n >= BitLen");

    int size = sizeof(TELEM) * 8;
    int ind = n / size;
    int shift = n % size;

    pMem[ind] = pMem[ind] & ~(1 << shift);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0) throw std::logic_error("GetBit(n) with n < 0");
    if (n >= BitLen) throw std::logic_error("GetBit(n) with n >= BitLen");

    int size = sizeof(TELEM) * 8;
    int ind = n / size;
    int shift = n % size;

    return ((pMem[ind] & (1 << shift)) != 0);
}


// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (BitLen == bf.BitLen)
    {
        for (int i = 0; i < MemLen; i++)
            pMem[i] = bf.pMem[i];
    } else {
        MemLen = bf.MemLen;
        BitLen = bf.BitLen;

        delete pMem;
        pMem = new TELEM[MemLen];

        for (int i = 0; i < MemLen; i++)
            pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return 0;
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 1;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return 1;
    return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    if (BitLen < bf.BitLen)
    {
        TBitField temp(bf.BitLen);
        for (int i = 0; i < bf.MemLen; i++)
            temp.pMem[i] |= bf.pMem[i];
        for (int i = 0; i < MemLen; i++)
            temp.pMem[i] |= pMem[i];

        return temp;
    } else {
        TBitField temp(BitLen);
        for (int i = 0; i < MemLen; i++)
            temp.pMem[i] |= pMem[i];
        for (int i = 0; i < bf.MemLen; i++)
            temp.pMem[i] |= bf.pMem[i];

        return temp;
    }
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    if (BitLen < bf.BitLen)
    {
        TBitField temp(bf.BitLen);
        for (int i = 0; i < bf.MemLen; i++)
            temp.pMem[i] |= bf.pMem[i];
        for (int i = 0; i < MemLen; i++)
            temp.pMem[i] &= pMem[i];

        return temp;
    }
    else {
        TBitField temp(BitLen);
        for (int i = 0; i < MemLen; i++)
            temp.pMem[i] |= pMem[i];
        for (int i = 0; i < bf.MemLen; i++)
            temp.pMem[i] &= bf.pMem[i];

        return temp;
    }
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField temp(BitLen);
    for (int i = 0; i < BitLen; i++)
    {
        int size = sizeof(TELEM) * 8;
        int ind = i / size;
        int shift = i % size;

        if ((pMem[ind] & (1 << shift)) == 0)
            temp.pMem[ind] |= 1 << shift;
        else
            temp.pMem[ind] &= ~(1 << shift);
    }
    
    return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int ind = 0;
    char string;

    for (int i = 0; i < bf.GetLength(); i++)
    {
        istr >> string;
        if (string == '1')
            bf.SetBit(i);
        ind++;
    }

    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    ostr << bf.GetBit(0);
    for (int i = 1; i < bf.GetLength(); i++)
        if (bf.GetBit(i))
            ostr << 1;
        else
            ostr << 0;

    return ostr;
}

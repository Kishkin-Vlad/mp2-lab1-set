// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

using namespace std;


TSet::TSet(int mp) : MaxPower(mp), BitField(mp) {}

// конструктор копирования
TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField) {}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : MaxPower(bf.GetLength()), BitField(bf) {}

TSet::operator TBitField()
{
    TBitField bf(BitField);

    return bf;
}

int TSet::GetMaxPower() const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    MaxPower = s.GetMaxPower();
    BitField = s.BitField;

    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    return (BitField == s.BitField);
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return (BitField != s.BitField);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TBitField bf(BitField | s.BitField);

    return bf;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem > MaxPower)
    {
        TSet bf(Elem);
        bf.InsElem(Elem);
        TSet merge(BitField | bf.BitField);

        return merge;
    } else {
        TSet bf(*this);
        bf.InsElem(Elem);

        return bf;
    }
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if (Elem > MaxPower)
        throw std::logic_error("operator-(Elem) with Elem > MaxPower");
    else {
        TSet bf(*this);
        bf.DelElem(Elem);

        return bf;
    }
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TSet bf(BitField & s.BitField);

    return bf;
}

TSet TSet::operator~(void) // дополнение
{
    TSet bf(~BitField);

    return bf;
}

void TSet::finding_the_divisor(int number)
{
    TSet dividers(MaxPower);
    for (int i = number; i < MaxPower; i += number)
        if (IsMember(i))
            dividers.InsElem(i);

    cout << "dividers: " << dividers;
}

// перегрузка ввода/вывода
istream &operator>>(istream &istr, TSet &s) // ввод
{
    int integ;
    char ch;

    istr >> ch;
    while (ch != '}')
    {
        istr >> integ;
        s.InsElem(integ);
        istr >> ch;
    }

    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    int len = s.GetMaxPower();
    int ind = 0, check_len = 0;

    ostr << "{";
    while (check_len < len)
    {
        if (s.IsMember(ind) == 1)
        {
            ostr << ind;
            break;
        }
        ind++;
        check_len++;
    }
    for (int i = check_len + 1; i < len; i++)
        if (s.IsMember(i))
            ostr << "," << i;
    ostr << "}";

    return ostr;
}

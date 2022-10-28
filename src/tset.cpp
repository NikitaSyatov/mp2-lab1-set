// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp)
{
    this->MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField)
{
    this->MaxPower = s.MaxPower;
    this->BitField = s.BitField;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf)
{
    this->MaxPower = bf.GetLength();
    this->BitField = bf;
}

TSet::operator TBitField()
{
    return TBitField(BitField);
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return this->MaxPower;
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
    MaxPower = s.MaxPower;
    BitField = s.BitField;

    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    return ((MaxPower == s.MaxPower) && (BitField == s.BitField));
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return ((MaxPower != s.MaxPower) || (BitField != s.BitField));
}

TSet TSet::operator+(const TSet &s) // объединение
{
    int resPower = MaxPower;
    if (MaxPower < s.MaxPower)
        resPower = s.MaxPower;
    TSet res(resPower);

    res.BitField = BitField | s.BitField;

    return res;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet tmp(*this);
    tmp.BitField.SetBit(Elem);

    return tmp;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet tmp(*this);
    tmp.BitField.ClrBit(Elem);

    return tmp;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    int resPower = MaxPower;
    if (MaxPower < s.MaxPower)
        resPower = s.MaxPower;
    TSet res(resPower);

    res.BitField = BitField & s.BitField;

    return res;
}

TSet TSet::operator~(void) // дополнение
{
    return ~BitField;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    unsigned int number;
    int index = s.GetMaxPower() - 1;
    while (index)
    {
        cout << "Enter Number:";
        istr >> number;
        if (number < s.GetMaxPower() && (s.IsMember(number) == 0))
        {
            s.InsElem(number);
            index--;
        }
        cout << "error! try again!";
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    for (size_t i = 0; i < s.GetMaxPower(); i++)
        if (s.IsMember(i))
            ostr << i << ", ";
    return ostr;
}

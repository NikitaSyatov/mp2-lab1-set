// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <= 0)
		throw exception("err");
	else
	{
		this->BitLen = len;
		this->MemLen = (len / (sizeof(TELEM) * 8)) + 1;
		this->pMem = new TELEM[MemLen];
		for (size_t i = 0; i < MemLen; i++)
			this->pMem[i] = 0;
	}
	
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	this->BitLen = bf.BitLen;
	this->MemLen = bf.MemLen;
	this->pMem = new TELEM[MemLen];
	for (size_t i = 0; i < MemLen; i++)
		this->pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return (1 << (n & (sizeof(TELEM) * 8) - 1));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n >= 0 && n < this->BitLen)
	{
		pMem[this->GetMemIndex(n)] |= GetMemMask(n);
	}
	else
		throw exception("err");
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n >= 0 && n < this->BitLen)
		pMem[this->GetMemIndex(n)] &= ~GetMemMask(n);
	else
		throw exception("err");
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n >= 0 && n < this->BitLen)
	{
		int res = this->pMem[GetMemIndex(n)];
		res &= GetMemMask(n);

		return res;
	}
	else
		throw exception("err");
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	this->BitLen = bf.BitLen;
	if (this->MemLen != bf.MemLen)
	{
		delete[] pMem;
		this->MemLen = bf.MemLen;
		this->pMem = new TELEM[MemLen];
	}
	for (size_t i = 0; i < this->MemLen; i++)
		this->pMem[i] = bf.pMem[i];

	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (this->BitLen == bf.BitLen)
	{
		for (size_t i = 0; i < this->MemLen; i++)
			if (this->pMem[i] != bf.pMem[i])
				return 0;
		return 1;
	}

	return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (this->BitLen == bf.BitLen)
	{
		for (size_t i = 0; i < this->MemLen; i++)
			if (this->pMem[i] != bf.pMem[i])
				return 1;
		return 0;
	}

	return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int maxBitLen = BitLen;

	if (BitLen < bf.BitLen)
		maxBitLen = bf.BitLen;

	size_t i = 0;
	TBitField tmp(maxBitLen);
	for (; i < std::min(MemLen, bf.MemLen); i++)
		tmp.pMem[i] = pMem[i] | bf.pMem[i];
	
	if (MemLen < bf.MemLen)
		for (; i < std::max(MemLen, bf.MemLen); i++)
			tmp.pMem[i] = bf.pMem[i];
	else
		for (; i < std::max(MemLen, bf.MemLen); i++)
			tmp.pMem[i] = this->pMem[i];
		
		
	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int minMemLen = MemLen;
	int maxBitLen = BitLen;

	if (bf.BitLen < this->MemLen)
		minMemLen = bf.MemLen;

	if (this->BitLen < bf.BitLen)
		maxBitLen = bf.BitLen;

	TBitField tmp(maxBitLen);
	for (size_t i = 0; i < minMemLen; i++)
		tmp.pMem[i] = pMem[i] & bf.pMem[i];

	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(BitLen);
	for (size_t i = 0; i < MemLen; i++)
	{
		tmp.pMem[i] = ~pMem[i];
	}
	tmp.pMem[MemLen - 1] &= GetMemMask(BitLen) - 1;

	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	unsigned int number;
	int index = bf.GetLength() - 1;
	while (index)
	{
		istr >> number;
		if (number < bf.GetLength() && (bf.GetBit(number) == 1))
		{
			bf.SetBit(number);
			index--;
		}
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (size_t i = 0; i < bf.GetLength(); i++)
		if (bf.GetBit(i))
			ostr << i << ", ";
	return ostr;
}

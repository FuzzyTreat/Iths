#include <iostream>
#include "lektion1.h"

void Lektion1::Speak()
{
    std::cout << "Hello World!" << std::endl;
}

void Lektion1::SetValue(int value)
{
    m_internalValue = value;
}

int Lektion1::GetValue()
{
    return m_internalValue;
}
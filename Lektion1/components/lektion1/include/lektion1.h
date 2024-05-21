#ifndef LEKTION1_H
#define LEKTION1_H

class Lektion1
{
    public:
        void Speak();
        void SetValue(int value);
        int GetValue();

    private:
        int m_internalValue;
};

#endif
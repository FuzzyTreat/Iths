#ifndef VALUESELECTOR_H
#define VALUESELECTOR_H

typedef enum class SelectedValue_e : int16_t
{
    Acceleration_X = 0,
    Acceleration_Y = 1,
    Acceleration_Z = 2,
    Rotation_X = 3,
    Rotation_Y = 4,
    Rotation_Z = 5
}SelectedValue_e;

typedef enum
{
    None,
    MoveUp,
    MoveDown
} ChangeDirection_e;

class ValueSelector
{
    typedef void (*onSelectedValueChange_cb)(void *);

    private:
        ChangeDirection_e direction;
        SelectedValue_e currentValue;
        onSelectedValueChange_cb onSelectedValueChanged = nullptr;
        void* onSelectedValueChanged_ptr = nullptr;
        void Update();

    protected:

    public:
        ValueSelector(SelectedValue_e selected);
        ValueSelector(SelectedValue_e selected, ChangeDirection_e direction);

        void SetOnChanged(onSelectedValueChange_cb onSelectedValueChanged, void *ptr);
        SelectedValue_e GetCurrentValue();
        void SetDirection(ChangeDirection_e direction);
        ChangeDirection_e GetDirection();
};

// Overload the SelectedValue_e++ operator
inline SelectedValue_e& operator++(SelectedValue_e& eValue, int)  // <--- note -- must be a reference
{
	const int i = static_cast<int>(eValue)+1;
    int index = i;
    
    if(index >= 6)
    {
        index = 0;
    }

	eValue = static_cast<SelectedValue_e>((index) % 6);
	return eValue;
}

// Overload the SelectedValue_e-- operator
inline SelectedValue_e& operator--(SelectedValue_e& type, int)  // <--- note -- must be a reference
{
	const int i = static_cast<int>(type)-1;
	
	if (i < 0) // Check whether to cycle to last item if number goes below 0
	{
		type = static_cast<SelectedValue_e>(5);
	}
	else // Else set it to current number -1
	{
		type = static_cast<SelectedValue_e>((i) % 6);
	}
	return type;
}

#endif
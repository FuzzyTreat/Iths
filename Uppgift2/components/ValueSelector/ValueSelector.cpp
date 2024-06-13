#include <stdio.h>
#include <esp_log.h>
#include "ValueSelector.h"

static const char *TAG = "ValueSelector";

ValueSelector::ValueSelector(SelectedValue_e selected): direction(None), currentValue(selected){}

ValueSelector::ValueSelector(SelectedValue_e selected, ChangeDirection_e direction): direction(direction), currentValue(selected){}

void ValueSelector::SetOnChanged(onSelectedValueChange_cb selectedValueChanged, void *ptr)
{
    this->onSelectedValueChanged = selectedValueChanged;
    this->onSelectedValueChanged_ptr = ptr;
};

SelectedValue_e ValueSelector::GetCurrentValue()
{
    return this->currentValue;
};

void ValueSelector::SetDirection(ChangeDirection_e direction)
{
    if(direction == MoveUp)
    {
        this->currentValue = this->currentValue++;
    }
    else
    {
        this->currentValue = this->currentValue--;
    }

    this->direction = direction;
    
    Update();
};

ChangeDirection_e ValueSelector::GetDirection()
{
    return this->direction;
};

void ValueSelector::Update()
{
    if(onSelectedValueChanged && direction != None)
    {
        onSelectedValueChanged(onSelectedValueChanged_ptr);
    }
}


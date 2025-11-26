#include "UI/FillButtonWidget.h"
#include "Components/Button.h"

void UFillButtonWidget::SetButtonEnabled(bool flag)
{
	Button->SetIsEnabled(flag);
}
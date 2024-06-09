#pragma once
#include <iostream>
#include "Model/Message.h"
namespace Piero {

void MyMessageElement(std::string message);
void ModalLoginForm(const char* id);

void MessageElement(Message message);


bool ToggleButton(const char* str_id, int* v);
}



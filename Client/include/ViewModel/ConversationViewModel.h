#pragma once

namespace Piero {

class ConversationViewModel
{
private:
    ConversationViewModel() {}
public:
    ~ConversationViewModel() = default;
private:
    static ConversationViewModel* s_Instance;

};





}
/* Local */
#include "Log.h"

void model::Log::Add(const std::string& message)
{
    m_messages.push_back(message);
}

void model::Log::Clear()
{
    m_messages.clear();
}

std::vector<std::string>& model::Log::GetMessages()
{
    return m_messages;
}

// Inicialization of static variables
std::vector<std::string> model::Log::m_messages;

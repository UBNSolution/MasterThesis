#pragma once

/* System */
#include <vector>
#include <string>

namespace model
{

class Log
{
public:
    static void Add(const std::string& message);
    static void Clear();
    static std::vector<std::string>& GetMessages();
private:
    static std::vector<std::string> m_messages;
};

}

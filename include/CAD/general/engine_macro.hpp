#define PUSH_MSG1(col, str)  {                                  \
    std::vector<std::pair<Engine::Color, std::string>> msg{     \
        {Engine::col,str},                                      \
    };                                                          \
    this->console.push_back(msg);                               \
}

#define PUSH_MSG2(col, str1, str2)  {                           \
    std::vector<std::pair<Engine::Color, std::string>> msg{     \
        {Engine::col[0],str1},                                  \
        {Engine::col[1],str2},                                  \
    };                                                          \
    this->console.push_back(msg);                               \
}
;
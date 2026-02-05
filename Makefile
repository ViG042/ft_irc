NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
DEPFLAGS = -MMD

SRCS = main.cpp Server.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@
	
all: $(NAME)

clean:
	rm -rf $(OBJS) $(DEPS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

-PHONY: all clean fclean re

-include $(DEPS)

Content-Disposition: form-data; name="file"; filename="Makefile"
Content-Type: application/octet-stream

CXX=g++
CXXFLAGS=-Wall -Wextra -Werror
SENDER=sender
RECEIVER=receiver
SRC_DIR=./src
OBJ_DIR=./object
RM=rm -rf

build: $(SENDER) $(RECEIVER)

S_SRC_DIR=$(SRC_DIR)/$(SENDER)
S_OBJ_DIR=$(OBJ_DIR)/$(SENDER)
S_SRCS=$(wildcard $(S_SRC_DIR)/*.cc)
S_OBJS=$(subst $(S_SRC_DIR),$(S_OBJ_DIR),$(S_SRCS:.cc=.o))

$(S_OBJ_DIR)/%.o: $(S_SRC_DIR)/%.cc
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SENDER): $(S_OBJS)
	$(CXX) $(CXXFLAGS) $(S_OBJS) -o $(S_OBJ_DIR)/$@

R_SRC_DIR=$(SRC_DIR)/$(RECEIVER)
R_OBJ_DIR=$(OBJ_DIR)/$(RECEIVER)
R_SRCS=$(wildcard $(R_SRC_DIR)/*.cc)
R_OBJS=$(subst $(R_SRC_DIR),$(R_OBJ_DIR),$(R_SRCS:.cc=.o))

$(R_OBJ_DIR)/%.o: $(R_SRC_DIR)/%.cc
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(RECEIVER): $(R_OBJS)
	$(CXX) $(CXXFLAGS) $(R_OBJS) -o $(R_OBJ_DIR)/$@

run: build
	$(S_OBJ_DIR)/$(SENDER) & $(R_OBJ_DIR)/$(RECEIVER) & wait

clean:
	$(RM) $(S_OBJS) $(R_OBJS)

fclean: clean
	$(RM) $(OBJ_DIR)

re: fclean all

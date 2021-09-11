TARGET_EXEC := canlib-tests

BUILD_DIR := ./build
SRCS_DIR := ../src/src tests/src

COMP_SRC := ../src/src/computers/*.cpp

# String substitution for every C/C++ file.
# As an example, hello.cpp turns into ./build/hello.cpp.o

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
#DEPS := $(OBJS:.o=.d)

# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := -I../src/inc -Itests/inc
COMPUTER_FLAGS := -I../src/inc/drivers -I../src/inc/computers

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS) -std=c++17

# The final build step.
gen:
	python3 ../generator/main.py can_spec.yml --testing
	
main0: gen
	g++ $(CPPFLAGS) $(COMPUTER_FLAGS) $(CXXFLAGS) -DCANLIB_ARCH_TESTFAMILY -DARCH0 ../src/src/bus.cpp ../src/src/structs.cpp ../src/src/drivers/testfamily.cpp ../src/src/pack_unpack.cpp ../src/src/computers/canlib_testcomp0.cpp tests/src/identify.cpp tests/src/message.cpp tests/src/send.cpp tests/src/constants.cpp tests/src/arch0.cpp tests/main.cpp -o main0
main1: gen
	g++ $(CPPFLAGS) $(COMPUTER_FLAGS) $(CXXFLAGS) -DCANLIB_ARCH_TESTFAMILY -DARCH1 ../src/src/bus.cpp ../src/src/structs.cpp ../src/src/drivers/testfamily.cpp ../src/src/pack_unpack.cpp ../src/src/computers/canlib_testcomp1.cpp tests/src/identify.cpp tests/src/message.cpp tests/src/send.cpp tests/src/constants.cpp tests/src/arch1.cpp tests/main.cpp -o main1
main2: gen
	g++ $(CPPFLAGS) $(COMPUTER_FLAGS) $(CXXFLAGS) -DCANLIB_ARCH_TESTFAMILY -DARCH2 ../src/src/bus.cpp ../src/src/structs.cpp ../src/src/drivers/testfamily.cpp ../src/src/pack_unpack.cpp ../src/src/computers/canlib_testcomp2.cpp tests/src/identify.cpp tests/src/message.cpp tests/src/send.cpp tests/src/constants.cpp tests/src/arch2.cpp tests/main.cpp -o main2

$(BUILD_DIR):
	mkdir -p build/src/src
	mkdir -p build/src/drivers
	mkdir -p build/src/src/computers
	mkdir -p build/tests/src

# Build step for C++ source
$(BUILD_DIR)/src/src/bus.cpp.o: $(BUILD_DIR)
	g++ ../src/src/bus.cpp $(CPPFLAGS) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/src/src/bus.cpp.o

$(BUILD_DIR)/src/src/structs.cpp.o: $(BUILD_DIR)
	g++ ../src/src/structs.cpp $(CPPFLAGS) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/src/src/structs.cpp.o

$(BUILD_DIR)/src/src/pack_unpack.cpp.o: $(BUILD_DIR)
	g++ ../src/src/pack_unpack.cpp $(CPPFLAGS) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/src/src/pack_unpack.cpp.o

$(BUILD_DIR)/tests/src/send.cpp.o: $(BUILD_DIR)
	g++ tests/src/send.cpp $(CPPFLAGS) $(COMPUTER_FLAGS) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/tests/src/send.cpp.o

$(BUILD_DIR)/tests/src/constants.cpp.o: $(BUILD_DIR)
	g++ tests/src/constants.cpp $(CPPFLAGS) $(COMPUTER_FLAGS) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/tests/src/constants.cpp.o

$(BUILD_DIR)/tests/src/identify.cpp.o: $(BUILD_DIR)
	g++ tests/src/identify.cpp $(CPPFLAGS) $(COMPUTER_FLAGS) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/tests/src/identify.cpp.o

$(BUILD_DIR)/tests/src/arch0.cpp.o: $(BUILD_DIR)
	g++ tests/src/arch0.cpp $(CPPFLAGS) $(COMPUTER_FLAGS) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/tests/src/arch0.cpp.o

$(BUILD_DIR)/tests/src/arch1.cpp.o: $(BUILD_DIR)
	g++ tests/src/arch1.cpp $(CPPFLAGS) $(COMPUTER_FLAGS) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/tests/src/arch1.cpp.o

$(BUILD_DIR)/tests/src/arch2.cpp.o: $(BUILD_DIR)
	g++ tests/src/arch2.cpp $(CPPFLAGS) $(COMPUTER_FLAGS) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/tests/src/arch2.cpp.o

$(BUILD_DIR)/tests/src/message.cpp.o: $(BUILD_DIR)
	g++ tests/src/message.cpp $(CPPFLAGS) $(COMPUTER_FLAGS) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/tests/src/message.cpp.o

$(BUILD_DIR)/src/src/drivers/testfamily.cpp.o: $(BUILD_DIR)
	g++ ../src/src/drivers/testfamily.cpp $(CPPFLAGS) $(COMPUTER_FLAGS) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/src/src/drivers/testfamily.cpp.o

$(BUILD_DIR)/tests/main.cpp.o: $(BUILD_DIR)
	g++ tests/main.cpp $(CPPFLAGS) $(COMPUTER_FLAGS) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/./tests/main.cpp.o

$(BUILD_DIR)/src/src/computers/canlib_testcomp0.cpp.o: $(BUILD_DIR)
	g++ ../src/src/computers/canlib_testcomp0.cpp $(CPPFLAGS) $(COMPUTER_FLAGS) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/src/src/computers/canlib_testcomp0.cpp.o

$(BUILD_DIR)/src/src/computers/canlib_testcomp1.cpp.o: $(BUILD_DIR)
	g++ ../src/src/computers/canlib_testcomp1.cpp $(CPPFLAGS) $(COMPUTER_FLAGS) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/src/src/computers/canlib_testcomp1.cpp.o

$(BUILD_DIR)/src/src/computers/canlib_testcomp2.cpp.o: $(BUILD_DIR)
	g++ ../src/src/computers/canlib_testcomp2.cpp $(CPPFLAGS) $(COMPUTER_FLAGS) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/src/src/computers/canlib_testcomp2.cpp.o

.PHONY: clean build
clean:
	python3 ../generator/main.py --clean
	rm -r build/
	rm main0 main1 main2
build: main0 main1 main2
run: 
	./main0 & ./main1 & ./main2

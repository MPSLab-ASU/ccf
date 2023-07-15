# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CGRA.cpp \
../src/Mapper.cpp \
../src/Parser.cpp \
../src/ARC.cpp \
../src/DFG.cpp \
../src/main.cpp \
../src/Node.cpp \
../src/RegisterFile.cpp \
../src/SchedulingInfo.cpp  \
../src/INTERITERATIONDEPENDENCY.cpp \
../src/PriorityHeight.cpp

OBJS += \
./src/CGRA.o \
./src/Mapper.o \
./src/Parser.o \
./src/ARC.o \
./src/DFG.o \
./src/main.o \
./src/Node.o \
./src/RegisterFile.o \
./src/SchedulingInfo.o \
./src/INTERITERATIONDEPENDENCY.o \
./src/PriorityHeight.o

CPP_DEPS += \
./src/CGRA.d \
./src/Mapper.d \
./src/Parser.d \
./src/ARC.d \
./src/DFG.d \
./src/main.d \
./src/Node.d \
./src/RegisterFile.d \
./src/SchedulingInfo.d \
./src/INTERITERATIONDEPENDENCY.d \
./src/PriorityHeight.d


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++11 -g -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



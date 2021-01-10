set(
	SRC_FILES
	./main.cpp
	./lexer.hpp
	./interpreter.hpp
)
set(
	EXEC_NAME
	level_5
)
add_executable(
	${EXEC_NAME}
	${SRC_FILES}
)
add_compile_options(
	${EXEC_NAME}
	PRIVATE "-Wall" "-g"
)

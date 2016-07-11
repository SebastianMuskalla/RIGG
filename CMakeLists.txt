cmake_minimum_required(VERSION 3.5)
project(RIGG)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp common/NFA.cpp common/NFA.h common/Letter.cpp common/Letter.h common/Alphabet.cpp common/Alphabet.h common/Letter.cpp common/Letter.h common/NFA.cpp common/NFA.h common/GameGrammar.cpp common/GameGrammar.h common/Transition.cpp common/Transition.h dfa/Box.cpp dfa/Box.h dfa/Clause.cpp dfa/Clause.h dfa/Formula.cpp dfa/Formula.h common/Printable.h dfa/Ternary.h dfa/Solver.cpp dfa/Solver.h cachat/Determinizer.cpp cachat/Determinizer.h cachat/GamePDS.cpp cachat/GamePDS.h cachat/GrammarDFAtoPDSAFA.cpp cachat/GrammarDFAtoPDSAFA.h cachat/PDSTransition.cpp cachat/PDSTransition.h cachat/PAFA.cpp cachat/PAFA.h cachat/AFATransition.cpp cachat/AFATransition.h cachat/Cachat.cpp cachat/Cachat.h)
add_executable(RIGG ${SOURCE_FILES})

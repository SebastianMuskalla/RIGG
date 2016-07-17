cmake_minimum_required(VERSION 3.5)
project(RIGG)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static -static-libgcc -static-libstdc++")

set(SOURCE_FILES main.cpp common/NFA.cpp common/NFA.h common/Letter.cpp common/Letter.h common/Alphabet.cpp common/Alphabet.h common/Letter.cpp common/Letter.h common/NFA.cpp common/NFA.h common/GameGrammar.cpp common/GameGrammar.h common/Transition.cpp common/Transition.h dfa/Box.cpp dfa/Box.h dfa/Clause.cpp dfa/Clause.h dfa/Formula.cpp dfa/Formula.h common/Printable.h dfa/Ternary.h dfa/WorklistKleene.cpp dfa/WorklistKleene.h cachat/Determinizer.cpp cachat/Determinizer.h cachat/GamePDS.cpp cachat/GamePDS.h cachat/GrammarDFAtoPDSAFA.cpp cachat/GrammarDFAtoPDSAFA.h cachat/PDSTransition.cpp cachat/PDSTransition.h cachat/PAFA.cpp cachat/PAFA.h cachat/AFATransition.cpp cachat/AFATransition.h cachat/Cachat.cpp cachat/Cachat.h cachat/CachatWorklist.cpp cachat/CachatWorklist.h randomgen/AlphabetGen.cpp randomgen/AlphabetGen.h randomgen/TVAutomataGen.cpp randomgen/TVAutomataGen.h common/PointeeComparator.h randomgen/TVGrammarGen.cpp randomgen/TVGrammarGen.h dfa/NaiveKleene.cpp dfa/NaiveKleene.h cachat/Minimizer.cpp cachat/Minimizer.h dfa/FormulaStorage.h benchmark/Runnable.h benchmark/BaseThread.cpp benchmark/BaseThread.h benchmark/BenchmarkThread.cpp benchmark/BenchmarkThread.h benchmark/NaiveKleeneRunnable.h benchmark/Event.cpp benchmark/Event.h benchmark/WorklistKleeneRunnable.h benchmark/CachatRunnable.h)
add_executable(RIGG ${SOURCE_FILES})

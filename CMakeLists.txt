cmake_minimum_required(VERSION 3.18)
project(RIGG)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -static -static-libgcc -static-libstdc++")

set(SOURCE_FILES
        src/main.cpp
        src/Help.cpp
        src/Help.h
        src/Examples.cpp
        src/Examples.h
        src/Benchmarking.cpp
        src/Benchmarking.h
        src/common/NFA.cpp
        src/common/NFA.h
        src/common/Letter.cpp
        src/common/Letter.h
        src/common/Alphabet.cpp
        src/common/Alphabet.h
        src/common/Letter.cpp
        src/common/Letter.h
        src/common/NFA.cpp
        src/common/NFA.h
        src/common/GameGrammar.cpp
        src/common/GameGrammar.h
        src/common/Transition.cpp
        src/common/Transition.h
        src/common/PointeeComparator.h
        src/summaries/Box.cpp
        src/summaries/Box.h
        src/summaries/Clause.cpp
        src/summaries/Clause.h
        src/summaries/Formula.cpp
        src/summaries/Formula.h
        src/common/Printable.h
        src/summaries/Ternary.h
        src/summaries/SummarySolver.cpp
        src/summaries/SummarySolver.h
        src/summaries/FormulaStorage.h
        src/saturation/Determinizer.cpp
        src/saturation/Determinizer.h
        src/saturation/GamePDS.cpp
        src/saturation/GamePDS.h
        src/saturation/Cachatifier.cpp
        src/saturation/Cachatifier.h
        src/saturation/PDSTransition.cpp
        src/saturation/PDSTransition.h
        src/saturation/PAFA.cpp
        src/saturation/PAFA.h
        src/saturation/AFATransition.cpp
        src/saturation/AFATransition.h
        src/saturation/SaturationSolver.cpp
        src/saturation/SaturationSolver.h
        src/saturation/Minimizer.cpp
        src/saturation/Minimizer.h
        src/randomgen/AlphabetGen.cpp
        src/randomgen/AlphabetGen.h
        src/randomgen/TVAutomataGen.cpp
        src/randomgen/TVAutomataGen.h
        src/randomgen/TVLinearGrammarGen.cpp
        src/randomgen/TVLinearGrammarGen.h
        src/logging/Logger.h
        src/logging/NullLogger.h
        src/logging/CoutLogger.h
        src/logging/FilteredCoutLogger.h
        src/logging/IndentedLogger.h
        src/logging/QuietLogger.h)

add_executable(RIGG ${SOURCE_FILES})

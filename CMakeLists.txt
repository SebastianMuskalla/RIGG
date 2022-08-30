cmake_minimum_required(VERSION 3.5)
project(RIGG)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static -static-libgcc -static-libstdc++")

set(SOURCE_FILES
        # examples.cpp
        src/main.cpp
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
        src/summaries/Box.cpp
        src/summaries/Box.h
        src/summaries/Clause.cpp
        src/summaries/Clause.h
        src/summaries/Formula.cpp
        src/summaries/Formula.h
        src/common/Printable.h
        src/summaries/Ternary.h
        src/summaries/WorklistKleene.cpp
        src/summaries/WorklistKleene.h
        src/saturation/Determinizer.cpp
        src/saturation/Determinizer.h
        src/saturation/GamePDS.cpp
        src/saturation/GamePDS.h
        src/saturation/GrammarDFAtoPDSAFA.cpp
        src/saturation/GrammarDFAtoPDSAFA.h
        src/saturation/PDSTransition.cpp
        src/saturation/PDSTransition.h
        src/saturation/PAFA.cpp
        src/saturation/PAFA.h
        src/saturation/AFATransition.cpp
        src/saturation/AFATransition.h
        src/saturation/Cachat.cpp
        src/saturation/Cachat.h
        src/saturation/CachatWorklist.cpp
        src/saturation/CachatWorklist.h
        src/randomgen/AlphabetGen.cpp
        src/randomgen/AlphabetGen.h
        src/randomgen/TVAutomataGen.cpp
        src/randomgen/TVAutomataGen.h
        src/common/PointeeComparator.h
        src/randomgen/TVGrammarGen.cpp
        src/randomgen/TVGrammarGen.h
        src/summaries/NaiveKleene.cpp
        src/summaries/NaiveKleene.h
        src/saturation/Minimizer.cpp
        src/saturation/Minimizer.h
        src/summaries/FormulaStorage.h
    )

add_executable(RIGG ${SOURCE_FILES})

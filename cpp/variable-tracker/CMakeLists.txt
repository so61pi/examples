cmake_minimum_required(VERSION 3.5)

project(variable-tracker)
set(CMAKE_CXX_STANDARD 14)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(LLVM_LIBRARY_DIR /usr/lib/llvm-3.8/lib)
set(LLVM_INCLUDE_DIR /usr/lib/llvm-3.8/include)

add_executable(${PROJECT_NAME} "main.cpp")
target_include_directories(${PROJECT_NAME} PRIVATE ${LLVM_INCLUDE_DIR})

find_library(LLVM_LIB_LLVMLTO                               LLVMLTO                             PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMObjCARCOpts                       LLVMObjCARCOpts                     PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMSymbolize                         LLVMSymbolize                       PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMDebugInfoPDB                      LLVMDebugInfoPDB                    PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMDebugInfoDWARF                    LLVMDebugInfoDWARF                  PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMXCoreDisassembler                 LLVMXCoreDisassembler               PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMXCoreCodeGen                      LLVMXCoreCodeGen                    PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMXCoreDesc                         LLVMXCoreDesc                       PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMXCoreInfo                         LLVMXCoreInfo                       PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMXCoreAsmPrinter                   LLVMXCoreAsmPrinter                 PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMSystemZDisassembler               LLVMSystemZDisassembler             PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMSystemZCodeGen                    LLVMSystemZCodeGen                  PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMSystemZAsmParser                  LLVMSystemZAsmParser                PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMSystemZDesc                       LLVMSystemZDesc                     PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMSystemZInfo                       LLVMSystemZInfo                     PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMSystemZAsmPrinter                 LLVMSystemZAsmPrinter               PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMSparcDisassembler                 LLVMSparcDisassembler               PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMSparcCodeGen                      LLVMSparcCodeGen                    PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMSparcAsmParser                    LLVMSparcAsmParser                  PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMSparcDesc                         LLVMSparcDesc                       PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMSparcInfo                         LLVMSparcInfo                       PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMSparcAsmPrinter                   LLVMSparcAsmPrinter                 PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMPowerPCDisassembler               LLVMPowerPCDisassembler             PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMPowerPCCodeGen                    LLVMPowerPCCodeGen                  PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMPowerPCAsmParser                  LLVMPowerPCAsmParser                PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMPowerPCDesc                       LLVMPowerPCDesc                     PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMPowerPCInfo                       LLVMPowerPCInfo                     PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMPowerPCAsmPrinter                 LLVMPowerPCAsmPrinter               PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMNVPTXCodeGen                      LLVMNVPTXCodeGen                    PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMNVPTXDesc                         LLVMNVPTXDesc                       PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMNVPTXInfo                         LLVMNVPTXInfo                       PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMNVPTXAsmPrinter                   LLVMNVPTXAsmPrinter                 PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMMSP430CodeGen                     LLVMMSP430CodeGen                   PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMMSP430Desc                        LLVMMSP430Desc                      PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMMSP430Info                        LLVMMSP430Info                      PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMMSP430AsmPrinter                  LLVMMSP430AsmPrinter                PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMMipsDisassembler                  LLVMMipsDisassembler                PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMMipsCodeGen                       LLVMMipsCodeGen                     PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMMipsAsmParser                     LLVMMipsAsmParser                   PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMMipsDesc                          LLVMMipsDesc                        PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMMipsInfo                          LLVMMipsInfo                        PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMMipsAsmPrinter                    LLVMMipsAsmPrinter                  PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMHexagonDisassembler               LLVMHexagonDisassembler             PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMHexagonCodeGen                    LLVMHexagonCodeGen                  PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMHexagonAsmParser                  LLVMHexagonAsmParser                PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMHexagonDesc                       LLVMHexagonDesc                     PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMHexagonInfo                       LLVMHexagonInfo                     PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMCppBackendCodeGen                 LLVMCppBackendCodeGen               PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMCppBackendInfo                    LLVMCppBackendInfo                  PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMBPFCodeGen                        LLVMBPFCodeGen                      PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMBPFDesc                           LLVMBPFDesc                         PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMBPFInfo                           LLVMBPFInfo                         PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMBPFAsmPrinter                     LLVMBPFAsmPrinter                   PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMARMDisassembler                   LLVMARMDisassembler                 PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMARMCodeGen                        LLVMARMCodeGen                      PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMARMAsmParser                      LLVMARMAsmParser                    PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMARMDesc                           LLVMARMDesc                         PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMARMInfo                           LLVMARMInfo                         PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMARMAsmPrinter                     LLVMARMAsmPrinter                   PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMAMDGPUCodeGen                     LLVMAMDGPUCodeGen                   PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMAMDGPUAsmParser                   LLVMAMDGPUAsmParser                 PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMAMDGPUDesc                        LLVMAMDGPUDesc                      PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMAMDGPUUtils                       LLVMAMDGPUUtils                     PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMAMDGPUInfo                        LLVMAMDGPUInfo                      PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMAMDGPUAsmPrinter                  LLVMAMDGPUAsmPrinter                PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMAArch64Disassembler               LLVMAArch64Disassembler             PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMAArch64CodeGen                    LLVMAArch64CodeGen                  PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMAArch64AsmParser                  LLVMAArch64AsmParser                PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMAArch64Desc                       LLVMAArch64Desc                     PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMAArch64Info                       LLVMAArch64Info                     PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMAArch64AsmPrinter                 LLVMAArch64AsmPrinter               PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMAArch64Utils                      LLVMAArch64Utils                    PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMMIRParser                         LLVMMIRParser                       PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMLibDriver                         LLVMLibDriver                       PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMOption                            LLVMOption                          PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMTableGen                          LLVMTableGen                        PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMLineEditor                        LLVMLineEditor                      PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMX86Disassembler                   LLVMX86Disassembler                 PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMX86AsmParser                      LLVMX86AsmParser                    PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMX86CodeGen                        LLVMX86CodeGen                      PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMSelectionDAG                      LLVMSelectionDAG                    PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMAsmPrinter                        LLVMAsmPrinter                      PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMX86Desc                           LLVMX86Desc                         PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMMCDisassembler                    LLVMMCDisassembler                  PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMX86Info                           LLVMX86Info                         PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMX86AsmPrinter                     LLVMX86AsmPrinter                   PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMX86Utils                          LLVMX86Utils                        PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMMCJIT                             LLVMMCJIT                           PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMPasses                            LLVMPasses                          PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMipo                               LLVMipo                             PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMVectorize                         LLVMVectorize                       PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMLinker                            LLVMLinker                          PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMIRReader                          LLVMIRReader                        PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMAsmParser                         LLVMAsmParser                       PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMDebugInfoCodeView                 LLVMDebugInfoCodeView               PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMInterpreter                       LLVMInterpreter                     PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMCodeGen                           LLVMCodeGen                         PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMScalarOpts                        LLVMScalarOpts                      PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMInstCombine                       LLVMInstCombine                     PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMInstrumentation                   LLVMInstrumentation                 PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMProfileData                       LLVMProfileData                     PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMBitWriter                         LLVMBitWriter                       PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMOrcJIT                            LLVMOrcJIT                          PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMTransformUtils                    LLVMTransformUtils                  PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMExecutionEngine                   LLVMExecutionEngine                 PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMTarget                            LLVMTarget                          PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMAnalysis                          LLVMAnalysis                        PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMRuntimeDyld                       LLVMRuntimeDyld                     PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMObject                            LLVMObject                          PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMMCParser                          LLVMMCParser                        PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMBitReader                         LLVMBitReader                       PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMMC                                LLVMMC                              PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMCore                              LLVMCore                            PATHS ${LLVM_LIBRARY_DIR})
find_library(LLVM_LIB_LLVMSupport                           LLVMSupport                         PATHS ${LLVM_LIBRARY_DIR})

find_library(CLANG_LIB_clangARCMigrate                      clangARCMigrate                     PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangAST                             clangAST                            PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangASTMatchers                     clangASTMatchers                    PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangAnalysis                        clangAnalysis                       PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangApplyReplacements               clangApplyReplacements              PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangBasic                           clangBasic                          PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangCodeGen                         clangCodeGen                        PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangDriver                          clangDriver                         PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangDynamicASTMatchers              clangDynamicASTMatchers             PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangEdit                            clangEdit                           PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangFormat                          clangFormat                         PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangFrontend                        clangFrontend                       PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangFrontendTool                    clangFrontendTool                   PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangIndex                           clangIndex                          PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangLex                             clangLex                            PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangParse                           clangParse                          PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangQuery                           clangQuery                          PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangRename                          clangRename                         PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangRewrite                         clangRewrite                        PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangRewriteFrontend                 clangRewriteFrontend                PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangSema                            clangSema                           PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangSerialization                   clangSerialization                  PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangStaticAnalyzerCheckers          clangStaticAnalyzerCheckers         PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangStaticAnalyzerCore              clangStaticAnalyzerCore             PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangStaticAnalyzerFrontend          clangStaticAnalyzerFrontend         PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangTidy                            clangTidy                           PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangTidyCERTModule                  clangTidyCERTModule                 PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangTidyCppCoreGuidelinesModule     clangTidyCppCoreGuidelinesModule    PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangTidyGoogleModule                clangTidyGoogleModule               PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangTidyLLVMModule                  clangTidyLLVMModule                 PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangTidyMiscModule                  clangTidyMiscModule                 PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangTidyModernizeModule             clangTidyModernizeModule            PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangTidyPerformanceModule           clangTidyPerformanceModule          PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangTidyReadabilityModule           clangTidyReadabilityModule          PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangTidyUtils                       clangTidyUtils                      PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangTooling                         clangTooling                        PATHS ${LLVM_LIBRARY_DIR})
find_library(CLANG_LIB_clangToolingCore                     clangToolingCore                    PATHS ${LLVM_LIBRARY_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${CLANG_LIB_clangARCMigrate}
    ${CLANG_LIB_clangAST}
    ${CLANG_LIB_clangASTMatchers}
    ${CLANG_LIB_clangAnalysis}
    ${CLANG_LIB_clangApplyReplacements}
    ${CLANG_LIB_clangBasic}
    ${CLANG_LIB_clangCodeGen}
    ${CLANG_LIB_clangDriver}
    ${CLANG_LIB_clangDynamicASTMatchers}
    ${CLANG_LIB_clangEdit}
    ${CLANG_LIB_clangFormat}
    ${CLANG_LIB_clangFrontend}
    ${CLANG_LIB_clangFrontendTool}
    ${CLANG_LIB_clangIndex}
    ${CLANG_LIB_clangLex}
    ${CLANG_LIB_clangParse}
    ${CLANG_LIB_clangQuery}
    ${CLANG_LIB_clangRename}
    ${CLANG_LIB_clangRewrite}
    ${CLANG_LIB_clangRewriteFrontend}
    ${CLANG_LIB_clangSema}
    ${CLANG_LIB_clangSerialization}
    ${CLANG_LIB_clangStaticAnalyzerCheckers}
    ${CLANG_LIB_clangStaticAnalyzerCore}
    ${CLANG_LIB_clangStaticAnalyzerFrontend}
    ${CLANG_LIB_clangTidy}
    ${CLANG_LIB_clangTidyCERTModule}
    ${CLANG_LIB_clangTidyCppCoreGuidelinesModule}
    ${CLANG_LIB_clangTidyGoogleModule}
    ${CLANG_LIB_clangTidyLLVMModule}
    ${CLANG_LIB_clangTidyMiscModule}
    ${CLANG_LIB_clangTidyModernizeModule}
    ${CLANG_LIB_clangTidyPerformanceModule}
    ${CLANG_LIB_clangTidyReadabilityModule}
    ${CLANG_LIB_clangTidyUtils}
    ${CLANG_LIB_clangTooling}
    ${CLANG_LIB_clangToolingCore}

    # duplicate the clang libs to prevent library order headache
    ${CLANG_LIB_clangARCMigrate}
    ${CLANG_LIB_clangAST}
    ${CLANG_LIB_clangASTMatchers}
    ${CLANG_LIB_clangAnalysis}
    ${CLANG_LIB_clangApplyReplacements}
    ${CLANG_LIB_clangBasic}
    ${CLANG_LIB_clangCodeGen}
    ${CLANG_LIB_clangDriver}
    ${CLANG_LIB_clangDynamicASTMatchers}
    ${CLANG_LIB_clangEdit}
    ${CLANG_LIB_clangFormat}
    ${CLANG_LIB_clangFrontend}
    ${CLANG_LIB_clangFrontendTool}
    ${CLANG_LIB_clangIndex}
    ${CLANG_LIB_clangLex}
    ${CLANG_LIB_clangParse}
    ${CLANG_LIB_clangQuery}
    ${CLANG_LIB_clangRename}
    ${CLANG_LIB_clangRewrite}
    ${CLANG_LIB_clangRewriteFrontend}
    ${CLANG_LIB_clangSema}
    ${CLANG_LIB_clangSerialization}
    ${CLANG_LIB_clangStaticAnalyzerCheckers}
    ${CLANG_LIB_clangStaticAnalyzerCore}
    ${CLANG_LIB_clangStaticAnalyzerFrontend}
    ${CLANG_LIB_clangTidy}
    ${CLANG_LIB_clangTidyCERTModule}
    ${CLANG_LIB_clangTidyCppCoreGuidelinesModule}
    ${CLANG_LIB_clangTidyGoogleModule}
    ${CLANG_LIB_clangTidyLLVMModule}
    ${CLANG_LIB_clangTidyMiscModule}
    ${CLANG_LIB_clangTidyModernizeModule}
    ${CLANG_LIB_clangTidyPerformanceModule}
    ${CLANG_LIB_clangTidyReadabilityModule}
    ${CLANG_LIB_clangTidyUtils}
    ${CLANG_LIB_clangTooling}
    ${CLANG_LIB_clangToolingCore}
    )

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${LLVM_LIB_LLVMLTO}
    ${LLVM_LIB_LLVMObjCARCOpts}
    ${LLVM_LIB_LLVMSymbolize}
    ${LLVM_LIB_LLVMDebugInfoPDB}
    ${LLVM_LIB_LLVMDebugInfoDWARF}
    ${LLVM_LIB_LLVMXCoreDisassembler}
    ${LLVM_LIB_LLVMXCoreCodeGen}
    ${LLVM_LIB_LLVMXCoreDesc}
    ${LLVM_LIB_LLVMXCoreInfo}
    ${LLVM_LIB_LLVMXCoreAsmPrinter}
    ${LLVM_LIB_LLVMSystemZDisassembler}
    ${LLVM_LIB_LLVMSystemZCodeGen}
    ${LLVM_LIB_LLVMSystemZAsmParser}
    ${LLVM_LIB_LLVMSystemZDesc}
    ${LLVM_LIB_LLVMSystemZInfo}
    ${LLVM_LIB_LLVMSystemZAsmPrinter}
    ${LLVM_LIB_LLVMSparcDisassembler}
    ${LLVM_LIB_LLVMSparcCodeGen}
    ${LLVM_LIB_LLVMSparcAsmParser}
    ${LLVM_LIB_LLVMSparcDesc}
    ${LLVM_LIB_LLVMSparcInfo}
    ${LLVM_LIB_LLVMSparcAsmPrinter}
    ${LLVM_LIB_LLVMPowerPCDisassembler}
    ${LLVM_LIB_LLVMPowerPCCodeGen}
    ${LLVM_LIB_LLVMPowerPCAsmParser}
    ${LLVM_LIB_LLVMPowerPCDesc}
    ${LLVM_LIB_LLVMPowerPCInfo}
    ${LLVM_LIB_LLVMPowerPCAsmPrinter}
    ${LLVM_LIB_LLVMNVPTXCodeGen}
    ${LLVM_LIB_LLVMNVPTXDesc}
    ${LLVM_LIB_LLVMNVPTXInfo}
    ${LLVM_LIB_LLVMNVPTXAsmPrinter}
    ${LLVM_LIB_LLVMMSP430CodeGen}
    ${LLVM_LIB_LLVMMSP430Desc}
    ${LLVM_LIB_LLVMMSP430Info}
    ${LLVM_LIB_LLVMMSP430AsmPrinter}
    ${LLVM_LIB_LLVMMipsDisassembler}
    ${LLVM_LIB_LLVMMipsCodeGen}
    ${LLVM_LIB_LLVMMipsAsmParser}
    ${LLVM_LIB_LLVMMipsDesc}
    ${LLVM_LIB_LLVMMipsInfo}
    ${LLVM_LIB_LLVMMipsAsmPrinter}
    ${LLVM_LIB_LLVMHexagonDisassembler}
    ${LLVM_LIB_LLVMHexagonCodeGen}
    ${LLVM_LIB_LLVMHexagonAsmParser}
    ${LLVM_LIB_LLVMHexagonDesc}
    ${LLVM_LIB_LLVMHexagonInfo}
    ${LLVM_LIB_LLVMCppBackendCodeGen}
    ${LLVM_LIB_LLVMCppBackendInfo}
    ${LLVM_LIB_LLVMBPFCodeGen}
    ${LLVM_LIB_LLVMBPFDesc}
    ${LLVM_LIB_LLVMBPFInfo}
    ${LLVM_LIB_LLVMBPFAsmPrinter}
    ${LLVM_LIB_LLVMARMDisassembler}
    ${LLVM_LIB_LLVMARMCodeGen}
    ${LLVM_LIB_LLVMARMAsmParser}
    ${LLVM_LIB_LLVMARMDesc}
    ${LLVM_LIB_LLVMARMInfo}
    ${LLVM_LIB_LLVMARMAsmPrinter}
    ${LLVM_LIB_LLVMAMDGPUCodeGen}
    ${LLVM_LIB_LLVMAMDGPUAsmParser}
    ${LLVM_LIB_LLVMAMDGPUDesc}
    ${LLVM_LIB_LLVMAMDGPUUtils}
    ${LLVM_LIB_LLVMAMDGPUInfo}
    ${LLVM_LIB_LLVMAMDGPUAsmPrinter}
    ${LLVM_LIB_LLVMAArch64Disassembler}
    ${LLVM_LIB_LLVMAArch64CodeGen}
    ${LLVM_LIB_LLVMAArch64AsmParser}
    ${LLVM_LIB_LLVMAArch64Desc}
    ${LLVM_LIB_LLVMAArch64Info}
    ${LLVM_LIB_LLVMAArch64AsmPrinter}
    ${LLVM_LIB_LLVMAArch64Utils}
    ${LLVM_LIB_LLVMMIRParser}
    ${LLVM_LIB_LLVMLibDriver}
    ${LLVM_LIB_LLVMOption}
    ${LLVM_LIB_LLVMTableGen}
    ${LLVM_LIB_LLVMLineEditor}
    ${LLVM_LIB_LLVMX86Disassembler}
    ${LLVM_LIB_LLVMX86AsmParser}
    ${LLVM_LIB_LLVMX86CodeGen}
    ${LLVM_LIB_LLVMSelectionDAG}
    ${LLVM_LIB_LLVMAsmPrinter}
    ${LLVM_LIB_LLVMX86Desc}
    ${LLVM_LIB_LLVMMCDisassembler}
    ${LLVM_LIB_LLVMX86Info}
    ${LLVM_LIB_LLVMX86AsmPrinter}
    ${LLVM_LIB_LLVMX86Utils}
    ${LLVM_LIB_LLVMMCJIT}
    ${LLVM_LIB_LLVMPasses}
    ${LLVM_LIB_LLVMipo}
    ${LLVM_LIB_LLVMVectorize}
    ${LLVM_LIB_LLVMLinker}
    ${LLVM_LIB_LLVMIRReader}
    ${LLVM_LIB_LLVMAsmParser}
    ${LLVM_LIB_LLVMDebugInfoCodeView}
    ${LLVM_LIB_LLVMInterpreter}
    ${LLVM_LIB_LLVMCodeGen}
    ${LLVM_LIB_LLVMScalarOpts}
    ${LLVM_LIB_LLVMInstCombine}
    ${LLVM_LIB_LLVMInstrumentation}
    ${LLVM_LIB_LLVMProfileData}
    ${LLVM_LIB_LLVMBitWriter}
    ${LLVM_LIB_LLVMOrcJIT}
    ${LLVM_LIB_LLVMTransformUtils}
    ${LLVM_LIB_LLVMExecutionEngine}
    ${LLVM_LIB_LLVMTarget}
    ${LLVM_LIB_LLVMAnalysis}
    ${LLVM_LIB_LLVMRuntimeDyld}
    ${LLVM_LIB_LLVMObject}
    ${LLVM_LIB_LLVMMCParser}
    ${LLVM_LIB_LLVMBitReader}
    ${LLVM_LIB_LLVMMC}
    ${LLVM_LIB_LLVMCore}
    ${LLVM_LIB_LLVMSupport}
    )

target_link_libraries(${PROJECT_NAME} PRIVATE
    Threads::Threads
    tinfo
    dl
    z
    )

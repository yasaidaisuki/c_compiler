#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "parser.hh"
#include "driver.h"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetRegistry.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/MC/MCAsmInfo.h>
#include <llvm/MC/MCObjectFileInfo.h>
#include <llvm/MC/MCContext.h>
#include <llvm/CodeGen/CommandFlags.inc>

// #include "build/y.tab.h"
using namespace std;

// Global variable to hold the root of the AST
unique_ptr<TopLevel> root;

int main(int argc, char *argv[])
{

    if (argc == 1)
    {
        cerr << "Usage: " << argv[0] << " <source-file.c>\n";
        return 1;
    }
    Mreow::Driver drv{};
    int n = drv.parse(argv[1]);
    if (n != 0)
        std::cout << "fail\n";
        return 1;
    else
        std::cout << "ok\n";
    
    // Initialize LLVM components
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder(context);
    auto module = std::make_unique<llvm::Module>("my_module", context);

    // given that parse is ok, TopLevel should be defined & not nullptr

    // Generate LLVM IR from the AST
    topLevel->codegen(context, builder, *module);
    
    // Verify the generated code
    llvm::verifyModule(*module, &llvm::errs());

    // Print IR to stdout for debugging
    module->print(llvm::outs(), nullptr);

    // Write the IR to an object file
    std::error_code EC;
    llvm::raw_fd_ostream dest("output.o", EC, llvm::sys::fs::OF_None);

    if (EC) {
        llvm::errs() << "Could not open file: " << EC.message();
        return 1;
    }

    // Create object code
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();
    auto TargetTriple = llvm::sys::getDefaultTargetTriple();
    module->setTargetTriple(TargetTriple);

    std::string Error;
    auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

    if (!Target) {
        llvm::errs() << Error;
        return 1;
    }

    // setting target machine to general CPU
    auto CPU = "generic";
    auto Features = "";

    llvm::TargetOptions opt;
    auto RM = llvm::Optional<llvm::Reloc::Model>();
    auto TheTargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

    module->setDataLayout(TheTargetMachine->createDataLayout());

    llvm::legacy::PassManager pass;
    if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, llvm::CGFT_ObjectFile)) {
        llvm::errs() << "TheTargetMachine can't emit a file of this type";
        return 1;
    }

    pass.run(*module);
    dest.flush();

    llvm::outs() << "Wrote object file to output.o\n";

}